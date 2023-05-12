// performance_test.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/un.h>

#define BUFFER_SIZE 4096
#define DATA_SIZE 100000000

// Function to generate 100MB of data
char *generate_data()
{
    char *data = malloc(DATA_SIZE);
    memset(data, 'A', DATA_SIZE);
    return data;
}

// Function to transmit data
void transmit_data(char *data, char *type, char *param, char *ip, int port)
{
    if (strcmp(type, "ipv4") == 0 && strcmp(param, "tcp") == 0)
    {
        transmit_ipv4_tcp(data, ip, port);
    }
    else if (strcmp(type, "ipv4") == 0 && strcmp(param, "udp") == 0)
    {
        transmit_ipv4_udp(data, ip, port);
    }
    else if (strcmp(type, "ipv6") == 0 && strcmp(param, "tcp") == 0)
    {
        transmit_ipv6_tcp(data, ip, port);
    }
    else if (strcmp(type, "ipv6") == 0 && strcmp(param, "udp") == 0)
    {
        transmit_ipv6_udp(data, ip, port);
    }
    else if (strcmp(type, "uds") == 0 && strcmp(param, "dgram") == 0)
    {
        transmit_uds_dgram(data, ip);
    }
    else if (strcmp(type, "uds") == 0 && strcmp(param, "stream") == 0)
    {
        transmit_uds_stream(data, ip);
    }
    else if (strcmp(type, "mmap") == 0)
    {
        transmit_mmap(data, ip);
    }
    else if (strcmp(type, "pipe") == 0)
    {
        transmit_pipe(data, ip);
    }
    else
    {
        fprintf(stderr, "Invalid type/param combination\n");
        exit(1);
    }
}

// Function to measure time
long measure_time(struct timeval start, struct timeval end)
{
    return (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
}

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        fprintf(stderr, "usage: %s -c IP PORT -p <type> <param>\n", argv[0]);
        exit(1);
    }

    char *data = generate_data();

    struct timeval start, end;
    gettimeofday(&start, NULL);
    transmit_data(data, argv[4], argv[5], argv[2], atoi(argv[3]));
    gettimeofday(&end, NULL);

    long time_taken = measure_time(start, end);
    printf("%s_%s,%ld\n", argv[4], argv[5], time_taken);

    free(data);

    return 0;
}
