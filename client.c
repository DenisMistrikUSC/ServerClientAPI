#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#include "udp.h"
#include "utils.h"

LogLevel log_level = LOG_LEVEL_INFO; 
static int use_websocket = 0;
static char* server_address = NULL;
static StatusCallback status_callback = NULL;
//static LogLevel log_level = LOG_LEVEL_INFO;
static int delay = 30;
static int max_retries = 3;
static JobStatus latest_status = STATUS_PENDING;

int client_init(ClientConfig* config) {
    if (config == NULL || config->server_address == NULL) {
        fprintf(stderr, "Invalid client configuration.\n");
        return -1;
    }
    server_address = strdup(config->server_address);
    log_level = config->log_level;
    delay = config->delay;
    max_retries = config->max_retries;
    return 0;
}
void status_update(JobStatus status) {
    latest_status = status;
    if (status_callback) {
        status_callback(status);
    }
}
void client_register_callback(StatusCallback callback) {
    status_callback = callback;
}
JobStatus client_get_latest_status() {
    return latest_status;
}

int client_start() {
    if (status_callback == NULL) {
        log_error("Status callback is not registered.");
        return -1;
    }
    return udp_start(server_address, status_callback, delay);
}

void client_stop() {
     udp_stop();
    if (server_address) {
        free(server_address);
        server_address = NULL;
    }
}
