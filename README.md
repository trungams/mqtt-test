# mqtt-test

All the tests mentioned below were run on my laptop with these specs:

```
Model: Thinkpad T480
OS: Ubuntu 18.04 TLS x84_64
Kernel: 4.18.0-24-generic
CPU: Intel i7-8550 (8) @ 4.000 GHz
GPU: Intel UHD Graphics 620
Memory: 6010 MiB / 15803 MiB
```

For each model, I ran the client-server program at least once, each time with 100,000 iterations (100,000 round-trips)

## MQTT test

### QoS 0

#### C client to C server

```
Total time elapsed: 0.808770998 seconds
Average time is 0.000008087 seconds(s) for 100000 iterations
Average round-trip per second: 123655 (rt/s)
```

#### Js client to Js server

```
Total time elapsed: 124.950524490 seconds
Average time is 0.001249505 second(s) for 100000 iterations
Average round-trip per second: 800 (rt/s)
```

### QoS 1

#### C client to C server

```
Total time elapsed: 1.771083202 seconds
Average time is 0.000017710 seconds(s) for 100000 iterations
Average round-trip per second: 56465 (rt/s)
```

#### C client to Js server

```
Total time elapsed: 15.469506514 seconds
Average time is 0.000154695 seconds(s) for 100000 iterations
Average round-trip per second: 6464 (rt/s)
```

#### Js client to C server

```
Total time elapsed: 109.199560530 seconds
Average time is 0.001091853 second(s) for 100013 iterations
Average round-trip per second: 915 (rt/s)
```

#### Js client to Js server

Since the call to `process.hrtime()` is more expensive in Javascript, it's better to take the elapsed time between the first publication and the last message reception.

```
Average round trips per second: 826 (rt/s)
```

### QoS 2

#### C client to C server

```
Total time elapsed: 4.565175539 seconds
Average time is 0.000045651 seconds(s) for 100000 iterations
Average round-trip per second: 21905 (rt/s)
```

#### Js client to Js server

```
Total time elapsed: 118.117489553 seconds
Average time is 0.001181092 second(s) for 100007 iterations
Average round-trip per second: 846 (rt/s)
```

## TCP test (for reference)

`TCP_NODELAY` were enabled so every message was put in a separate TCP packet. Below is the result of a single test run with 100,000 round trips.

```
Socket successfully created
Total time elapsed: 0.680732861 seconds
Average time is 0.000006807 seconds(s) for 100000 iterations
Average round-trip per second: 146907
```

## UDP test (for reference)

Below is the result of a single test run with 100,000 round trips.

```
Socket successfully created
Total time elapsed: 0.579993131 seconds
Average time is 0.000005799 seconds(s) for 100000 iterations
Average round-trip per second: 172443 (rt/s)
```
