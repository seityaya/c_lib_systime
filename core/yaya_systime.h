#ifndef YAYA_SYSTIME_H
#define YAYA_SYSTIME_H

#include "stdbool.h"
#include "stdint.h"
#include "time.h"

typedef double yaya_systime_float_t;

typedef enum {
    YAYA_TIME_CLOCK_TYPE_REALTIME   = CLOCK_REALTIME,
    YAYA_TIME_CLOCK_TYPE_MONOTONIC  = CLOCK_MONOTONIC,
    YAYA_TIME_CLOCK_TYPE_CPUTIME    = CLOCK_PROCESS_CPUTIME_ID,
} yaya_time_clockid_type_e;

typedef enum {
    YAYA_TIME_FRAGMENT_TYPE_RESOLUTION,
    YAYA_TIME_FRAGMENT_TYPE_REAL,
    YAYA_TIME_FRAGMENT_TYPE_USER,
    YAYA_TIME_FRAGMENT_TYPE_SYS,
} yaya_time_fragment_type_e;

typedef struct {
    int64_t signum; // -1 or +1, 0 if not normasize
    int64_t second; //  0 .. INT64_MAX
    int64_t millis; //  0 .. 999
    int64_t micros; //  0 .. 999
    int64_t nanos;  //  0 .. 999
} yaya_time_fragment_t;

typedef struct {
    yaya_time_fragment_t real;
    yaya_time_fragment_t user;
    yaya_time_fragment_t sys;
} yaya_time_sys_t;

int64_t              yaya_time_delay(yaya_time_fragment_t time_delay, yaya_time_clockid_type_e clockid);
int64_t              yaya_time_sleep(yaya_time_fragment_t time_sleep, yaya_time_clockid_type_e clockid);

yaya_time_fragment_t yaya_time_fragment_get(yaya_time_fragment_type_e type, yaya_time_clockid_type_e clockid);

yaya_time_fragment_t yaya_time_fragment_nor(yaya_time_fragment_t time_fragment);
yaya_time_fragment_t yaya_time_fragment_dif(yaya_time_fragment_t beg, yaya_time_fragment_t end);
yaya_time_fragment_t yaya_time_fragment_sum(yaya_time_fragment_t beg, yaya_time_fragment_t end);

yaya_time_fragment_t yaya_time_fragment_build(int64_t second, int64_t millis, int64_t micros, int64_t nanos);
yaya_systime_float_t yaya_time_fragment_convflt(yaya_time_fragment_t time_fragment);
int64_t              yaya_time_fragment_convstr(yaya_time_fragment_t time_fragment, char* buff, int64_t size);


yaya_time_sys_t      yaya_time_sys_get(void);

yaya_time_sys_t      yaya_time_sys_nor(yaya_time_sys_t time_sys);
yaya_time_sys_t      yaya_time_sys_dif(yaya_time_sys_t time_sys_1, yaya_time_sys_t time_sys_2);
yaya_time_sys_t      yaya_time_sys_sum(yaya_time_sys_t time_sys_1, yaya_time_sys_t time_sys_2);

int64_t              yaya_time_sys_convstr(yaya_time_sys_t time_sys, char* buff, int64_t size);

#endif /*YAYA_SYSTIME_H*/
