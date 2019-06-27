/*
 * https://www.eclipse.org/paho/files/mqttdoc/MQTTAsync/html/publish.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <MQTTAsync.h>
#include "./benchmark.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC       "test/example"
#define PAYLOAD     "Hello, world!"
#define QOS         1
#define TIMEOUT     10000L

volatile MQTTAsync_token delivered_token;
unsigned long long message_count = 0;
int started = 0;
int finished = 0;

void delivered (void *context, MQTTAsync_token dt) {
    delivered_token = dt;
    message_count++;
    if (message_count == BENCHMARK_ITERATIONS) finished = 1;
}

void message_arrived (void *context, MQTTAsync_successData *response) {
    MQTTAsync_message *message = (MQTTAsync_message*)context;
    MQTTAsync_freeMessage(&message);
}

void connection_lost (void *context, char *cause) {
    printf("Connection lost\n");
    printf("\tcause: %s\n", cause);
}

void on_connect (void *context, MQTTAsync_successData *response) {
    started = 1;
}

int main (void) {
    // mqtt structs
    MQTTAsync client;
    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    MQTTAsync_message pub_msg = MQTTAsync_message_initializer;
    int rc;

    // timespec structs for benchmarking
    struct timespec *begin, *end, *result;
    begin   = Benchmark_timespec_init;
    end     = Benchmark_timespec_init;
    result  = Benchmark_timespec_init;

    MQTTAsync_create(&client, ADDRESS, CLIENTID,
            MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.context = client;
    conn_opts.onSuccess = on_connect;

    MQTTAsync_setCallbacks(client, NULL, connection_lost, NULL, NULL);
    MQTTAsync_setDeliveryCompleteCallback(client, NULL, delivered);

    if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS) {
        printf("Failed to start connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    while (!started);

    clock_gettime(CLOCK_REALTIME, begin);
    while (message_count < BENCHMARK_ITERATIONS) {
        MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;

        pub_msg.payload = PAYLOAD;
        pub_msg.payloadlen = strlen(PAYLOAD);
        pub_msg.qos = QOS;
        pub_msg.retained = 0;
        delivered_token = 0;

        opts.onSuccess = message_arrived;
        opts.context = &pub_msg;

        MQTTAsync_sendMessage(client, TOPIC, &pub_msg, &opts);
        while (delivered_token == 0);
    }
    while (!finished);

    clock_gettime(CLOCK_REALTIME, end);
    timespec_difference(result, begin, end);

    print_result(result);

    MQTTAsync_disconnect(client, NULL);
    MQTTAsync_destroy(&client);
    return rc;
}
