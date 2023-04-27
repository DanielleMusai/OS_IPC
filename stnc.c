#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAX_MSG_LENGTH 128

//prints an error message and exits the program in case of an error
void error(const char *msg) { 
    perror(msg);
    exit(0);
}

//handles the communication between the client and the server
//receives a socket file descriptor as a parameter and reads from the socket and writes to the socket 
//in a loop until the message "exit" is received
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

    if (strcmp(argv[1], "-s") == 0) { // checks if the first argument passed to the program is "-s"
        sockfd = socket(AF_INET, SOCK_STREAM, 0); // a socket is created using the socket() function
        //The code checks if the socket was successfully created.
        // If not, an error message is printed and the program exits.
        if (sockfd < 0)
            error("ERROR opening socket");

        bzero((char *) &serv_addr, sizeof(serv_addr)); // serv_addr struct is zeroed out using the bzero() function
        //  The port number is retrieved from the second argument passed to the program 
        //  and converted to an integer using the atoi() function
        portno = atoi(argv[2]); 
        //  The serv_addr struct is set up with the address family (AF_INET), IP address (INADDR_ANY), and port number
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(portno);

        if (bind(sockfd, (struct sockaddr *) &serv_addr,
                 sizeof(serv_addr)) < 0) // the socket is bound to the address and port using the bind() function
            error("ERROR on binding");

        listen(sockfd,5); //  the listen() function is called on the socket to start listening for incoming connections

        int clilen;
        struct sockaddr_in cli_addr;
        clilen = sizeof(cli_addr);
        // calls the accept() function to accept incoming connections. This function blocks until a client connects to the server
        int newsockfd = accept(sockfd,
                              (struct sockaddr *) &cli_addr,
                               (socklen_t *)&clilen);
                              

        if (newsockfd < 0)
            error("ERROR on accept");

        handle_client(newsockfd); //  if a client connects, the handle_client() function is called with the new socket file descriptor
        close(newsockfd); //  once the handle_client() function returns, the new socket and the original socket are closed using the close()
        close(sockfd);
    }
    else if (strcmp(argv[1], "-c") == 0) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0); // creates a new socket file descriptor for the client, using the IPv4 protocol and the TCP transport protocol
        if (sockfd < 0)
            error("ERROR opening socket");

        bzero(&hints, sizeof hints);
        // sets the address family to IPv4
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        // resolves the server's hostname and service (port) name to an address that can be used to connect to it.
        // The resulting address information is stored in the res structure
        getaddrinfo(argv[2], argv[3], &hints, &res);
        struct sockaddr_in *addr_in = (struct sockaddr_in *)res->ai_addr;
        portno = addr_in->sin_port; // extracts the port number from the address information structure
        memcpy(&serv_addr.sin_addr, &addr_in->sin_addr, sizeof(struct in_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = portno;

        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) // connects the client socket to the server socket at the specified address and port number
            error("ERROR connecting");

        handle_client(sockfd); // handles communication with the server using the newly connected socket
        close(sockfd);
    }

    return 0;
}
