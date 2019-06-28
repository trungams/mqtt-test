const BENCHMARK_ITERATIONS = parseInt(1e5);
var start, stop;

function startClock () {
    start = process.hrtime();
}

function stopClock () {
    stop = process.hrtime(start);
}

function printResult () {
    console.log(
        'The program took %d.%s second(s) to run %d iterations',
        stop[0],
        stop[1].toString().padStart(9, '0'),
        BENCHMARK_ITERATIONS
    );
}

module.exports = {
    startClock: startClock,
    stopClock: stopClock,
    printResult: printResult,
    BENCHMARK_ITERATIONS: BENCHMARK_ITERATIONS
}
