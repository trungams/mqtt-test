#include "./benchmark.h"

void timespec_difference (struct timespec *result, struct timespec *begin, struct timespec *end) {
    if (end->tv_nsec < begin->tv_nsec) {
        result->tv_sec = end->tv_sec - begin->tv_sec - 1;
        result->tv_nsec = end->tv_nsec - begin->tv_nsec + 1e9;
    } else {
        result->tv_sec = end->tv_sec - begin->tv_sec;
        result->tv_nsec = end->tv_nsec - begin->tv_nsec;
    }
}

void print_result (struct timespec *result) {
    printf("\033[1;31m");
    printf("The program took %lu.%09lu second(s) to run %llu iterations.\n", result->tv_sec, result->tv_nsec, BENCHMARK_ITERATIONS);
    printf("\033[0m");
}
