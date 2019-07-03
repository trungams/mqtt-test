#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include "./benchmark.h"

#define PAYLOAD     "hello client\n"
#define PORT        12000
#define MAX_BUF     100

void start_chat (int sockfd) {
    int val;
    char buf[MAX_BUF];

    if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val))) {
        printf("Cannot set TCP_NODELAY option for client socket\n");
        exit(EXIT_FAILURE);
    }

    for (unsigned long long i = 0; i < BENCHMARK_ITERATIONS; i++) {
        recv(sockfd, buf, sizeof(buf), 0);
        strncpy(buf, PAYLOAD, MAX_BUF);
        send(sockfd, buf, strlen(buf), MSG_CONFIRM);
    }

    sleep(1);
    close(sockfd);
}

int main (void) {
    int sockfd, connfd;
    unsigned int len;
    struct sockaddr_in server_addr, client_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Socket successfully created\n");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
        printf("socket bind failed...\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Socket successfully binded to port %d\n", PORT);
    }

    // there is only one client, so leaving the queue small is fine
    if ((listen(sockfd, 10)) != 0) {
        printf("Listen failed..\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Server is listening...\n");
    }

    connfd = accept(sockfd, (struct sockaddr*)&client_addr, &len);
    if (connfd < 0) {
        printf("Server accept failed..\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Server has accepted a client's connection\n");
    }

    start_chat(connfd);

    close(sockfd);

    printf("All connections closed\n");

    return 0;
}
