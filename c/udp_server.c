#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include "./benchmark.h"

#define PAYLOAD     "hello client\n"
#define PORT        12000
#define MAX_BUF     100

void start_chat (int sockfd) {
    int rc;
    unsigned int len;
    char buf[MAX_BUF];
    struct sockaddr_in client_addr;

    memset(&client_addr, 0, sizeof(client_addr));

    for (unsigned long long i = 0; i < BENCHMARK_ITERATIONS; i++) {
        recvfrom(sockfd, buf, sizeof(buf), MSG_WAITALL,
            (struct sockaddr*)&client_addr, &len);
        strncpy(buf, PAYLOAD, MAX_BUF);
        rc = sendto(sockfd, buf, strlen(buf), MSG_CONFIRM,
            (struct sockaddr*)&client_addr, len);
    }

    close(sockfd);
}

int main (void) {
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Socket successfully created\n");
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) {
        printf("socket bind failed...\n");
        exit(EXIT_FAILURE);
    } else {
        printf("socket successfully binded to port %d\n", PORT);
    }

    start_chat(sockfd);

    printf("All connections closed\n");

    return 0;
}
