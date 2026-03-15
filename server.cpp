#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <array>
#include <string>

#include "connect_4_helpers.h"

using namespace std;

#define PORT 12345
#define MAX_MSG 256

static array<int, 3> player_fds = { -1, -1, -1 }; // index 1 and 2
static char board[6][7];
static int current_turn = 1; // 1 or 2
static int winner = 0;

static mutex game_lock;
static condition_variable turn_cond;

static void send_to_player(int fd, const string& msg) {
    send(fd, msg.c_str(), msg.size(), 0);
}

static void broadcast(const string& msg) {
    for (int p = 1; p <= 2; p++) {
        if (player_fds[p] != -1) send_to_player(player_fds[p], msg);
    }
}

struct ClientArg {
    int fd;
    int player_id;
};

static void client_thread(ClientArg arg) {
    int fd = arg.fd;
    int pid = arg.player_id;
    char buf[MAX_MSG];

    // Welcome already sent by main acceptor
    while (!winner) {
        // Wait for my turn
        unique_lock<mutex> lock(game_lock);
        while (current_turn != pid && !winner) {
            send_to_player(fd, "WAIT\n");
            turn_cond.wait(lock);
        }
        if (winner) {
            lock.unlock();
            break;
        }
        // It's my turn
        send_to_player(fd, "YOUR_TURN\n");
        lock.unlock();

        ssize_t r = recv(fd, buf, sizeof(buf)-1, 0);
        if (r <= 0) break; // disconnect or error
        buf[r] = '\0';
        int x, y;
        if (sscanf(buf, " ( %d , %d ) ", &x, &y) != 2) {
            send_to_player(fd, "INVALID\n");
            // still my turn; continue loop
            lock.lock();
            turn_cond.notify_all();
            lock.unlock();
            continue;
        }
        if (x < 0 || x > 5 || y < 0 || y > 6) {
            send_to_player(fd, "OUT_OF_BOUNDS\n");
            lock.lock();
            turn_cond.notify_all();
            lock.unlock();
            continue;
        }
        lock.lock();
        if (!is_valid_move(x, y)) {
            send_to_player(fd, "INVALID_MOVE\n");
            turn_cond.notify_all();
            lock.unlock();
            continue;
        }

        // apply move
        board[x][y] = (pid == 1) ? 'X' : 'O';
        if (make_move(x, y, (pid == 1) ? 'X' : 'O') == 1) {
            string notify = "WINNER " + to_string(pid) + "\n";
            broadcast(notify);
            winner = pid;
            turn_cond.notify_all();
            lock.unlock();
            break;
        }
        string notify = "MOVE " + to_string(pid) + " " + to_string(x) + " " + to_string(y) + "\n";
        broadcast(notify);

        // next player's turn
        current_turn = (current_turn == 1) ? 2 : 1;
        turn_cond.notify_all();
        lock.unlock();
    }

    close(fd);
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0) {
        perror("setsockopt SO_REUSEADDR");
        exit(EXIT_FAILURE);
    }
#ifdef SO_REUSEPORT
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) != 0) {
        perror("setsockopt SO_REUSEPORT");
        /* Not fatal on some platforms; continue */
    }
#endif
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (::bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 2) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    cout << "Waiting for player 1..." << endl;
    int s1 = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (s1 < 0) { perror("accept"); exit(EXIT_FAILURE); }
    player_fds[1] = s1;
    send_to_player(s1, "WELCOME 1\n");
    cout << "Player 1 connected" << endl;

    cout << "Waiting for player 2..." << endl;
    int s2 = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (s2 < 0) { perror("accept"); exit(EXIT_FAILURE); }
    player_fds[2] = s2;
    send_to_player(s2, "WELCOME 2\n");
    cout << "Player 2 connected" << endl;

    // initialize game state
    for (int i = 0; i < 6; i++) for (int j = 0; j < 7; j++) board[i][j] = '.';
    initialize();

    ClientArg a1 = { player_fds[1], 1 };
    ClientArg a2 = { player_fds[2], 2 };

    thread t1(client_thread, a1);
    thread t2(client_thread, a2);

    t1.join();
    t2.join();

    close(server_fd);
    return 0;
}