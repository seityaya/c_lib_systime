#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include <sys/times.h>
#include <time.h>
#include <unistd.h>

#include "yaya_systime.h"

#define CONSTANT_NANOS INT64_C(1000000000)
#define CONSTANT_MICRO INT64_C(1000000)
#define CONSTANT_MILLI INT64_C(1000)

#define CONSTANT_MIN_FRACTIONAL INT64_C(0)
#define CONSTANT_MAX_FRACTIONAL INT64_C(999)
#define CONSTANT_DIV_FRACTIONAL INT64_C(1000)

/*==================================================================================================================================================*/
#define set_sign(time_fragment)                                                                                                                      \
    do {                                                                                                                                             \
        if (time_fragment.signum != 0) {                                                                                                             \
            time_fragment.nanos  *= time_fragment.signum;                                                                                            \
            time_fragment.micros *= time_fragment.signum;                                                                                            \
            time_fragment.millis *= time_fragment.signum;                                                                                            \
            time_fragment.second *= time_fragment.signum;                                                                                            \
            time_fragment.signum  = 0;                                                                                                               \
        }                                                                                                                                            \
    } while (false)

#define counter(m, M)                                                                                                                                \
    do {                                                                                                                                             \
        if (m != 0) {                                                                                                                                \
            M += m / CONSTANT_DIV_FRACTIONAL;                                                                                                        \
            m  = m % CONSTANT_DIV_FRACTIONAL;                                                                                                        \
        }                                                                                                                                            \
    } while (false)

#define signums(m, s)                                                                                                                                \
    do {                                                                                                                                             \
        if (s == 0) {                                                                                                                                \
            s = +1;                                                                                                                                  \
        }                                                                                                                                            \
        if (m > 0) {                                                                                                                                 \
            m *= +1;                                                                                                                                 \
            s  = +1;                                                                                                                                 \
        }                                                                                                                                            \
        if (m < 0) {                                                                                                                                 \
            m *= -1;                                                                                                                                 \
            s  = -1;                                                                                                                                 \
        }                                                                                                                                            \
    } while (false)

static time_fragment_t time_nor(time_fragment_t time_fragment) {
    set_sign(time_fragment);

    counter(time_fragment.nanos, time_fragment.micros);
    counter(time_fragment.micros, time_fragment.millis);
    counter(time_fragment.millis, time_fragment.second);

    signums(time_fragment.nanos, time_fragment.signum);
    signums(time_fragment.micros, time_fragment.signum);
    signums(time_fragment.millis, time_fragment.signum);
    signums(time_fragment.second, time_fragment.signum);

    return time_fragment;
}
/*==================================================================================================================================================*/

static clockid_t time_clockid_type_get(time_clockid_type_e type) {
    switch (type) {
    case YAYA_TIME_CLOCK_TYPE_REALTIME:
        return CLOCK_REALTIME;
    case YAYA_TIME_CLOCK_TYPE_MONOTONIC:
        return CLOCK_MONOTONIC;
    case YAYA_TIME_CLOCK_TYPE_CPUTIME:
        return CLOCK_PROCESS_CPUTIME_ID;
    default:
        return -1;
    }
}

time_fragment_t time_get(time_fragment_type_e type, time_clockid_type_e clockid) {
    time_fragment_t time_fragment = {0};

    int             value_ts;
    struct timespec clock_time;
    long            value_tms;
    struct tms      tms;

    switch (type) {
    case YAYA_TIME_FRAGMENT_TYPE_RESOLUTION: {
        value_ts = clock_getres(CLOCK_REALTIME, &clock_time);
        if (value_ts == -1) {
            return time_fragment;
        }
        return time_build(clock_time.tv_sec, 0, 0, clock_time.tv_nsec);
    }
    case YAYA_TIME_FRAGMENT_TYPE_REAL: {
        value_ts = clock_gettime(time_clockid_type_get(clockid), &clock_time);
        if (value_ts == -1) {
            return time_fragment;
        }
        return time_build(clock_time.tv_sec, 0, 0, clock_time.tv_nsec);
    }
    case YAYA_TIME_FRAGMENT_TYPE_USER: {
        value_tms = times(&tms);
        if (value_tms == -1) {
            return time_fragment;
        }
        int64_t      tps  = (int64_t)sysconf(_SC_CLK_TCK);
        time_float_t dsec = (time_float_t)(tms.tms_utime) / (time_float_t)(tps);
        int64_t      sec  = (int64_t)(dsec);
        int64_t      nns  = ((int64_t)(dsec * (time_float_t)(tps)) - (sec * tps)) * (CONSTANT_NANOS / tps);

        return time_build(sec, 0, 0, nns);
    }
    case YAYA_TIME_FRAGMENT_TYPE_SYS: {
        value_tms = times(&tms);
        if (value_tms == -1) {
            return time_fragment;
        }
        int64_t      tps  = (int64_t)sysconf(_SC_CLK_TCK);
        time_float_t dsec = (time_float_t)(tms.tms_stime) / (time_float_t)(tps);
        int64_t      sec  = (int64_t)(dsec);
        int64_t      nns  = ((int64_t)(dsec * (time_float_t)(tps)) - (sec * tps)) * (CONSTANT_NANOS / tps);

        return time_build(sec, 0, 0, nns);
    }
    }
    return time_fragment;
}

