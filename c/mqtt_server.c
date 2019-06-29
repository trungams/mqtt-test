#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <MQTTAsync.h>
#include <math.h>
#include "./benchmark.h"


#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "MQTTServer"
#define TOPIC_OUT   "server/client"
#define TOPIC_IN    "client/server"
#define PAYLOAD     "hello client"
#define QOS         1
#define TIMEOUT     10000L

typedef long long int ll;
typedef unsigned long long int ull;

volatile MQTTAsync_token delivered_token;
ull messages_in = 0, messages_out = 0;
int started = 0, finished = 0;

void on_message_delivery (void *context, MQTTAsync_token dt) {
    // printf("message delivered with token %d\n", dt);
    delivered_token = dt;
    messages_out++;
    // printf("Messages sent %llu\n", messages_out);
    if (messages_out >= BENCHMARK_ITERATIONS) finished = 1;
}

void send_message (MQTTAsync client) {
    MQTTAsync_message message = MQTTAsync_message_initializer;
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;

    char str[10];
    sprintf(str, "%llu", messages_out);

    // message.payload = str;
    // message.payloadlen = messages_out ? (int)((ceil(log10(messages_out))+1)*sizeof(char)) : 2;

    message.payload = PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);
    message.qos = QOS;
    message.retained = 0;
    delivered_token = 0;

    opts.context = &message;

    // printf("%s\n", (char*)message.payload);
    MQTTAsync_sendMessage(client, TOPIC_OUT, &message, &opts);
}

int on_message_arrival (void *context, char *topic_name, int topic_length, MQTTAsync_message *message) {
    if (!strcmp(topic_name, TOPIC_IN)) {
        MQTTAsync_freeMessage(&message);
        MQTTAsync_free(topic_name);
        messages_in++;

        // if (messages_in % 1000 == 0) printf("Server has received %llu messages\n", messages_in);
        send_message((MQTTAsync)context);
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
    printf("Server has subscribed to topic\n");
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
    MQTTAsync client;
    MQTTAsync_connectOptions connect_options = MQTTAsync_connectOptions_initializer;
    int rc;

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

    while (!started)
    while (!finished);
    printf("Server has finished running.\n");
    printf("\tTotal messages sent: %llu\n", messages_out);
    printf("\tTotal messages received: %llu\n", messages_in);

    MQTTAsync_disconnect(client, NULL);
    MQTTAsync_destroy(&client);

    return 0;
}
