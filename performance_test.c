/* Keep in mind that this is a high-level skeleton and not a complete solution. You will need to fill in the logic for each function,
handle errors, and potentially add more code to handle the different communication styles (ipv4, ipv6, mmap, pipe, uds)
and their parameters (udp, tcp, dgram, stream, filename). */


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
// you may need to include additional headers

// Function to generate 100MB of data
char* generate_data() {
    // TODO
}

// Function to compute a checksum of the data
char* compute_checksum(char* data) {
    // TODO
}

// Function to transmit data
void transmit_data(char* data, char* type, char* param, char* ip, int port) {
    // TODO
}

// Function to measure time
long measure_time(struct timeval start, struct timeval end) {
    // TODO
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        fprintf(stderr, "usage: %s -c IP PORT -p <type> <param>\n", argv[0]);
        exit(1);
    }

    char* data = generate_data();
    char* checksum = compute_checksum(data);

    struct timeval start, end;
    gettimeofday(&start, NULL);
    transmit_data(data, argv[4], argv[5], argv[2], atoi(argv[3]));
    gettimeofday(&end, NULL);

    long time_taken = measure_time(start, end);
    printf("%s_%s,%ld\n", argv[4], argv[5], time_taken);

    return 0;
}
