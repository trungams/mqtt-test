/*
 * https://www.hacksparrow.com/nodejs/udp-server-and-client-example.html
 */

var dgram = require('dgram')
var benchmarker = require('./benchmarker')

const HOST = '127.0.0.1'
const PORT = 12000
const PAYLOAD = 'server\n'

var messagesIn = 0, messagesOut = 0;

var client = dgram.createSocket('udp4');

function main () {
    console.log('Start sending messages');

    benchmarker.startClock();

    client.send(PAYLOAD, PORT, HOST, function (err, bytes) {
        messagesOut++;
    });
}

client.on('message', function (message, remote) {
    messagesIn++;
    if (messagesIn >= benchmarker.BENCHMARK_ITERATIONS) {
        benchmarker.stopClock();
        client.close();
    } else {
        client.send(PAYLOAD, PORT, HOST, function (err, bytes) {
            messagesOut++;
        });
    }
});

client.on('close', function () {
    console.log('Connection closed');
    benchmarker.printResult(messagesIn);
})

main();
