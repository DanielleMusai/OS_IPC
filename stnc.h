#ifndef STNC_H
#define STNC_H

// Constants
#define MAX_MESSAGE_LENGTH 1024


// Function declarations
int start_server(int port);
int connect_to_server(char* ip, int port);
void handle_connection(int socket_fd);

#endif
