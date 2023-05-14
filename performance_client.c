// performance_client.c

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
    memset(data, '0', DATA_SIZE);
    return data;
}

void transmit_ipv4_tcp(char *data, char *ip, int port)
{
    // Implementation code for transmitting data over IPv4 TCP
    printf("Transmitting data over IPv4 TCP\n");
    printf("IP: %s\n", ip);
    printf("Port: %d\n", port);

    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    send(sockfd, data, DATA_SIZE, 0);

    close(sockfd);
    ;
}
void transmit_ipv4_udp(char *data, char *ip, int port)
{
    // Implementation code for transmitting data over IPv4 UDP
    printf("Transmitting data over IPv4 UDP\n");
    printf("IP: %s\n", ip);
    printf("Port: %d\n", port);
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    sendto(sockfd, data, DATA_SIZE, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    close(sockfd);
}

void transmit_ipv6_tcp(char *data, char *ip, int port)
{
    // Implementation code for transmitting data over IPv6 TCP
    printf("Transmitting data over IPv6 TCP\n");
    printf("IP: %s\n", ip);
    printf("Port: %d\n", port);
    
    int sockfd;
    struct sockaddr_in6 server_addr;

    sockfd = socket(AF_INET6, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(port);

    inet_pton(AF_INET6, ip, &server_addr.sin6_addr);

    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    send(sockfd, data, DATA_SIZE, 0);

    close(sockfd);
}
void transmit_ipv6_udp(char *data, char *ip, int port)
{
    // Implementation code for transmitting data over IPv6 UDP
    printf("Transmitting data over IPv6 UDP\n");
    printf("IP: %s\n", ip);
    printf("Port: %d\n", port);
    
    int sockfd;
    struct sockaddr_in6 server_addr;

    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(port);

    inet_pton(AF_INET6, ip, &server_addr.sin6_addr);

    sendto(sockfd, data, DATA_SIZE, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    close(sockfd);
}
void transmit_uds_dgram(char *data, char *socket_path)
{
    // Implementation code for transmitting data over UDS datagram
    printf("Transmitting data over UDS datagram\n");
    printf("Socket Path: %s\n", socket_path);
    int sockfd;
    struct sockaddr_un server_addr;

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, socket_path, sizeof(server_addr.sun_path) - 1);

    sendto(sockfd, data, DATA_SIZE, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    close(sockfd);
}

void transmit_uds_stream(char *data, char *socket_path)
{
    // Implementation code for transmitting data over UDS stream
    printf("Transmitting data over UDS stream\n");
    printf("Socket Path: %s\n", socket_path);
    int sockfd;
    struct sockaddr_un server_addr;

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, socket_path, sizeof(server_addr.sun_path) - 1);

    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    send(sockfd, data, DATA_SIZE, 0);

    close(sockfd);
}

void transmit_mmap(char *data, char *file_path)
{
    // Implementation code for transmitting data using memory-mapped files
    printf("Transmitting data using memory-mapped files\n");
    printf("File Path: %s\n", file_path);
    int fd;
    char *addr;

    fd = open(file_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    ftruncate(fd, DATA_SIZE);

    addr = mmap(NULL, DATA_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);

    memcpy(addr, data, DATA_SIZE);

    munmap(addr, DATA_SIZE);
    close(fd);
}
void transmit_pipe(char *data, char *pipe_name)
{
    // Implementation code for transmitting data using pipes
    printf("Transmitting data using pipes\n");
    printf("Pipe Name: %s\n", pipe_name);
    int fd;

    mkfifo(pipe_name, 0666);

    fd = open(pipe_name, O_WRONLY);

    write(fd, data, DATA_SIZE);

    close(fd);
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
        fprintf(stderr, "usage: %s -c 1 IP 2 PORT 3 -p <type> 5 <param> 6\n", argv[0]);
        exit(1);
    }

    char *data = generate_data();

    struct timeval start, end;
    gettimeofday(&start, NULL);

    transmit_data(data, argv[5], argv[6], argv[2], atoi(argv[3]));

    gettimeofday(&end, NULL);

    long time_taken = measure_time(start, end);
    printf("%s_%s,%ld\n", argv[4], argv[5], time_taken);

    free(data);

    return 0;
}
