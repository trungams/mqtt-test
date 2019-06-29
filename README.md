For each model, I ran the client-server program multiple times, each time with 100,000 iterations (100,000 round-trips)

## C client to C server model:
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
Average round trips per second: 100,000 / 35.226790378 ~ 2838 (rt/s)

## C client to Js server:
    - 188.592342917
    - 203.759902348
    - 218.887707756
    - 203.947276544
    - 247.063681056
Average round trips per second: ~ 470 (rt/s)

## Js client to C server:
    - 108.895439412
    - 108.845733769
    - 108.777132635
    - 108.983030826
    - 108.880994437
Average round trips per second: ~ 918 (rt/s)

## Js client to Js server:
    - 120.904808283
    - 121.231965020
    - 121.020516876
    - 120.687788815
    - 121.299585964
Average round trips per second: 826 (rt/s)
