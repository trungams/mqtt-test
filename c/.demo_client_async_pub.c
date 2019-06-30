/*
 * https://www.eclipse.org/paho/files/mqttdoc/MQTTClient/html/pubasync.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <MQTTClient.h>
#include "./benchmark.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC       "test/example"
#define PAYLOAD     "Hello, world!"
#define QOS         1
#define TIMEOUT     10000L

volatile MQTTClient_deliveryToken delivered_token;
unsigned long long message_count = 0;
int finished = 0;

void on_message_delivery (void *context, MQTTClient_deliveryToken dt) {
    delivered_token = dt;
    message_count++;
    if (message_count == BENCHMARK_ITERATIONS) finished = 1;
}

int on_message_arrival (void *context, char *topic_name, int topic_length, MQTTClient_message *message) {
    return 1;
}

void connection_lost (void *context, char *cause) {
    printf("Connection lost\n");
    printf("\tcause: %s\n", cause);
}

int main (void) {
    // mqtt structs
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pub_msg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    // timespec structs for benchmarking
    struct timespec *begin, *end, *result;
    begin   = Benchmark_timespec_init;
    end     = Benchmark_timespec_init;
    result  = Benchmark_timespec_init;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
            MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    MQTTClient_setCallbacks(client, NULL, connection_lost, on_message_arrival, on_message_delivery);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    clock_gettime(CLOCK_REALTIME, begin);
    while (message_count < BENCHMARK_ITERATIONS) {
        pub_msg.payload = PAYLOAD;
        pub_msg.payloadlen = strlen(PAYLOAD);
        pub_msg.qos = QOS;
        pub_msg.retained = 0;
        delivered_token = 0;

        MQTTClient_publishMessage(client, TOPIC, &pub_msg, &token);
        while (delivered_token != token);
    }
    while (!finished);

    clock_gettime(CLOCK_REALTIME, end);
    timespec_difference(result, begin, end);

    print_result(result);

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