int64_t time_delay(time_fragment_t time_delay, time_clockid_type_e clockid) {
    time_delay = time_nor(time_delay);

    time_fragment_t t_beg = {0};
    time_fragment_t t_end = {0};
    time_fragment_t t_cur = {0};
    time_fragment_t t_dif = {0};

    t_beg = time_get(YAYA_TIME_FRAGMENT_TYPE_REAL, clockid);
    t_end = time_sum(t_beg, time_delay);

    do {
        t_cur = time_get(YAYA_TIME_FRAGMENT_TYPE_REAL, clockid);
        t_dif = time_dif(t_cur, t_end);
    } while (time_convflt(t_dif) < 0);
    return 0;
}

int64_t time_sleep(time_fragment_t time_sleep, time_clockid_type_e clockid) {
    time_sleep = time_nor(time_sleep);

    struct timespec clock_time;

    // clang-format off
    clock_time.tv_sec = time_sleep.second;
    clock_time.tv_nsec = time_sleep.millis * CONSTANT_DIV_FRACTIONAL * CONSTANT_DIV_FRACTIONAL +
                         time_sleep.micros * CONSTANT_DIV_FRACTIONAL +
                         time_sleep.nanos;
    // clang-format on
    return clock_nanosleep(clockid, 0, &clock_time, NULL);
}

/*==================================================================================================================================================*/

time_fragment_t time_dif(time_fragment_t time_fragment_1, time_fragment_t time_fragment_2) {
    time_fragment_1 = time_nor(time_fragment_1);
    time_fragment_2 = time_nor(time_fragment_2);

    time_fragment_t time_fragment = {0};

    set_sign(time_fragment_1);
    set_sign(time_fragment_2);
    time_fragment.nanos  = time_fragment_1.nanos - time_fragment_2.nanos;
    time_fragment.micros = time_fragment_1.micros - time_fragment_2.micros;
    time_fragment.millis = time_fragment_1.millis - time_fragment_2.millis;
    time_fragment.second = time_fragment_1.second - time_fragment_2.second;
    time_fragment.signum = 0;

    return time_nor(time_fragment);
}

time_fragment_t time_sum(time_fragment_t time_fragment_1, time_fragment_t time_fragment_2) {
    time_fragment_1 = time_nor(time_fragment_1);
    time_fragment_2 = time_nor(time_fragment_2);

    time_fragment_t time_fragment = {0};

    set_sign(time_fragment_1);
    set_sign(time_fragment_2);
    time_fragment.nanos  = time_fragment_1.nanos + time_fragment_2.nanos;
    time_fragment.micros = time_fragment_1.micros + time_fragment_2.micros;
    time_fragment.millis = time_fragment_1.millis + time_fragment_2.millis;
    time_fragment.second = time_fragment_1.second + time_fragment_2.second;
    time_fragment.signum = 0;

    return time_nor(time_fragment);
}

/*==================================================================================================================================================*/

time_fragment_t time_build(int64_t second, int64_t millis, int64_t micros, int64_t nanos) {
    time_fragment_t time_fragment = {0};

    time_fragment.nanos  = nanos;
    time_fragment.micros = micros;
    time_fragment.millis = millis;
    time_fragment.second = second;
    time_fragment.signum = 0;

    return time_nor(time_fragment);
}

