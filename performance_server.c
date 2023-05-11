/* his is a skeleton and not a complete solution. You will need to fill in the logic for each function, handle errors,
and potentially add more code to handle the different communication styles (ipv4, ipv6, mmap, pipe, uds)
and their parameters (udp, tcp, dgram, stream, filename). */

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
// You may need to include additional headers

// Function to receive data
void receive_data(int sockfd, char* type, char* param) {
    // TODO
}

// Function to verify checksum of the data
void verify_checksum(char* data, char* checksum) {
    // TODO
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "usage: %s -s port -p -q\n", argv[0]);
        exit(1);
    }

    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct addrinfo hints, *res;
    int is_performance_test = 0;
    int is_quiet_mode = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[2]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (strcmp(argv[3], "-p") == 0) {
        is_performance_test = 1;
    }

    if (strcmp(argv[4], "-q") == 0) {
        is_quiet_mode = 1;
    }

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd, 5);
    int clilen = sizeof(serv_addr);

    int newsockfd = accept(sockfd, (struct sockaddr *)&serv_addr, (socklen_t *)&clilen);
    if (newsockfd < 0)
        error("ERROR on accept");

    if (is_performance_test) {
        char* data = receive_data(newsockfd, argv[3], argv[4]);
        // In real scenario you should also receive the checksum
        // and verify it using the verify_checksum function
        // For simplicity, this part is omitted in this skeleton

        if (!is_quiet_mode) {
            printf("Received data: %s\n", data);
        }
    }

    close(newsockfd);
    close(sockfd);

    return 0;
}
