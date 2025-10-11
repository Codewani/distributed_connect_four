#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "connect_4_helpers.h"

#define PORT 12345
#define MAX_MSG 256

static void send_all(int fd, const char* msg) {
    send(fd, msg, strlen(msg), 0);
}

int main(void) {
    int server_fd, new_socket1, new_socket2;
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
    if ((new_socket1 = accept(server_fd, (struct sockaddr*)&address,
                             (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("Player 1 connected\n");
    send_all(new_socket1, "WELCOME 1\n");

    printf("Waiting for player 2...\n");
    if ((new_socket2 = accept(server_fd, (struct sockaddr*)&address,
                             (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("Player 2 connected\n");
    send_all(new_socket2, "WELCOME 2\n");

    // initialize game state
    char board[6][7];
    for (int i = 0; i < 6; i++) for (int j = 0; j < 7; j++) board[i][j] = '.';
    initialize();

    int current = 1; // 1 or 2
    int winner = 0;
    char buf[MAX_MSG];

    while (!winner) {
        int active_fd = (current == 1) ? new_socket1 : new_socket2;
        int other_fd = (current == 1) ? new_socket2 : new_socket1;

        send_all(active_fd, "YOUR_TURN\n");
        send_all(other_fd, "WAIT\n");

        ssize_t r = recv(active_fd, buf, sizeof(buf)-1, 0);
        if (r <= 0) break;
        buf[r] = '\0';
        int x, y;
        if (sscanf(buf, " ( %d , %d ) ", &x, &y) != 2) {
            send_all(active_fd, "INVALID\n");
            continue;
        }
        if (x < 0 || x > 5 || y < 0 || y > 6) {
            send_all(active_fd, "OUT_OF_BOUNDS\n");
            continue;
        }
        if (!is_valid_move(x, y)) {
            send_all(active_fd, "INVALID_MOVE\n");
            continue;
        }

        // apply move
        board[x][y] = (current == 1) ? 'X' : 'O';
        make_move(x, y, (current == 1) ? 'X' : 'O');

        // notify both players of move
        char notify[MAX_MSG];
        snprintf(notify, sizeof(notify), "MOVE %d %d %d\n", current, x, y);
        send_all(new_socket1, notify);
        send_all(new_socket2, notify);

        // check winner
        if (check_winner(board, (current == 1) ? 'X' : 'O')) {
            snprintf(notify, sizeof(notify), "WINNER %d\n", current);
            send_all(new_socket1, notify);
            send_all(new_socket2, notify);
            winner = 1;
            break;
        }

        current = (current == 1) ? 2 : 1;
    }

    close(new_socket1);
    close(new_socket2);
    close(server_fd);
    return 0;
}
