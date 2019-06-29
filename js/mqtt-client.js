var mqtt = require('mqtt')
var benchmarker = require('./benchmarker')

const ADDRESS   = "tcp://localhost:1883"
const CLIENTID  = "MQTTClient"
const TOPIC_OUT = "client/server"
const TOPIC_IN  = "server/client"
const PAYLOAD   = "hello server"
const QOS       = 1
const TIMEOUT   = 10000

var messagesIn = 0;
var messagesOut = 0;
var started = false, finished = false;

var connectOptions = {
    keepalive: 20,
    clientId: CLIENTID,
    clean: false
}

var client = mqtt.connect(ADDRESS, connectOptions);

client.on('connect', function () {
    client.subscribe(TOPIC_IN, {qos: 1}, function (err) {
        if (!err) {
            console.log('Client has subscribed to topic');
            started = true;
        }
    })
});

client.on('end', function () {
    console.log('Client has finished running');
    console.log('Total messages sent: ' + messagesOut);
    console.log('Total messages received: ' + messagesIn);
})

client.on('message', function (topic, message, packet) {
    if (topic == TOPIC_IN) {
        messagesIn++;
        if (messagesIn >= benchmarker.BENCHMARK_ITERATIONS) {
            finished = true;
            client.end();
        }
    }
});

function publishUntilFinish () {
    if (!finished) {
        client.publish(
            TOPIC_OUT,
            PAYLOAD,
            {qos: 1, retain: false},
            function (err) {
                if (!err) {
                    messagesOut++;
                }
            }
        );
        setTimeout(publishUntilFinish, 1);
    } else {
        benchmarker.stopClock();
        benchmarker.printResult();
    }
}

function main () {
    if (!started) {
        setTimeout(main, 100);
    } else {
        benchmarker.startClock();
        publishUntilFinish();
    }
}

main();
