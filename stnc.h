#ifndef STNC_H
#define STNC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAX_MSG_LENGTH 128

void error(const char *msg);
void handle_client(int sockfd);
int main(int argc, char *argv[]);

#endif
