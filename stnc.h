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
#include <pthread.h>
#include <arpa/inet.h>

void error(const char *msg);
void *receive_messages(void *sockfd_ptr);
void *send_messages(void *sockfd_ptr);
void handle_client(int sockfd);
int main(int argc, char *argv[]);

#endif
