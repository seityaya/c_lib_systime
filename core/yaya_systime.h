#ifndef YAYA_SYSTIME_H
#define YAYA_SYSTIME_H

#include "stdint.h"

typedef double time_float_t;

typedef enum {
    YAYA_TIME_CLOCK_TYPE_REALTIME,
    YAYA_TIME_CLOCK_TYPE_MONOTONIC,
    YAYA_TIME_CLOCK_TYPE_CPUTIME,
} time_clockid_type_e;

typedef enum {
    YAYA_TIME_FRAGMENT_TYPE_RESOLUTION,
    YAYA_TIME_FRAGMENT_TYPE_REAL,
    YAYA_TIME_FRAGMENT_TYPE_USER,
    YAYA_TIME_FRAGMENT_TYPE_SYS,
} time_fragment_type_e;

typedef struct {
    int64_t signum; // -1 or +1, 0 if not normasize
    int64_t second; //  0 .. INT64_MAX
    int64_t millis; //  0 .. 999
    int64_t micros; //  0 .. 999
    int64_t nanos;  //  0 .. 999
} time_fragment_t;

typedef struct {
    time_fragment_t real;
    time_fragment_t user;
    time_fragment_t sys;
} time_system_t;


time_fragment_t time_get(time_fragment_type_e type, time_clockid_type_e clockid);

int64_t         time_delay(time_fragment_t time_delay, time_clockid_type_e clockid);
int64_t         time_sleep(time_fragment_t time_sleep, time_clockid_type_e clockid);

time_fragment_t time_nor(time_fragment_t time_fragment);
time_fragment_t time_dif(time_fragment_t beg, time_fragment_t end);
time_fragment_t time_sum(time_fragment_t beg, time_fragment_t end);

time_fragment_t time_build(int64_t second, int64_t millis, int64_t micros, int64_t nanos);
time_float_t    time_convflt(time_fragment_t time_fragment);
int64_t         time_convstr(time_fragment_t time_fragment, char* buff, int64_t size);


time_system_t   time_system_get(time_clockid_type_e clockid);

time_system_t   time_system_nor(time_system_t time_sys);
time_system_t   time_system_dif(time_system_t time_sys_1, time_system_t time_sys_2);
time_system_t   time_system_sum(time_system_t time_sys_1, time_system_t time_sys_2);

int64_t         time_system_convstr(time_system_t time_sys, char* buff, int64_t size);

#endif /*YAYA_SYSTIME_H*/
