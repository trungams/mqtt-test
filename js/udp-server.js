/*
 * https://www.hacksparrow.com/nodejs/udp-server-and-client-example.html
 */

var dgram = require('dgram')
var benchmarker = require('./benchmarker')

const HOST = '127.0.0.1'
const PORT = 12000
const PAYLOAD = 'hello client\n'

var messagesOut = 0;

var server = dgram.createSocket('udp4');

server.on('listening', function () {
    var address = server.address();
    console.log('UDP server listening on: ' + address.address + ':' + address.port);
});

server.on('close', function () {
    console.log('Connection closed');
    process.exit();
});

server.on('message', function (message, remote) {
    server.send(PAYLOAD, remote.port, remote.address, function (err, bytes) {
        if (!err) {
            messagesOut++;
            if (messagesOut >= benchmarker.BENCHMARK_ITERATIONS) {
                server.close();
            }
        }
    });
});

server.bind(PORT, HOST);
