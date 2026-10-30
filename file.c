#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void) {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[4096];
    const char *payload =
        "{\"name\":\"Alice\",\"status\":\"ok\"}";

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sockfd);
        return 1;
    }

    snprintf(buffer, sizeof(buffer),
        "POST /api/data HTTP/1.1\r\n"
        "Host: 127.0.0.1:8080\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        strlen(payload),
        payload);

    send(sockfd, buffer, strlen(buffer), 0);

    int received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (received > 0) {
        buffer[received] = '\0';
        printf("%s\n", buffer);
    }

    close(sockfd);
    return 0;
}