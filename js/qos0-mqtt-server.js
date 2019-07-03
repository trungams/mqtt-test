var mqtt = require('mqtt')
var benchmarker = require('./benchmarker')

const ADDRESS   = "tcp://localhost:1883"
const CLIENTID  = "MQTTServer"
const TOPIC_OUT = "server/client"
const TOPIC_IN  = "client/server"
const PAYLOAD   = "hello client"
const QOS       = 0
const TIMEOUT   = 10000

var messagesIn = 0;
var messagesOut = 0;

var connectOptions = {
    keepalive: 20,
    clientId: CLIENTID,
    clean: false
}

var client = mqtt.connect(ADDRESS, connectOptions);

client.on('connect', function () {
    client.subscribe(TOPIC_IN, {qos: QOS}, function (err) {
        if (!err) {
            console.log('Server has subscribed to topic');
        }
    })
});

client.on('end', function () {
    console.log('Server has finished running');
    console.log('Total messages sent: ' + messagesOut);
    console.log('Total messages received: ' + messagesIn);
});

client.on('message', function (topic, message) {
    if (topic == TOPIC_IN) {
        messagesIn++;
        // console.log('Server has received %d messages', messagesIn);
        client.publish(
            TOPIC_OUT,
            PAYLOAD,
            {qos: QOS, retain: false}
        );
        if (messagesIn >= benchmarker.BENCHMARK_ITERATIONS) {
            finished = true;
            client.end();
        }
    }
});
