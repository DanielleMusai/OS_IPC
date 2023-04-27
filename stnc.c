#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAX_MSG_LENGTH 256

void error(const char *msg) {
    perror(msg);
    exit(0);
}

void handle_client(int sockfd) {
    char buffer[MAX_MSG_LENGTH];
    int n;

    while (1) {
        bzero(buffer, MAX_MSG_LENGTH);
        fgets(buffer, MAX_MSG_LENGTH, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0)
            error("ERROR writing to socket");
        if (strcmp(buffer, "exit\n") == 0)
            break;
        bzero(buffer, MAX_MSG_LENGTH);
        n = read(sockfd, buffer, MAX_MSG_LENGTH-1);
        if (n < 0)
            error("ERROR reading from socket");
        printf("%s", buffer);
        if (strcmp(buffer, "exit\n") == 0)
            break;
    }
}

int main(int argc, char *argv[]) {
    int sockfd, portno;
    struct sockaddr_in serv_addr;
  //  struct hostent *server;
    struct addrinfo hints, *res;

    if (argc < 3) {
        fprintf(stderr,"usage: %s -s PORT or %s -c IP PORT\n", argv[0], argv[0]);
        exit(1);
    }

    if (strcmp(argv[1], "-s") == 0) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            error("ERROR opening socket");

        bzero((char *) &serv_addr, sizeof(serv_addr));
        portno = atoi(argv[2]);
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(portno);

        if (bind(sockfd, (struct sockaddr *) &serv_addr,
                 sizeof(serv_addr)) < 0)
            error("ERROR on binding");

        listen(sockfd,5);

        int clilen;
        struct sockaddr_in cli_addr;
        clilen = sizeof(cli_addr);
        int newsockfd = accept(sockfd,
                              (struct sockaddr *) &cli_addr,
                               (socklen_t *)&clilen);
                              
                              
        if (newsockfd < 0)
            error("ERROR on accept");

        handle_client(newsockfd);
        close(newsockfd);
        close(sockfd);
    }
    else if (strcmp(argv[1], "-c") == 0) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            error("ERROR opening socket");

        bzero(&hints, sizeof hints);
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        getaddrinfo(argv[2], argv[3], &hints, &res);
        struct sockaddr_in *addr_in = (struct sockaddr_in *)res->ai_addr;
        portno = addr_in->sin_port;
        memcpy(&serv_addr.sin_addr, &addr_in->sin_addr, sizeof(struct in_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = portno;

        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
            error("ERROR connecting");

        handle_client(sockfd);
        close(sockfd);
    }

    return 0;
}
