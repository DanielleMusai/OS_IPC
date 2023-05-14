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

#define MAX_LENGTH 128

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

    if (argc < 3)
    {
        fprintf(stderr, "usage: %s -s PORT or %s -c IP PORT\n", argv[0], argv[0]);
        exit(1);
    }

    is_server = strcmp(argv[1], "-s") == 0;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-p") == 0)
        {
            perf_mode = 1;
            break;
        }
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket < 0)
        error("ERROR opening socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[is_server ? 2 : 3]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);

    if (is_server)
    {
        serv_addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
            error("ERROR on binding");

        listen(sockfd, 5);
        int clilen = sizeof(serv_addr);

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
        close(sockfd);
    }

    return 0;
}
/* 
// If "-p" is present in the command-line arguments
    if (p_index != -1) {
        if (is_server){
            char* exec_argv[argc - p_index + 1];
            exec_argv[0] = "performance_client";  // assuming "performance_test" is the executable name
            for (int i = p_index; i < argc; i++) {
                exec_argv[i - p_index + 1] = argv[i];
            }
            exec_argv[argc - p_index + 1] = NULL;  // the argument list must be terminated by a NULL pointer

            execv("path_to_performance_test", exec_argv);  // replace "path_to_performance_test" with the actual path
            perror("execv");  // execv returns only if there is an error
            return 1;
        }
    } */

/* 
mission:
1. rewrite performance_client.c according performence_server.c - Danny
2. put the execution of both client and server in stnc main() - Elhai
3. implement sendinding to the server TYPE and PARMETER - Danny
4. add -q to the server to quit the server - Elhai

*/