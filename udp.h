#ifndef UDP_H
#define UDP_H

#include "client.h"


int udp_start(const char* server_address, StatusCallback status_callback, int delayd);
void udp_stop();

#endif
