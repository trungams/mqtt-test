# Compiler
CC=gcc
LIBS=-lpaho-mqtt3c -lpaho-mqtt3a -lpaho-mqtt3as -lm
CFLAGS=-g -Wall
BENCHMARKER=benchmark.c
MQTT_CLIENT=mqtt_client.c
MQTT_SERVER=mqtt_server.c

BIN_DIR=./bin

.PHONY: async sync mqtt tcp udp

all: mqtt tcp udp

async: async_client_pub.c async_client_sub.c
	if [ ! -d $(BIN_DIR) ]; then mkdir -p $(BIN_DIR); fi
	$(CC) -o $(BIN_DIR)/async_client_pub $(CFLAGS) async_client_pub.c $(BENCHMARKER) $(LIBS)
	$(CC) -o $(BIN_DIR)/async_client_sub $(CFLAGS) async_client_sub.c $(BENCHMARKER) $(LIBS)

sync: client_sync_pub.c client_async_pub.c client_async_sub.c $(BENCHMARKER)
	if [ ! -d $(BIN_DIR) ]; then mkdir -p $(BIN_DIR); fi
	$(CC) -o $(BIN_DIR)/client_sync_pub $(CFLAGS) client_sync_pub.c $(BENCHMARKER) $(LIBS)
	$(CC) -o $(BIN_DIR)/client_async_pub $(CFLAGS) client_async_pub.c $(BENCHMARKER) $(LIBS)
	$(CC) -o $(BIN_DIR)/client_async_sub $(CFLAGS) client_async_sub.c $(BENCHMARKER) $(LIBS)

mqtt: $(CLIENT) $(SERVER)
	if [ ! -d $(BIN_DIR) ]; then mkdir -p $(BIN_DIR); fi
	$(CC) -o $(BIN_DIR)/mqtt-client $(CFLAGS) $(MQTT_CLIENT) $(BENCHMARKER) $(LIBS)
	$(CC) -o $(BIN_DIR)/mqtt-server $(CFLAGS) $(MQTT_SERVER) $(BENCHMARKER) $(LIBS)

tcp:
	if [ ! -d $(BIN_DIR) ]; then mkdir -p $(BIN_DIR); fi
	$(CC) -o $(BIN_DIR)/tcp-client $(CFLAGS) tcp_client.c $(BENCHMARKER) $(LIBS)
	$(CC) -o $(BIN_DIR)/tcp-server $(CFLAGS) tcp_server.c $(BENCHMARKER) $(LIBS)

udp:
	if [ ! -d $(BIN_DIR) ]; then mkdir -p $(BIN_DIR); fi
	$(CC) -o $(BIN_DIR)/udp-client $(CFLAGS) udp_client.c $(BENCHMARKER) $(LIBS)
	$(CC) -o $(BIN_DIR)/udp-server $(CFLAGS) udp_server.c $(BENCHMARKER) $(LIBS)

clean:
	rm -rf $(BIN_DIR)/*
