#ifndef CLIENT_H
#define CLIENT_H

typedef enum {
    STATUS_PENDING,
    STATUS_COMPLETED,
    STATUS_ERROR
} JobStatus;

typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} LogLevel;

typedef void (*StatusCallback)(JobStatus status);

// struct for configs
typedef struct {
    const char* server_address;
    LogLevel log_level;
    int delay;
    int max_retries;
} ClientConfig;

// initializes the library
int client_init(ClientConfig* config);

// registers callback for lcient updates
void client_register_callback(StatusCallback callback);

// starts the client
int client_start();

// stops the client
void client_stop();

//returns the last known status update
void status_update(JobStatus status);


#endif
