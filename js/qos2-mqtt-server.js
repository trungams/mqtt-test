var mqtt = require('mqtt')
var sleep = require('sleep')
var benchmarker = require('./benchmarker')

const ADDRESS   = "tcp://localhost:1883"
const CLIENTID  = "MQTTServer"
const TOPIC_OUT = "server/client"
const TOPIC_IN  = "client/server"
const PAYLOAD   = "hello client"
const QOS       = 2
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

function onMessageDelivery (err) {
    if (!err) {
        messagesOut++;
        // console.log('Message has been published');
        if (messagesOut >= benchmarker.BENCHMARK_ITERATIONS) {
            sleep.sleep(3);
            client.end();
        }
    } else {
        console.log('Error ' + err + err.toString());
    }
};

client.on('message', function (topic, message) {
    if (topic == TOPIC_IN) {
        messagesIn++;
        client.publish(
            TOPIC_OUT,
            PAYLOAD,
            {qos: QOS, retain: false},
            onMessageDelivery
        );
    }
});
