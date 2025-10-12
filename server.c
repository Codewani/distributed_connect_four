#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "connect_4_helpers.h"

#define PORT 12345
#define MAX_MSG 256

static int player_fds[3] = { -1, -1, -1 }; // index 1 and 2
static char board[6][7];
static int current_turn = 1; // 1 or 2
static int winner = 0;

static pthread_mutex_t game_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t turn_cond = PTHREAD_COND_INITIALIZER;

static void send_to_player(int fd, const char* msg) {
    send(fd, msg, strlen(msg), 0);
}

static void broadcast(const char* msg) {
    for (int p = 1; p <= 2; p++) {
        if (player_fds[p] != -1) send_to_player(player_fds[p], msg);
    }
}

struct client_arg { int fd; int player_id; };

static void* client_thread(void* v) {
    struct client_arg* a = (struct client_arg*)v;
    int fd = a->fd;
    int pid = a->player_id;
    char buf[MAX_MSG];

    // Welcome already sent by main acceptor
    while (!winner) {
        // Wait for my turn
        pthread_mutex_lock(&game_lock);
        while (current_turn != pid && !winner) {
            send_to_player(fd, "WAIT\n");
            pthread_cond_wait(&turn_cond, &game_lock);
        }
        if (winner) {
            pthread_mutex_unlock(&game_lock);
            break;
        }
        // It's my turn
        send_to_player(fd, "YOUR_TURN\n");
        pthread_mutex_unlock(&game_lock);

        ssize_t r = recv(fd, buf, sizeof(buf)-1, 0);
        if (r <= 0) break; // disconnect or error
        buf[r] = '\0';
        int x, y;
        if (sscanf(buf, " ( %d , %d ) ", &x, &y) != 2) {
            send_to_player(fd, "INVALID\n");
            // still my turn; continue loop
            pthread_mutex_lock(&game_lock);
            pthread_cond_broadcast(&turn_cond);
            pthread_mutex_unlock(&game_lock);
            continue;
        }
        if (x < 0 || x > 5 || y < 0 || y > 6) {
            send_to_player(fd, "OUT_OF_BOUNDS\n");
            pthread_mutex_lock(&game_lock);
            pthread_cond_broadcast(&turn_cond);
            pthread_mutex_unlock(&game_lock);
            continue;
        }
        pthread_mutex_lock(&game_lock);
        if (!is_valid_move(x, y)) {
            send_to_player(fd, "INVALID_MOVE\n");
            pthread_cond_broadcast(&turn_cond);
            pthread_mutex_unlock(&game_lock);
            continue;
        }

        // apply move
        board[x][y] = (pid == 1) ? 'X' : 'O';
        make_move(x, y, (pid == 1) ? 'X' : 'O');
        char notify[MAX_MSG];
        snprintf(notify, sizeof(notify), "MOVE %d %d %d\n", pid, x, y);
        broadcast(notify);

        if (check_winner(board, (pid == 1) ? 'X' : 'O')) {
            snprintf(notify, sizeof(notify), "WINNER %d\n", pid);
            broadcast(notify);
            winner = 1;
            pthread_cond_broadcast(&turn_cond);
            pthread_mutex_unlock(&game_lock);
            break;
        }

        // next player's turn
        current_turn = (current_turn == 1) ? 2 : 1;
        pthread_cond_broadcast(&turn_cond);
        pthread_mutex_unlock(&game_lock);
    }

    close(fd);
    return NULL;
}

int main(void) {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 2) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for player 1...\n");
    int s1 = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (s1 < 0) { perror("accept"); exit(EXIT_FAILURE); }
    player_fds[1] = s1;
    send_to_player(s1, "WELCOME 1\n");
    printf("Player 1 connected\n");

    printf("Waiting for player 2...\n");
    int s2 = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (s2 < 0) { perror("accept"); exit(EXIT_FAILURE); }
    player_fds[2] = s2;
    send_to_player(s2, "WELCOME 2\n");
    printf("Player 2 connected\n");

    // initialize game state
    for (int i = 0; i < 6; i++) for (int j = 0; j < 7; j++) board[i][j] = '.';
    initialize();

    pthread_t t1, t2;
    struct client_arg a1 = { player_fds[1], 1 };
    struct client_arg a2 = { player_fds[2], 2 };

    pthread_create(&t1, NULL, client_thread, &a1);
    pthread_create(&t2, NULL, client_thread, &a2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    close(server_fd);
    return 0;
}
