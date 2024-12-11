# ServerClientAPI

Server client api, easily extendable to different protocols (ie tcp), 
Currently only uses udp for speed.

Features:
Configurable log levels
Thread safe operations
Real time status updates

Compiling:
gcc -pthread -o client main.c client.c udp.c utils.c
gcc -o udp_server udp_server.c


Rundown of API:

There is example usage within main.c utilizes udp to receive messages from a server continuously, 
It can also receive the latest status using the client_get_latest_status() function found within the client script

Uses the port 12345 by default, when the server recieves the initial client ready message it cycles through
the statuses pending, complete, error to simulate the functionality of a video translation server.

Configuration options:
Defined using the ClientConfig struct
Has the following settings:
Log level, server address, the delay between reading from the socket, and max retries

Logging:
The API has multiple levels of configurable logging depending on your needs:
Log debug: extra verbosity for debugging purposes
Log info: For general information
Log warn: For warnings
Log error: For critical errors only

Integration shell script
run it using the following commands
chmod +x integration_test.sh
./integration_test.sh

Manual testing:
Run ./udp_server
Run ./client in seperate terminal

Troubleshooting:
If the port is still bound and cannot be reused
lsof -i :12345
kill <PID>


Prerequistes: 
gcc installed
support for pthreads