/*
 * https://www.hacksparrow.com/nodejs/tcp-socket-programming-in-node-js.html
 */

var net = require('net')
var benchmarker = require('./benchmarker')

const HOST = '127.0.0.1'
const PORT = 12000
const PAYLOAD = 'hello server\n'

var client = new net.Socket();
var messagesIn = 0, messagesOut = 0;

client.connect(PORT, HOST, function () {
    console.log('Connected to: ' + HOST + ':' + PORT);
    benchmarker.startClock();

    client.write(PAYLOAD);
    messagesOut++;
});

client.on('data', function (data) {
    messagesIn++;
    if (messagesIn >= benchmarker.BENCHMARK_ITERATIONS) {
        benchmarker.stopClock();
        client.destroy();
    } else {
        client.write(PAYLOAD);
        messagesOut++;
    }
});

client.on('close', function () {
    console.log('Connection closed');
    benchmarker.printResult(messagesIn);
});
