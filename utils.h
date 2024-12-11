#ifndef UTILS_H
#define UTILS_H

#include "client.h"

JobStatus parse_status_message(const char* message);
extern LogLevel log_level;
void log_debug(const char* format, ...);
void log_info(const char* format, ...);
void log_warn(const char* format, ...);
void log_error(const char* format, ...);

#endif
