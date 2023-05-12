// performance_server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <poll.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/un.h>

#define BUFFER_SIZE 4096

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

// Function to receive data ONLY tcp over IPv4
char *receive_data(int sockfd)
{
}

void receive_ipv4_tcp(int port)
{
    // TODO: Implement IPv4 TCP communication
}

void receive_ipv4_udp(int port)
{
    // TODO: Implement IPv4 UDP communication
}

void receive_ipv6_tcp(int port)
{
    // TODO: Implement IPv6 TCP communication
}

void receive_ipv6_udp(int port)
{
    // TODO: Implement IPv6 UDP communication
}

void receive_uds_dgram(char *filename)
{
    // TODO: Implement Unix Domain Socket Datagram communication
}

void receive_uds_stream(char *filename)
{
    // TODO: Implement Unix Domain Socket Stream communication
}

void receive_mmap(char *filename)
{
    // TODO: Implement memory-mapped file communication
}

void receive_pipe(char *filename)
{
    // TODO: Implement named pipe communication
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, "usage: %s -s port -p -q\n", argv[0]);
        exit(1);
    }

    int portno = atoi(argv[2]);
    char *type = argv[3];
    char *param = argv[4];

    if (strcmp(type, "ipv4") == 0 && strcmp(param, "tcp") == 0)
    {
        receive_ipv4_tcp(portno);
    }
    else if (strcmp(type, "ipv4") == 0 && strcmp(param, "udp") == 0)
    {
        receive_ipv4_udp(portno);
    }
    else if (strcmp(type, "ipv6") == 0 && strcmp(param, "tcp") == 0)
    {
        receive_ipv6_tcp(portno);
    }
    else if (strcmp(type, "ipv6") == 0 && strcmp(param, "udp") == 0)
    {
        receive_ipv6_udp(portno);
    }
    else if (strcmp(type, "uds") == 0 && strcmp(param, "dgram") == 0)
    {
        receive_uds_dgram(param);
    }
    else if (strcmp(type, "uds") == 0 && strcmp(param, "stream") == 0)
    {
        receive_uds_stream(param);
    }
    else if (strcmp(type, "mmap") == 0)
    {
        receive_mmap(param);
    }
    else if (strcmp(type, "pipe") == 0)
    {
        receive_pipe(param);
    }
    else
    {
        fprintf(stderr, "Invalid type/param combination\n");
        exit(1);
    }

    return 0;
}
