#ifndef YAYA_SYSTIME_H
#define YAYA_SYSTIME_H

#include "time.h"
#include "stdint.h"
#include "stdbool.h"

#define NANOS 1000000000
#define MICRO 1000000
#define MILLI 1000

typedef struct timespec realtime_t;

realtime_t get_time_real();

realtime_t time_dif(realtime_t beg, realtime_t end);

realtime_t time_sum(realtime_t beg, realtime_t end);

realtime_t time_build(uint64_t sec, uint64_t nsec);

long double time_convert(realtime_t time);

void time_sleep(realtime_t time);

#endif /*YAYA_SYSTIME_H*/
