#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include "./benchmark.h"

#define PAYLOAD     "hello server\n"
#define PORT        12000
#define MAX_BUF     100

int main (void) {
    int sockfd;
    unsigned int len;
    char buf[MAX_BUF];
    struct sockaddr_in server_addr;

    struct timespec *begin, *end, *result;
    begin   = Benchmark_timespec_init;
    end     = Benchmark_timespec_init;
    result  = Benchmark_timespec_init;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("socket creation failed...\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Socket successfully created\n");
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    clock_gettime(CLOCK_REALTIME, begin);
    for (unsigned long long i = 0; i < BENCHMARK_ITERATIONS; i++) {
        char *msg = PAYLOAD;
        sendto(sockfd, msg, strlen(msg), MSG_CONFIRM,
                (struct sockaddr*)&server_addr, sizeof(server_addr));
        recvfrom(sockfd, buf, sizeof(buf), MSG_WAITALL,
                (struct sockaddr*)&server_addr, &len);
    }
    clock_gettime(CLOCK_REALTIME, end);

    timespec_difference(result, begin, end);
    print_result(result);

    close(sockfd);
    return 0;
}
