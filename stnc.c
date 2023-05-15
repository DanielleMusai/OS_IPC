// stnc.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <poll.h>
#include <arpa/inet.h>
#include "stnc.h"

#define MAX_LENGTH 128
#define BUFFER_SIZE 4096

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

/*
handle_client function is designed to handle communication between a client and a server over a network connection.
The function takes a socket file descriptor (sockfd) as input,
and uses it to send and receive messages to/from the other end of the connection.
*/

void handle_client(int sockfd)
{
    struct pollfd fds[2];
    int ret;
    char buffer[MAX_LENGTH];

    // We want to listen to this socket
    fds[0].fd = sockfd;
    fds[0].events = POLLIN;

    // We also want to listen to user input
    fds[1].fd = STDIN_FILENO;
    fds[1].events = POLLIN;

    while (1)
    {
        ret = poll(fds, 2, -1); // Wait indefinitely for some activity
        if (ret == -1)
        {
            perror("poll");
            exit(EXIT_FAILURE);
        }

        if (fds[0].revents & POLLIN)
        {
            // Handle incoming data, similar to your receive_messages() function
            bzero(buffer, MAX_LENGTH);
            int n = read(sockfd, buffer, MAX_LENGTH - 1);
            if (n > 0)
            {
                printf("%s", buffer);
            }
            else if (n < 0)
            {
                error("ERROR reading from socket");
            }

            if (strcmp(buffer, "exit\n") == 0)
            {
                break;
            }
        }

        if (fds[1].revents & POLLIN)
        {
            // Handle user input, similar to your send_messages() function
            bzero(buffer, MAX_LENGTH);
            fgets(buffer, MAX_LENGTH, stdin);
            int n = write(sockfd, buffer, strlen(buffer));
            if (n < 0)
            {
                error("ERROR writing to socket");
            }

            if (strcmp(buffer, "exit\n") == 0)
            {
                break;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct addrinfo hints, *res;
    int is_server;
    int perf_mode = 0;
    char *type = NULL;
    char *param = NULL;

    if (argc < 3)
    {
        fprintf(stderr, "usage: %s -s PORT or %s -c IP PORT\n", argv[0], argv[0]);
        exit(1);
    }

    is_server = strcmp(argv[1], "-s") == 0;

    perf_mode = is_server ? (strcmp(argv[3], "-p") == 0) : (strcmp(argv[4], "-p") == 0);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[is_server ? 2 : 3]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);

    if (perf_mode) // Performance mode
    {
        // Server side
        if (is_server)
        {
            // Bind to the socket
            if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
                error("ERROR on binding");

            // Listen for incoming connections
            listen(sockfd, 5);
            struct sockaddr_in cli_addr;
            socklen_t clilen = sizeof(cli_addr);

            // Accept a connection
            int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
            if (newsockfd < 0)
                error("ERROR on accept");

            // Read the 'type' and 'param' parameters from the client
            char typeBuffer[BUFFER_SIZE];
            char paramBuffer[BUFFER_SIZE];
            bzero(typeBuffer, BUFFER_SIZE);
            bzero(paramBuffer, BUFFER_SIZE);

            read(newsockfd, typeBuffer, BUFFER_SIZE - 1);
            read(newsockfd, paramBuffer, BUFFER_SIZE - 1);

            // Allocate memory and copy the strings from the buffers
            type = malloc(strlen(typeBuffer) + 1);
            param = malloc(strlen(paramBuffer) + 1);
            strcpy(type, typeBuffer);
            strcpy(param, paramBuffer);

            printf("Received type: %s\n", type);
            printf("Received param: %s\n", param);

            close(newsockfd);
            close(sockfd);
        }
        else // Client side
        {
            type = argv[5];
            param = argv[6];

            // Set up the server address structure
            if (inet_pton(AF_INET, argv[2], &serv_addr.sin_addr) <= 0)
                error("ERROR on inet_pton");

            // Connect to the server
            if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
                error("ERROR connecting");
            printf("Sent type: %s\n", type);
            printf("Sent param: %s\n", param);

            // Send the 'type' and 'param' parameters to the server
            int ret = write(sockfd, type, strlen(type) + 1);
            if (ret == -1)
                perror("write type error");
            sleep(1);
            ret = write(sockfd, param, strlen(param) + 1);
            if (ret == -1)
                perror("write param error");
            close(sockfd);
        }
    }
    else // Chat mode
    {
        if (is_server)
        {
            serv_addr.sin_addr.s_addr = INADDR_ANY;

            if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
                error("ERROR on binding");

            listen(sockfd, 5);
            socklen_t clilen = sizeof(serv_addr);

            int newsockfd = accept(sockfd, (struct sockaddr *)&serv_addr, (socklen_t *)&clilen);
            if (newsockfd < 0)
                error("ERROR on accept");

            handle_client(newsockfd);
            close(newsockfd);
            close(sockfd);
        }
        else
        {
            bzero(&hints, sizeof hints);
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            getaddrinfo(argv[2], argv[3], &hints, &res);
            struct sockaddr_in *addr_in = (struct sockaddr_in *)res->ai_addr;
            memcpy(&serv_addr.sin_addr, &addr_in->sin_addr, sizeof(struct in_addr));

            if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
                error("ERROR connecting");

            handle_client(sockfd);
            freeaddrinfo(res);
            close(sockfd);
        }
    }

    if (perf_mode)
    {
        int exec_argc;
        if (is_server)
        {
            for (int i = 0; i < argc; i++)
            {
                printf("argv[%d]: %s\n", i, argv[i]);
            }
            if (type == NULL || param == NULL || argc < 3 || argc > 7)
            {
                fprintf(stderr, "usage: %s -s =1 port =2 -p=3 -q=4 type =5 param =6\n", argv[0]);
                exit(1);
            }
            int quite = (argc > 3 && strcmp(argv[4], "-q") == 0) ? 1 : 0;
            if (quite)
                printf("Server is running in quiet mode\n");
            char *exec_argv[argc + 2 + quite];

            exec_argc = argc + 2 + quite;
            exec_argv[0] = "performance_server"; // assuming "performance_test" is the executable name

            if (quite)
            {
                exec_argv[4] = "-q";
                exec_argv[5] = type;
                exec_argv[6] = param;
            }
            else
            {
                exec_argv[4] = type;
                exec_argv[5] = param;
            }
            printf("%d\n", exec_argc);
            exec_argv[exec_argc] = NULL;                                                 // the argument list must be terminated by a NULL pointer
            execv("/home/agassi/OperatingSystems/OS_IPC/performance_server", exec_argv); // replace "path_to_performance_test" with the actual path
            perror("execv");                                                             // execv returns only if there is an error
            return 1;
        }
        else // is client
        {
            if (argc != 7)
            {
                fprintf(stderr, "usage: %s -s =1 port =2 ip=3 -p=4 type =5 param =6\n", argv[0]);
                exit(1);
            }
            argv[0] = "performance_client"; 
            execv("/home/agassi/OperatingSystems/OS_IPC/performance_client", argv);
            perror("execv"); // execv returns only if there is an error
            return 1;
        }
        free(type);
        free(param);
    }

    return 0;
}

