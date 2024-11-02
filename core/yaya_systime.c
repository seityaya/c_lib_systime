#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/times.h>
#include <time.h>

#include "yaya_systime.h"

#define CONSTANT_NANOS INT64_C(1000000000)
#define CONSTANT_MICRO INT64_C(1000000)
#define CONSTANT_MILLI INT64_C(1000)

#define CONSTANT_MIN_FRACTIONAL INT64_C(0)
#define CONSTANT_MAX_FRACTIONAL INT64_C(999)
#define CONSTANT_DIV_FRACTIONAL INT64_C(1000)

static int64_t scnprintf(char *buf, int64_t size, const char *fmt, ...) {
    va_list args;
    int     i;
    va_start(args, fmt);
    i = vsnprintf(buf, size, fmt, args);
    va_end(args);
    return (i >= size) ? size : i;
}

/*==================================================================================================================================================*/

yaya_time_fragment_t yaya_time_fragment_get(yaya_time_fragment_type_e type) {
    yaya_time_fragment_t time_fragment = {0};
    (void)(type);
    return yaya_time_fragment_nor(time_fragment);
}

void yaya_time_fragment_delay(yaya_time_fragment_t time_fragment, yaya_time_clockid_type_e clockid) {
    (void)(time_fragment);
    (void)(clockid);
}

void yaya_time_fragment_sleep(yaya_time_fragment_t time_fragment, yaya_time_clockid_type_e clockid) {
    (void)(time_fragment);
    (void)(clockid);
}

static yaya_time_fragment_t set_sign(yaya_time_fragment_t time_fragment) {
    if (time_fragment.signum != 0) {
        time_fragment.nanos  *= time_fragment.signum;
        time_fragment.micros *= time_fragment.signum;
        time_fragment.millis *= time_fragment.signum;
        time_fragment.second *= time_fragment.signum;
        time_fragment.signum  = 0;
    }
    return time_fragment;
}

static void counter(int64_t *m, int64_t *M) {
    if (*m > CONSTANT_MAX_FRACTIONAL) {
        int64_t k  = *m / CONSTANT_DIV_FRACTIONAL;
        *M        += k;
        *m         = *m % CONSTANT_DIV_FRACTIONAL;
    }
    if (*m < CONSTANT_MIN_FRACTIONAL) {
        int64_t k  = *m / CONSTANT_DIV_FRACTIONAL;
        *M        += k;
        *m         = *m % CONSTANT_DIV_FRACTIONAL;
    }
}

static void signums(int64_t *m, int64_t *s) {
    if (*s == 0) {
        *s = +1;
    }
    if (*m > 0) {
        *m *= +1;
        *s  = +1;
    }
    if (*m < 0) {
        *m *= -1;
        *s  = -1;
    }
}

yaya_time_fragment_t yaya_time_fragment_nor(yaya_time_fragment_t time_fragment) {
    time_fragment = set_sign(time_fragment);

    counter(&time_fragment.nanos, &time_fragment.micros);
    counter(&time_fragment.micros, &time_fragment.millis);
    counter(&time_fragment.millis, &time_fragment.second);

    signums(&time_fragment.nanos, &time_fragment.signum);
    signums(&time_fragment.micros, &time_fragment.signum);
    signums(&time_fragment.millis, &time_fragment.signum);
    signums(&time_fragment.second, &time_fragment.signum);

    return time_fragment;
}

yaya_time_fragment_t yaya_time_fragment_dif(yaya_time_fragment_t time_fragment_1, yaya_time_fragment_t time_fragment_2) {
    yaya_time_fragment_t time_fragment = {0};

    time_fragment_1      = set_sign(time_fragment_1);
    time_fragment_2      = set_sign(time_fragment_2);
    time_fragment.nanos  = time_fragment_1.nanos - time_fragment_2.nanos;
    time_fragment.micros = time_fragment_1.micros - time_fragment_2.micros;
    time_fragment.millis = time_fragment_1.millis - time_fragment_2.millis;
    time_fragment.second = time_fragment_1.second - time_fragment_2.second;
    time_fragment.signum = 0;

    return yaya_time_fragment_nor(time_fragment);
}

yaya_time_fragment_t yaya_time_fragment_sum(yaya_time_fragment_t time_fragment_1, yaya_time_fragment_t time_fragment_2) {
    yaya_time_fragment_t time_fragment = {0};

    time_fragment_1      = set_sign(time_fragment_1);
    time_fragment_2      = set_sign(time_fragment_2);
    time_fragment.nanos  = time_fragment_1.nanos + time_fragment_2.nanos;
    time_fragment.micros = time_fragment_1.micros + time_fragment_2.micros;
    time_fragment.millis = time_fragment_1.millis + time_fragment_2.millis;
    time_fragment.second = time_fragment_1.second + time_fragment_2.second;
    time_fragment.signum = 0;

    return yaya_time_fragment_nor(time_fragment);
}

/*==================================================================================================================================================*/

