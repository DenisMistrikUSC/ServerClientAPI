#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define UDP_PORT 12345

void send_status_updates(int sockfd, struct sockaddr_in* client_addr, socklen_t addr_len) {
    const char* statuses[] = {
        "{\"result\": \"pending\"}", "{\"result\": \"pending\"}", "{\"result\": \"completed\"}", "{\"result\": \"error\"}"
    };
    int status_index = 0;

    while (1) {
        const char* message = statuses[status_index];
        sendto(sockfd, message, strlen(message), 0, (struct sockaddr*)client_addr, addr_len);
        printf("Sent status: %s\n", message);
        status_index = (status_index + 1) % 4;
        sleep(2);
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(UDP_PORT);

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP server is running on port %d\n", UDP_PORT);
    char buffer[1024];
    ssize_t recv_len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &addr_len);
    if (recv_len > 0) {
        printf("Received message from client. Starting to send status updates.\n");
        send_status_updates(sockfd, &client_addr, addr_len);
    }

    close(sockfd);
    return 0;
}