#define inversefactor(number, factor) (((number) != 0) ? (((time_float_t)(1) / (time_float_t)(factor)) * (time_float_t)(number)) : (0))

time_float_t time_convflt(time_fragment_t time_fragment) {
    time_fragment = time_nor(time_fragment);

    // clang-format off
    time_float_t time_float =
            (time_float_t)(time_fragment.signum) *
                 ((time_float_t)(time_fragment.second)                         +
                           inversefactor(time_fragment.millis, CONSTANT_MILLI) +
                           inversefactor(time_fragment.micros, CONSTANT_MICRO) +
                           inversefactor(time_fragment.nanos , CONSTANT_NANOS));
    // clang-format on
    return time_float;
}

static int64_t scnprintf(char *buf, int64_t size, const char *fmt, ...) {
    va_list args;
    int     i;

    va_start(args, fmt);
    i = vsnprintf(buf, size, fmt, args);
    va_end(args);

    return (i >= size) ? size : i;
}

int64_t time_convstr(time_fragment_t time_fragment, char *buff, int64_t size) {
    time_fragment = time_nor(time_fragment);

    int64_t count = 0;

    count += scnprintf(&buff[count], size - count, "| %c", time_fragment.signum == +1 ? '+' : time_fragment.signum == -1 ? '-' : '?');
    count += scnprintf(&buff[count], size - count, "%11" PRIi64 " sec  ", time_fragment.second);
    count += scnprintf(&buff[count], size - count, "%3" PRIi64 " millis  ", time_fragment.millis);
    count += scnprintf(&buff[count], size - count, "%3" PRIi64 " micros  ", time_fragment.micros);
    count += scnprintf(&buff[count], size - count, "%3" PRIi64 " nanos ", time_fragment.nanos);
    count += scnprintf(&buff[count], size - count, "| %+22.9f fraction second |", time_convflt(time_fragment));

    return count;
}

/*==================================================================================================================================================*/

static time_system_t time_system_nor(time_system_t time_sys) {
    time_sys.real = time_nor(time_sys.real);
    time_sys.user = time_nor(time_sys.user);
    time_sys.sys  = time_nor(time_sys.sys);

    return time_sys;
}

time_system_t time_system_get(time_clockid_type_e clockid) {
    time_system_t time_sys = {0};

    time_sys.real = time_get(YAYA_TIME_FRAGMENT_TYPE_REAL, clockid);
    time_sys.user = time_get(YAYA_TIME_FRAGMENT_TYPE_USER, clockid);
    time_sys.sys  = time_get(YAYA_TIME_FRAGMENT_TYPE_SYS, clockid);

    return time_sys;
}

time_system_t time_system_dif(time_system_t time_sys_1, time_system_t time_sys_2) {
    time_system_t time_sys = {0};

    time_sys.real = time_dif(time_sys_1.real, time_sys_2.real);
    time_sys.user = time_dif(time_sys_1.user, time_sys_2.user);
    time_sys.sys  = time_dif(time_sys_1.sys, time_sys_2.sys);

    return time_sys;
}

time_system_t time_system_sum(time_system_t time_sys_1, time_system_t time_sys_2) {
    time_system_t time_sys = {0};

    time_sys.real = time_sum(time_sys_1.real, time_sys_2.real);
    time_sys.user = time_sum(time_sys_1.user, time_sys_2.user);
    time_sys.sys  = time_sum(time_sys_1.sys, time_sys_2.sys);

    return time_sys;
}

int64_t time_system_convstr(time_system_t time_sys, char *buff, int64_t size) {
    time_sys = time_system_nor(time_sys);

    int64_t count = 0;

    count += scnprintf(&buff[count], size - count, "real: ");
    count += time_convstr(time_sys.real, &buff[count], size - count);
    count += scnprintf(&buff[count], size - count, "\n");
    count += scnprintf(&buff[count], size - count, "user: ");
    count += time_convstr(time_sys.user, &buff[count], size - count);
    count += scnprintf(&buff[count], size - count, "\n");
    count += scnprintf(&buff[count], size - count, "sys:  ");
    count += time_convstr(time_sys.sys, &buff[count], size - count);

    return count;
}
