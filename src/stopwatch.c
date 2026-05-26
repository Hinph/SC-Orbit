#include "stopwatch.h"

#include <stdio.h>
#include <string.h>

static void timespec_diff(
    struct timespec const* const a,
    struct timespec const* const b,
    struct timespec* const result
) {
    result->tv_sec  = a->tv_sec  - b->tv_sec;
    result->tv_nsec = a->tv_nsec - b->tv_nsec;
    if (result->tv_nsec < 0) {
        result->tv_sec  -= 1;
        result->tv_nsec += 1000000000L;
    }
}

static inline int64_t timespec_to_msecs(struct timespec const* const ts) {
    return ((int64_t)ts->tv_sec  *    1000) +
           ((int64_t)ts->tv_nsec / 1000000);
}

int timespec_now(struct timespec* const ts) {
    if (clock_gettime(CLOCK_MONOTONIC, ts)) {
        perror("failed to get the current time");
        return 1;
    }
    return 0;
}

int Stopwatch_init(struct Stopwatch* const watch, int64_t duration) {
    watch->duration = duration;
    if (timespec_now(&watch->ts_started)) {
        return 1;
    }
    return 0;
}

int Stopwatch_reset(struct Stopwatch* const watch) {
    watch->triggered = 0;
    if (timespec_now(&watch->ts_started)) {
        return 1;
    }
    return 0;
}

int Stopwatch_update(struct Stopwatch* const watch) {
    struct timespec ts_curr;
    struct timespec ts_diff;
    memset(&ts_curr, 0, sizeof(ts_curr));
    memset(&ts_diff, 0, sizeof(ts_diff));

    if (timespec_now(&ts_curr)) {
        return 1;
    }

    timespec_diff(&ts_curr, &watch->ts_started, &ts_diff);

    if (timespec_to_msecs(&ts_diff) >= watch->duration) {
        watch->triggered = 1;
    }
    return 0;
}
