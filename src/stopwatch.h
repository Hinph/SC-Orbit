#ifndef SCWRAPPER_STOPWATCH_H
#define SCWRAPPER_STOPWATCH_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

struct Stopwatch {
    struct timespec ts_started;
    int64_t duration; // milliseconds
    bool triggered;
};

int Stopwatch_init(struct Stopwatch* const watch, int64_t duration);
int Stopwatch_update(struct Stopwatch* const watch);
int Stopwatch_reset(struct Stopwatch* const watch);

#endif // SCWRAPPER_STOPWATCH_H
