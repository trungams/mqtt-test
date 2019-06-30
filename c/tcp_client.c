#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include "./benchmark.h"

#define PAYLOAD     "hello server\n"
#define PORT        12000
#define MAX_BUF     100

int main (void) {
    int sockfd, val;
    char buf[MAX_BUF];
    struct sockaddr_in server_addr;

    struct timespec *begin, *end, *result;
    begin   = Benchmark_timespec_init;
    end     = Benchmark_timespec_init;
    result  = Benchmark_timespec_init;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Socket successfully created\n");
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
        printf("connection with the server failed\n");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val))) {
        printf("Cannot set TCP_NODELAY option for client socket\n");
        exit(EXIT_FAILURE);
    }

    clock_gettime(CLOCK_REALTIME, begin);
    for (unsigned long long i = 0; i < BENCHMARK_ITERATIONS; i++) {
        char *msg = PAYLOAD;
        send(sockfd, msg, strlen(msg), MSG_CONFIRM);
        recv(sockfd, buf, sizeof(buf), 0);
    }
    clock_gettime(CLOCK_REALTIME, end);

    timespec_difference(result, begin, end);
    print_result(result);

    close(sockfd);
    return 0;
}
