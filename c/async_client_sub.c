/*
 * https://www.eclipse.org/paho/files/mqttdoc/MQTTAsync/html/subscribe.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <MQTTAsync.h>
#include "./benchmark.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "test/example"
#define PAYLOAD     "Hello, world!"
#define QOS         1
#define TIMEOUT     10000L

volatile MQTTAsync_token delivered_token;
unsigned long long message_count = 0;

int disc_finished = 0;
int subscribed = 0;
int finished = 0;

void connection_lost (void *context, char *cause) {
    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    int rc;

    printf("\nConnection lost\n");
    printf("\tcause: %s\n", cause);

    printf("Reconnecting\n");
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS) {
        printf("Failed to start connect, return code %d\n", rc);
        finished = 1;
    }
}

int message_arrived (void *context, char *topic_name, int topic_len, MQTTAsync_message *message) {
    // char *payloadptr;
    //
    // printf("Message arrived\n");
    // printf("\ttopic: %s\n", topic_name);
    // printf("\tmessage: ");
    //
    // payloadptr = message->payload;
    // for (int i = 0; i < message->payloadlen; i++) {
    //     putchar(*payloadptr++);
    // }
    // putchar('\n');

    message_count++;
    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topic_name);
    return 1;
}

void on_disconnect (void *context, MQTTAsync_successData *response) {
    printf("Successful disconnection\n");
    disc_finished = 1;
}

void on_subscribe (void *context, MQTTAsync_successData *response) {
    printf("Subscribed successully\n");
    subscribed = 1;
}

void on_subscribe_failure (void *context, MQTTAsync_failureData *response) {
    printf("Subscribe failed, rc %d\n", response ? response->code : 0);
    finished = 1;
}

void on_connect_failure (void *context, MQTTAsync_failureData *response) {
    printf("Connect failed, rc %d\n", response ? response->code : 0);
    finished = 1;
}

void on_connect (void *context, MQTTAsync_successData *response) {
    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    MQTTAsync_message pub_msg = MQTTAsync_message_initializer;
    int rc;

    printf("Successful connection\n");

    printf("Subscribing to topic %s\n for client %s using QoS%d\n\n"
            "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
    opts.onSuccess = on_subscribe;
    opts.onFailure = on_subscribe_failure;
    opts.context = client;

    delivered_token = 0;

    if ((rc = MQTTAsync_subscribe(client, TOPIC, QOS, &opts)) != MQTTASYNC_SUCCESS) {
        printf("Failed to start subscribe, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
}

int main (void) {
    MQTTAsync client;
    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;
    MQTTAsync_message pub_msg = MQTTAsync_message_initializer;
    MQTTAsync_token token;
    int rc;
    int ch;

    MQTTAsync_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    MQTTAsync_setCallbacks(client, NULL, connection_lost, message_arrived, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.onSuccess = on_connect;
    conn_opts.onFailure = on_connect_failure;
    conn_opts.context = client;

    if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS) {
        printf("Failed to start connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    while (!subscribed) usleep(10000L);

    if (finished)
        goto exit;

    do {
        ch = getchar();
    } while (ch != 'Q' && ch != 'q');

    disc_opts.onSuccess = on_disconnect;
    if ((rc = MQTTAsync_disconnect(client, &disc_opts)) != MQTTASYNC_SUCCESS) {
        printf("Failed to start disconnect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    while (!disc_finished) usleep(10000L);

exit:
    MQTTAsync_destroy(&client);

    printf("Stopped listening, total message received: %llu\n", message_count);

    return rc;
}
