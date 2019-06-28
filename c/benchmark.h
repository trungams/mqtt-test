#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define Benchmark_timespec_init (struct timespec*)malloc(sizeof(struct timespec))

static const unsigned long long BENCHMARK_ITERATIONS = 1e5;

void timespec_difference (struct timespec*, struct timespec*, struct timespec*);
void print_result (struct timespec*);
