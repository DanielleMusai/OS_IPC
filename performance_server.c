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
    char buffer[BUFFER_SIZE];
    ssize_t bytesReceived;
    size_t totalBytesReceived = 0;
    size_t bufferSize = BUFFER_SIZE;
    char *data = NULL;

    // Receive data in chunks until the end
    while ((bytesReceived = recv(sockfd, buffer, bufferSize, 0)) > 0)
    {
        // Resize the data buffer to accommodate the new data
        data = realloc(data, totalBytesReceived + bytesReceived);
        if (data == NULL)
        {
            error("Memory allocation failed");
        }

        // Copy the received data into the buffer
        memcpy(data + totalBytesReceived, buffer, bytesReceived);
        totalBytesReceived += bytesReceived;
    }

    // Check for receive errors
    if (bytesReceived < 0)
    {
        error("Error receiving data from socket");
    }

    // Null-terminate the data buffer
    data = realloc(data, totalBytesReceived + 1);
    if (data == NULL)
    {
        error("Memory allocation failed");
    }
    data[totalBytesReceived] = '\0';

    return data;
}

void receive_ipv4_tcp(int port, int quite)
{
    int sockfd, newsockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientLen;
    char *receivedData;

    // Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Error opening socket");
    }

    // Set up the server address structure
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        error("Error on binding");
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0)
    {
        error("Error on listening");
    }
    
    printf("Server listening on port %d\n", port);

    while (1)
    {
        clientLen = sizeof(clientAddr);

        // Accept a client connection
        newsockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientLen);
        if (newsockfd < 0)
        {
            error("Error on accepting connection");
        }
        
        printf("Client connected\n");

        // Receive data from the client
        receivedData = receive_data(newsockfd);

        if(!quite)
            printf("Received data:\n%s\n", receivedData);

        // Free the received data memory
        free(receivedData);

        // Close the client socket
        close(newsockfd);
    }

    // Close the server socket
    close(sockfd);
}

void receive_ipv4_udp(int port, int quite)
{
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientLen;
    char buffer[BUFFER_SIZE];
    ssize_t bytesReceived;

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        error("Error opening socket");
    }

    // Set up the server address structure
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        error("Error on binding");
    }

    if(!quite)
        printf("Server listening on port %d\n", port);

    while (1)
    {
        clientLen = sizeof(clientAddr);

        // Receive data from a client
        bytesReceived = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&clientAddr, &clientLen);
        if (bytesReceived < 0)
        {
            error("Error receiving data");
        }

        // Null-terminate the received data
        buffer[bytesReceived] = '\0';

        if(!quite)
            printf("Received data from %s:%d:\n%s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buffer);
    }

    // Close the socket
    close(sockfd);
}

void receive_ipv6_tcp(int port, int quite)
{
    int sockfd, newsockfd;
    struct sockaddr_in6 serverAddr, clientAddr;
    socklen_t clientLen;
    char *receivedData;

    // Create a TCP socket
    sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Error opening socket");
    }

    // Set up the server address structure
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin6_family = AF_INET6;
    serverAddr.sin6_port = htons(port);
    serverAddr.sin6_addr = in6addr_any;

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        error("Error on binding");
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0)
    {
        error("Error on listening");
    }

    printf("Server listening on port %d\n", port);

    while (1)
    {
        clientLen = sizeof(clientAddr);

        // Accept a client connection
        newsockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientLen);
        if (newsockfd < 0)
        {
            error("Error on accepting connection");
        }

        printf("Client connected\n");

        // Receive data from the client
        receivedData = receive_data(newsockfd);

        if(!quite)
            printf("Received data:\n%s\n", receivedData);

        // Free the received data memory
        free(receivedData);

        // Close the client socket
        close(newsockfd);
    }

    // Close the server socket
    close(sockfd);
}

void receive_ipv6_udp(int port, int quite)
{
    int sockfd;
    struct sockaddr_in6 serverAddr, clientAddr;
    socklen_t clientLen;
    char buffer[BUFFER_SIZE];
    ssize_t bytesReceived;

    // Create a UDP socket
    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        error("Error opening socket");
    }

    // Set up the server address structure
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin6_family = AF_INET6;
    serverAddr.sin6_port = htons(port);
    serverAddr.sin6_addr = in6addr_any;

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        error("Error on binding");
    }

    printf("Server listening on port %d\n", port);

    while (1)
    {
        clientLen = sizeof(clientAddr);

        // Receive data from a client
        bytesReceived = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&clientAddr, &clientLen);
        if (bytesReceived < 0)
        {
            error("Error receiving data");
        }

        // Null-terminate the received data
        buffer[bytesReceived] = '\0';

        char clientAddrStr[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &(clientAddr.sin6_addr), clientAddrStr, INET6_ADDRSTRLEN);
        if(!quite)
            printf("Received data from [%s]:%d:\n%s\n", clientAddrStr, ntohs(clientAddr.sin6_port), buffer);
    }

    // Close the socket
    close(sockfd);
}

