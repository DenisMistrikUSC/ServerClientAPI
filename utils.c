#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "utils.h"


JobStatus parse_status_message(const char* message) {
    if (strstr(message, "\"pending\"")) {
        return STATUS_PENDING;
    } else if (strstr(message, "\"completed\"")) {
        return STATUS_COMPLETED;
    } else if (strstr(message, "\"error\"")) {
        return STATUS_ERROR;
    } else {
        return STATUS_ERROR;
    }
}

void log_message(LogLevel level, const char* format, va_list args) {
    if (level >= log_level) {
        vprintf(format, args);
        printf("\n");
    }
}

void log_debug(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log_message(LOG_LEVEL_DEBUG, format, args);
    va_end(args);
}

void log_info(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log_message(LOG_LEVEL_INFO, format, args);
    va_end(args);
}

void log_warn(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log_message(LOG_LEVEL_WARN, format, args);
    va_end(args);
}

void log_error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log_message(LOG_LEVEL_ERROR, format, args);
    va_end(args);
}
