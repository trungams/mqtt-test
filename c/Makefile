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

mqtt: $(CLIENT) $(SERVER)
	if [ ! -d $(BIN_DIR) ]; then mkdir -p $(BIN_DIR); fi
	$(CC) -o $(BIN_DIR)/qos0-mqtt-client $(CFLAGS) qos0_$(MQTT_CLIENT) $(BENCHMARKER) $(LIBS)
	$(CC) -o $(BIN_DIR)/qos0-mqtt-server $(CFLAGS) qos0_$(MQTT_SERVER) $(BENCHMARKER) $(LIBS)
	$(CC) -o $(BIN_DIR)/qos1-mqtt-client $(CFLAGS) qos1_$(MQTT_CLIENT) $(BENCHMARKER) $(LIBS)
	$(CC) -o $(BIN_DIR)/qos1-mqtt-server $(CFLAGS) qos1_$(MQTT_SERVER) $(BENCHMARKER) $(LIBS)
	$(CC) -o $(BIN_DIR)/qos2-mqtt-client $(CFLAGS) qos2_$(MQTT_CLIENT) $(BENCHMARKER) $(LIBS)
	$(CC) -o $(BIN_DIR)/qos2-mqtt-server $(CFLAGS) qos2_$(MQTT_SERVER) $(BENCHMARKER) $(LIBS)

tcp:
	if [ ! -d $(BIN_DIR) ]; then mkdir -p $(BIN_DIR); fi
	$(CC) -o $(BIN_DIR)/tcp-client $(CFLAGS) tcp_client.c $(BENCHMARKER) $(LIBS)
	$(CC) -o $(BIN_DIR)/tcp-server $(CFLAGS) tcp_server.c $(BENCHMARKER) $(LIBS)

udp:
	if [ ! -d $(BIN_DIR) ]; then mkdir -p $(BIN_DIR); fi
	$(CC) -o $(BIN_DIR)/udp-client $(CFLAGS) udp_client.c $(BENCHMARKER) $(LIBS)
	$(CC) -o $(BIN_DIR)/udp-server $(CFLAGS) udp_server.c $(BENCHMARKER) $(LIBS)

debug:
	if [ ! -d $(BIN_DIR) ]; then mkdir -p $(BIN_DIR); fi
	$(CC) -o $(BIN_DIR)/qos0-mqtt-client -D DEBUG $(CFLAGS) qos0_$(MQTT_CLIENT) $(BENCHMARKER) $(LIBS)
	$(CC) -o $(BIN_DIR)/qos0-mqtt-server -D DEBUG $(CFLAGS) qos0_$(MQTT_SERVER) $(BENCHMARKER) $(LIBS)
	$(CC) -o $(BIN_DIR)/qos1-mqtt-client -D DEBUG $(CFLAGS) qos1_$(MQTT_CLIENT) $(BENCHMARKER) $(LIBS)
	$(CC) -o $(BIN_DIR)/qos1-mqtt-server -D DEBUG $(CFLAGS) qos1_$(MQTT_SERVER) $(BENCHMARKER) $(LIBS)
	$(CC) -o $(BIN_DIR)/qos2-mqtt-client -D DEBUG $(CFLAGS) qos2_$(MQTT_CLIENT) $(BENCHMARKER) $(LIBS)
	$(CC) -o $(BIN_DIR)/qos2-mqtt-server -D DEBUG $(CFLAGS) qos2_$(MQTT_SERVER) $(BENCHMARKER) $(LIBS)

clean:
	rm -rf $(BIN_DIR)/*