void receive_uds_dgram(char *filename, int quite)
{
    int sockfd;
    struct sockaddr_un serverAddr, clientAddr;
    socklen_t clientLen;
    char buffer[BUFFER_SIZE];
    ssize_t bytesReceived;

    // Create a UDP socket
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        error("Error opening socket");
    }

    // Set up the server address structure
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    strncpy(serverAddr.sun_path, filename, sizeof(serverAddr.sun_path) - 1);

    // Remove the existing socket file if it exists
    unlink(filename);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        error("Error on binding");
    }

    printf("Server listening on Unix Domain Socket Datagram: %s\n", filename);

    while (1)
    {
        clientLen = sizeof(clientAddr);

        // Receive data from a client
        bytesReceived = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&clientAddr, &clientLen);
        if (bytesReceived < 0)
        {
            error("Error receiving data");
        }

        // Null-terminate the received data
        buffer[bytesReceived] = '\0';
    if(!quite)    
        printf("Received data:\n%s\n", buffer);
    }

    // Close the socket
    close(sockfd);

    // Remove the socket file
    unlink(filename);
}

void receive_uds_stream(char *filename, int quite)
{
    int sockfd, newsockfd;
    struct sockaddr_un serverAddr, clientAddr;
    socklen_t clientLen;
    char *receivedData;

    // Create a stream socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Error opening socket");
    }

    // Set up the server address structure
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    strncpy(serverAddr.sun_path, filename, sizeof(serverAddr.sun_path) - 1);

    // Remove the existing socket file if it exists
    unlink(filename);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        error("Error on binding");
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0)
    {
        error("Error on listening");
    }
    if(!quite)
        printf("Server listening on Unix Domain Socket Stream: %s\n", filename);

    while (1)
    {
        clientLen = sizeof(clientAddr);

        // Accept a client connection
        newsockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientLen);
        if (newsockfd < 0)
        {
            error("Error on accepting connection");
        }

        printf("Client connected\n");

        // Receive data from the client
        receivedData = receive_data(newsockfd);
        if(!quite)
            printf("Received data:\n%s\n", receivedData);

        // Free the received data memory
        free(receivedData);

        // Close the client socket
        close(newsockfd);
    }

    // Close the server socket
    close(sockfd);

    // Remove the socket file
    unlink(filename);
}

void receive_mmap(char *filename, int quite)
{
    int fd;
    struct stat fileStat;
    char *mappedData;

    // Open the file for reading
    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        error("Error opening file");
    }

    // Get the file size
    if (fstat(fd, &fileStat) < 0)
    {
        error("Error getting file information");
    }
    off_t fileSize = fileStat.st_size;

    // Map the file into memory
    mappedData = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mappedData == MAP_FAILED)
    {
        close(fd);
        error("Error mapping file into memory");
    }

    // Print the contents of the memory-mapped file
    if(!quite)
        printf("Received data from memory-mapped file:\n%s\n", mappedData);

    // Unmap the file from memory
    if (munmap(mappedData, fileSize) < 0)
    {
        error("Error unmapping file from memory");
    }

    // Close the file
    close(fd);
}

void receive_pipe(char *filename, int quite)
{
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    // Open the named pipe for reading
    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        error("Error opening named pipe");
    }

    printf("Server listening on named pipe: %s\n", filename);

    while (1)
    {
        // Read data from the named pipe
        bytesRead = read(fd, buffer, BUFFER_SIZE - 1);
        if (bytesRead < 0)
        {
            error("Error reading from named pipe");
        }

        // Null-terminate the received data
        buffer[bytesRead] = '\0';
        if(!quite)
            printf("Received data:\n%s\n", buffer);
    }

    // Close the named pipe
    close(fd);
}

int main(int argc, char *argv[])
{
    printf("Server started\n");
    int portno = atoi(argv[2]);
    char *type;
    char *param;
    int quite = ((strcmp(argv[4], "-q") == 0) ? 1 : 0);
    if (quite == 1)
    {
        printf("quite mode\n");
        type = argv[5];
        param = argv[6];
    }
    else
    {
        type = argv[4];
        param = argv[5];
    }

    printf("in pref server\n");
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    if (strcmp(type, "ipv4") == 0 && strcmp(param, "tcp") == 0)
    {
        receive_ipv4_tcp(portno, quite);
    }
    else if (strcmp(type, "ipv4") == 0 && strcmp(param, "udp") == 0)
    {
        receive_ipv4_udp(portno, quite);
    }
    else if (strcmp(type, "ipv6") == 0 && strcmp(param, "tcp") == 0)
    {
        receive_ipv6_tcp(portno, quite);
    }
    else if (strcmp(type, "ipv6") == 0 && strcmp(param, "udp") == 0)
    {
        receive_ipv6_udp(portno, quite);
    }
    else if (strcmp(type, "uds") == 0 && strcmp(param, "dgram") == 0)
    {
        receive_uds_dgram(param, quite);
    }
    else if (strcmp(type, "uds") == 0 && strcmp(param, "stream") == 0)
    {
        receive_uds_stream(param, quite);
    }
    else if (strcmp(type, "mmap") == 0)
    {
        receive_mmap(param, quite);
    }
    else if (strcmp(type, "pipe") == 0)
    {
        receive_pipe(param, quite);
    }
    else
    {
        fprintf(stderr, "Invalid type/param combination\n");
        exit(1);
    }

    return 0;
}
