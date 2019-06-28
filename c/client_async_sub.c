/*
 * https://www.eclipse.org/paho/files/mqttdoc/MQTTClient/html/subasync.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <MQTTClient.h>
#include "./benchmark.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "test/example"
#define PAYLOAD     "Hello, world!"
#define QOS         1
#define TIMEOUT     10000L

volatile MQTTClient_deliveryToken delivered_token;
unsigned long long message_count = 0;

void delivered (void *context, MQTTClient_deliveryToken dt) {
    delivered_token = dt;
}

int message_arrived (void *context, char *topic_name, int topic_len, MQTTClient_message *message) {
    printf("%llu\n", ++message_count);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topic_name);
    return 1;
}

void connection_lost (void *context, char *cause) {
    printf("\nConnection lost\n");
    printf("\tcause: %s\n", cause);
}

int main (void) {
    // mqtt structs
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
            MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    MQTTClient_setCallbacks(client, NULL, connection_lost, message_arrived, delivered);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    printf("Subscribing to topic %s\n for client %s using QoS%d\n\n"
            "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
    MQTTClient_subscribe(client, TOPIC, QOS);

    do {
        ch = getchar();
    } while (ch != 'Q' && ch != 'q');

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    printf("Stopped listening, total message received: %llu\n", message_count);

    return rc;
}
