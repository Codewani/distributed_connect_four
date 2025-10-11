#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define DEFAULT_PORT "12345"
#define MAX_MSG 1024

static void usage(const char* prog) {
    fprintf(stderr, "Usage: %s <server-ip> [port]\n", prog);
}

int main(int argc, char const* argv[])
{
    if (argc < 2) {
        usage(argv[0]);
        return 1;
    }

    const char* server = argv[1];
    const char* port = (argc >= 3) ? argv[2] : DEFAULT_PORT;

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(server, port, &hints, &res) != 0) {
        perror("getaddrinfo");
        return 1;
    }

    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock < 0) {
        perror("socket");
        freeaddrinfo(res);
        return 1;
    }

    if (connect(sock, res->ai_addr, res->ai_addrlen) < 0) {
        perror("connect");
        close(sock);
        freeaddrinfo(res);
        return 1;
    }

    freeaddrinfo(res);

    char buf[MAX_MSG];
    while (1) {
        ssize_t r = recv(sock, buf, sizeof(buf)-1, 0);
        if (r <= 0) break;
        buf[r] = '\0';
        if (strncmp(buf, "WELCOME", 7) == 0) {
            printf("%s", buf);
            continue;
        } else if (strncmp(buf, "YOUR_TURN", 9) == 0) {
            printf("Your turn. Enter move as '(x, y)': ");
            if (fgets(buf, sizeof(buf), stdin) == NULL) break;
            send(sock, buf, strlen(buf), 0);
        } else if (strncmp(buf, "WAIT", 4) == 0) {
            printf("Waiting for opponent...\n");
        } else if (strncmp(buf, "MOVE", 4) == 0) {
            printf("Server: %s", buf);
        } else if (strncmp(buf, "WINNER", 6) == 0) {
            printf("%s", buf);
            break;
        } else if (strncmp(buf, "INVALID", 7) == 0 || strncmp(buf, "OUT_OF_BOUNDS", 13) == 0 || strncmp(buf, "INVALID_MOVE", 12) == 0) {
            printf("Server: %s", buf);
        } else {
            printf("Server: %s", buf);
        }
    }

    close(sock);
    return 0;
}