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

For each model, I ran the client-server program multiple times, each time with 100,000 iterations (100,000 round-trips)

## MQTT test

### C client to C server model:
    - 33.863676486
    - 36.129232282
    - 37.417619567
    - 37.339931135
    - 39.621571466
    - 40.188722199
    - 33.449645491
    - 31.530099426
    - 31.418409862
    - 31.308995866
Average round trips per second: 100,000 / 35.226790378 ~ 2,838 (rt/s)

### C client to Js server:
    - 188.592342917
    - 203.759902348
    - 218.887707756
    - 203.947276544
    - 247.063681056
Average round trips per second: ~ 470 (rt/s)

### Js client to C server:
    - 108.895439412
    - 108.845733769
    - 108.777132635
    - 108.983030826
    - 108.880994437
Average round trips per second: ~ 918 (rt/s)

### Js client to Js server:
    - 120.904808283
    - 121.231965020
    - 121.020516876
    - 120.687788815
    - 121.299585964
Average round trips per second: 826 (rt/s)


## TCP test (for reference)

TCP_NODELAY were enabled so every message was put in a separate TCP packet

    - 1.037630941
    - 1.030040723
    - 1.094750510
    - 0.999576736
    - 1.054357771
Average round trips per second: 95,852 (rt/s)

## UDP test (for reference)

    - 0.907663345
    - 0.906056855
    - 0.883218538
    - 0.869614683
    - 0.880399120
Average round trips per second: 112,436 (rt/s)
