#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <MQTTAsync.h>
#include "./benchmark.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "MQTTClient"
#define TOPIC_OUT   "client/server"
#define TOPIC_IN    "server/client"
#define PAYLOAD     "hello server"
#define QOS         1
#define TIMEOUT     10000L

typedef long long int ll;
typedef unsigned long long int ull;

volatile MQTTAsync_token delivered_token;
ull messages_in = 0, messages_out = 0;
int started = 0, finished = 0;

void on_message_delivery (void *context, MQTTAsync_token dt) {
    delivered_token = dt;
    messages_out++;
}

int on_message_arrival (void *context, char *topic_name, int topic_length, MQTTAsync_message *message) {
    if (!strcmp(topic_name, TOPIC_IN)) {
        MQTTAsync_freeMessage(&message);
        MQTTAsync_free(topic_name);
        messages_in++;
        // if (messages_in % 1000 == 0) printf("Client has received %llu messages\n", messages_in);
        if (messages_in >= BENCHMARK_ITERATIONS) finished = 1;
    }
    return 1;
}

void on_connection_lost (void *context, char *cause) {
    printf("Connection lost\n");
    printf("\tcause: %s\n", cause);
    exit(EXIT_FAILURE);
}

void on_subscribe (void *context, MQTTAsync_successData *response) {
    started = 1;
    printf("Client has subscribed to topic\n");
}

void on_connect (void *context, MQTTAsync_successData *response) {
    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    int rc;

    printf("Successful connection\n");

    opts.onSuccess = on_subscribe;
    opts.context = client;

    if ((rc = MQTTAsync_subscribe(client, TOPIC_IN, QOS, &opts)) != MQTTASYNC_SUCCESS) {
        printf("Failed to subscribe, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
}

int main (void) {
    // mqtt structs;
    MQTTAsync client;
    MQTTAsync_connectOptions connect_options = MQTTAsync_connectOptions_initializer;
    MQTTAsync_message message = MQTTAsync_message_initializer;
    int rc;

    // timespec structs for benchmarking
    struct timespec *begin, *end, *result;
    begin       = Benchmark_timespec_init;
    end         = Benchmark_timespec_init;
    result      = Benchmark_timespec_init;

    MQTTAsync_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);

    MQTTAsync_setCallbacks(client, client, on_connection_lost,
        on_message_arrival, on_message_delivery);

    connect_options.keepAliveInterval = 20;
    connect_options.cleansession = 1;
    connect_options.onSuccess = on_connect;
    connect_options.context = client;

    if ((rc = MQTTAsync_connect(client, &connect_options)) != MQTTASYNC_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    while (!started);

    clock_gettime(CLOCK_REALTIME, begin);
    while (messages_in < BENCHMARK_ITERATIONS) {
        MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;

        message.payload = PAYLOAD;
        message.payloadlen = strlen(PAYLOAD);
        message.qos = QOS;
        message.retained = 0;
        delivered_token = 0;

        opts.context = &message;

        MQTTAsync_sendMessage(client, TOPIC_OUT, &message, &opts);
        while (delivered_token == 0);
    }
    while (!finished);

    clock_gettime(CLOCK_REALTIME, end);
    timespec_difference(result, begin, end);
    print_result(result);

    printf("Client has finished running.\n");
    printf("\tTotal messages sent: %llu\n", messages_out);
    printf("\tTotal messages received: %llu\n", messages_in);

    MQTTAsync_disconnect(client, NULL);
    MQTTAsync_destroy(&client);

    return 0;
}
