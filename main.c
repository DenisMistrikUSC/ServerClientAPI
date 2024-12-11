#include <stdio.h>
#include <unistd.h>
#include "client.h"
#include "utils.h"

void status_update_handler(JobStatus status) {
    switch (status) {
        case STATUS_PENDING:
            log_info("Job status: Pending");
            break;
        case STATUS_COMPLETED:
            log_info("Job status: Completed");
            break;
        case STATUS_ERROR:
            log_info("Job status: Error");
            break;
    }
}

int main() {
    ClientConfig config;
    config.server_address = "127.0.0.1"; 
    config.log_level = LOG_LEVEL_DEBUG;
    config.delay = 2;
    config.max_retries = 3;
    if (client_init(&config) != 0) {
        log_error("Failed to initialize client");
        return -1;
    }
    client_register_callback(status_update_handler);
    if (client_start() != 0) {
        log_error("Failed to start client");
        return -1;
    }
    while (1) {
        sleep(1);
    }
    client_stop();

    return 0;
}
