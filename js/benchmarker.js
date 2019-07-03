const BENCHMARK_ITERATIONS = parseInt(1e5);
const BILLION = parseInt(1e9);
const NSEC_PER_SEC = parseInt(1e9);
var start, stop;

var sum = [0,0];
var result = [0,0];

function startClock () {
    // console.log("Start clock");
    start = process.hrtime();
}

function stopClock () {
    // console.log("Stop clock");
    stop = process.hrtime(start);
    sum = addTime(sum, stop);
}

function addTime(a,b) {
    var _sum = [0,0];
    _sum[0] = a[0] + b[0];
    _sum[1] = a[1] + b[1];
    if (_sum[1] >= BILLION) {
        _sum[1] -= BILLION;
        _sum[0]++;
    }
    return _sum;
}

function printResult (iterations) {
    iterations = iterations ? iterations : BENCHMARK_ITERATIONS;

    var totalNanoSeconds = sum[0] * NSEC_PER_SEC + sum[1];
    var average = parseInt(totalNanoSeconds / iterations);
    result[0] = parseInt(average / NSEC_PER_SEC);
    result[1] = average % NSEC_PER_SEC;
    var roundTripPerSecond = parseInt(BILLION / average);

    console.log("\033[1;31m");
    console.log("Total time elapsed: %d.%s seconds",
        sum[0], sum[1].toString().padStart(9, '0')
    );
    console.log(
        'Average time is %d.%s second(s) for %d iterations',
        result[0],
        result[1].toString().padStart(9, '0'),
        iterations
    );
    console.log("Average round-trip per second: " + roundTripPerSecond + " (rt/s)");
    console.log("\033[0m");
}

module.exports = {
    startClock: startClock,
    stopClock: stopClock,
    printResult: printResult,
    BENCHMARK_ITERATIONS: BENCHMARK_ITERATIONS
}