yaya_time_fragment_t yaya_time_fragment_build(int64_t second, int64_t millis, int64_t micros, int64_t nanos) {
    yaya_time_fragment_t time_fragment = {0};

    time_fragment.nanos  = nanos;
    time_fragment.micros = micros;
    time_fragment.millis = millis;
    time_fragment.second = second;
    time_fragment.signum = 0;

    return yaya_time_fragment_nor(time_fragment);
}

static yaya_systime_float_t inversefactor(int64_t number, int64_t factor) {
    if (number != 0) {
        return ((yaya_systime_float_t)(1) / (yaya_systime_float_t)(factor)) * (yaya_systime_float_t)(number);
    }
    return 0;
}

yaya_systime_float_t yaya_time_fragment_convflt(yaya_time_fragment_t time_fragment) {
    time_fragment = yaya_time_fragment_nor(time_fragment);
    // clang-format off
    yaya_systime_float_t time_float =
            (yaya_systime_float_t)(time_fragment.signum) *
                 ((yaya_systime_float_t)(time_fragment.second)                 +
                           inversefactor(time_fragment.millis, CONSTANT_MILLI) +
                           inversefactor(time_fragment.micros, CONSTANT_MICRO) +
                           inversefactor(time_fragment.nanos , CONSTANT_NANOS));
    // clang-format on
    return time_float;
}

int64_t yaya_time_fragment_convstr(yaya_time_fragment_t time_fragment, char *buff, int64_t size) {
    int64_t count = 0;

    time_fragment = yaya_time_fragment_nor(time_fragment);

    // count += scnprintf(&buff[count], size - count, "[%c] ", time_fragment.signum == +1 ? '+' : time_fragment.signum == -1 ? '-' : '?');

    count += scnprintf(&buff[count], size - count, "| %+4" PRIi64 " sec  ", time_fragment.second * time_fragment.signum);
    count += scnprintf(&buff[count], size - count, "%3" PRIi64 " millis  ", time_fragment.millis);
    count += scnprintf(&buff[count], size - count, "%3" PRIi64 " micros  ", time_fragment.micros);
    count += scnprintf(&buff[count], size - count, "%3" PRIi64 " nanos ", time_fragment.nanos);
    count += scnprintf(&buff[count], size - count, "| %+14.9f fraction second |", yaya_time_fragment_convflt(time_fragment));

    return count;
}

/*==================================================================================================================================================*/

yaya_time_sys_t yaya_time_sys_get(void) {
    yaya_time_sys_t time_sys = {0};

    time_sys.real = yaya_time_fragment_get(YAYA_TIME_FRAGMENT_TYPE_REAL);
    time_sys.user = yaya_time_fragment_get(YAYA_TIME_FRAGMENT_TYPE_USER);
    time_sys.sys  = yaya_time_fragment_get(YAYA_TIME_FRAGMENT_TYPE_SYS);

    return time_sys;
}

yaya_time_sys_t yaya_time_sys_nor(yaya_time_sys_t time_sys) {
    time_sys.real = yaya_time_fragment_nor(time_sys.real);
    time_sys.user = yaya_time_fragment_nor(time_sys.user);
    time_sys.sys  = yaya_time_fragment_nor(time_sys.sys);

    return time_sys;
}

yaya_time_sys_t yaya_time_sys_dif(yaya_time_sys_t time_sys_1, yaya_time_sys_t time_sys_2) {
    yaya_time_sys_t time_sys = {0};

    time_sys.real = yaya_time_fragment_dif(time_sys_1.real, time_sys_2.real);
    time_sys.user = yaya_time_fragment_dif(time_sys_1.user, time_sys_2.user);
    time_sys.sys  = yaya_time_fragment_dif(time_sys_1.sys, time_sys_2.sys);

    return time_sys;
}

yaya_time_sys_t yaya_time_sys_sum(yaya_time_sys_t time_sys_1, yaya_time_sys_t time_sys_2) {
    yaya_time_sys_t time_sys = {0};

    time_sys.real = yaya_time_fragment_sum(time_sys_1.real, time_sys_2.real);
    time_sys.user = yaya_time_fragment_sum(time_sys_1.user, time_sys_2.user);
    time_sys.sys  = yaya_time_fragment_sum(time_sys_1.sys, time_sys_2.sys);

    return time_sys;
}

int64_t yaya_time_sys_convstr(yaya_time_sys_t time_sys, char *buff, int64_t size) {
    int64_t count = 0;

    count += scnprintf(&buff[count], size - count, "real: ");
    count += yaya_time_fragment_convstr(time_sys.real, &buff[count], size - count);
    count += scnprintf(&buff[count], size - count, "\n");
    count += scnprintf(&buff[count], size - count, "user: ");
    count += yaya_time_fragment_convstr(time_sys.user, &buff[count], size - count);
    count += scnprintf(&buff[count], size - count, "\n");
    count += scnprintf(&buff[count], size - count, "sys:  ");
    count += yaya_time_fragment_convstr(time_sys.sys, &buff[count], size - count);
    count += scnprintf(&buff[count], size - count, "\n");

    return count;
}
