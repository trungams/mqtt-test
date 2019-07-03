/*
 * https://www.hacksparrow.com/nodejs/tcp-socket-programming-in-node-js.html
 */

var net = require('net')
var benchmarker = require('./benchmarker')

const HOST = '127.0.0.1'
const PORT = 12000
const PAYLOAD = 'hello client\n'

var messagesOut = 0;

var server = net.createServer(function (sock) {
    console.log('Connected: ' + sock.remoteAddress + ':' + sock.remotePort);

    sock.on('close', function (data) {
        console.log('Closed: ' + sock.remoteAddress + ':' + sock.remotePort);
    });

    sock.on('data', function (data) {
        sock.write(PAYLOAD);
        messagesOut++;
        if (messagesOut >= benchmarker.BENCHMARK_ITERATIONS) {
            server.close();
        }
    });
});

server.listen(PORT, HOST);

console.log('Server listening on ' + HOST + ':' + PORT);
