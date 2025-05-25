#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 12345
#define MAX_MSG 1024

int main(int argc, char const* argv[])
{
    int status, valread, client_fd;
    struct sockaddr_in6 serv_addr;
    char* hello = "Hello from client\n";
    char buffer[MAX_MSG] = { 0 };
    char input[MAX_MSG];
    if ((client_fd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET6, "2601:c4:c180:3c80:796b:a35f:dcef:a7b6", &serv_addr.sin6_addr) <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))< 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
  
    // subtract 1 for the null
    // terminator at the end

    while (1) {
        // Get input from user
        printf("Enter message (or 'quit' to exit): ");
        fgets(input, MAX_MSG, stdin);
        
        if (strncmp(input, "quit", 4) == 0) {
            break;
        }

        // Send message
        send(client_fd, input, strlen(input), 0);
        
        // Receive response
        valread = read(client_fd, buffer, MAX_MSG - 1);
        if (valread > 0) {
            printf("Server: %s\n", buffer);
            memset(buffer, 0, MAX_MSG);  // Clear buffer for next read
        }
    }

    shutdown(client_fd, SHUT_WR);

    valread = read(client_fd, buffer, 1024 - 1); 
    printf("%s\n", buffer);

    // closing the connected socket
    close(client_fd);
    return 0;
}