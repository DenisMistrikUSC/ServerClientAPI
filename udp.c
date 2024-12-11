#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "udp.h"
#include "utils.h"

#define UDP_PORT 12345

static pthread_t udp_thread;
static int running = 0;
static StatusCallback status_callback = NULL;
static int udp_socket = -1;
int delay = 1;
void* udp_receive_thread(void* arg) {
    char buffer[1024];
    struct sockaddr_in from_addr;
    socklen_t addr_len = sizeof(from_addr);
    log_warn("Running Forever");
    while (running) {
        ssize_t recv_len = recvfrom(udp_socket, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&from_addr, &addr_len);
        if (recv_len > 0) {
            buffer[recv_len] = '\0';
            log_debug("Recieved %s", buffer);
            JobStatus status = parse_status_message(buffer);
            status_update(status);
        }
        sleep(delay);
    }
    return NULL;
}

int udp_start(const char* server_address, StatusCallback callback, int delayd) {
    status_callback = callback;
    running = 1;
    delay = delayd;
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket < 0) {
        perror("socket");
        return -1;
    }
    struct sockaddr_in local_addr;
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = 0;

    if (bind(udp_socket, (struct sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
        perror("bind");
        close(udp_socket);
        return -1;
    }
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_PORT);
    if (inet_pton(AF_INET, server_address, &server_addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid server address\n");
        close(udp_socket);
        return -1;
    }
    log_debug("Connected to server at %s:%d", server_address, UDP_PORT);
    const char* init_message = "Client ready";
    sendto(udp_socket, init_message, strlen(init_message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (pthread_create(&udp_thread, NULL, udp_receive_thread, NULL) != 0) {
        fprintf(stderr, "Failed to create UDP receive thread\n");
        close(udp_socket);
        return -1;
    }

    printf("UDP client started and sent initial message to server\n");
    return 0;
}


void udp_stop() {
    running = 0;
    pthread_join(udp_thread, NULL);
    if (udp_socket >= 0) {
        close(udp_socket);
        udp_socket = -1;
    }
}
