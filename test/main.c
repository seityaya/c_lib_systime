#include "yaya_systime.h"

#include "stdio.h"

#define BUFF_SIZE 500

#define CONSTANT_NANOS INT64_C(1000000000)
#define CONSTANT_MICRO INT64_C(1000000)
#define CONSTANT_MILLI INT64_C(1000)

static yaya_systime_float_t yaya_time_fragment_convflt__(yaya_time_fragment_t time_fragment) {
#define float_result(m, c) ((m) != 0 ? (yaya_systime_float_t)(((yaya_systime_float_t)(1.0) / (yaya_systime_float_t)(c)) * (yaya_systime_float_t)(m)) : 0)
    // clang-format off
    yaya_systime_float_t time_float =
        (yaya_systime_float_t)(time_fragment.second) +
        float_result(time_fragment.millis, CONSTANT_MILLI) +
        float_result(time_fragment.micros, CONSTANT_MICRO) +
        float_result(time_fragment.nanos , CONSTANT_NANOS);
    // clang-format on
#undef C
    return time_float;
}

int main(void) {
    yaya_time_fragment_t time_fr = {0};

    char buff[BUFF_SIZE];

    printf("fragment:\n");

    time_fr = yaya_time_fragment_build(0, 0, 0, 0);
    yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
    printf("+0 +0    +0    +0    ==> %s %+25.9f \n", buff, yaya_time_fragment_convflt__(time_fr));

    time_fr = yaya_time_fragment_build(1, 100, 100, 100);
    yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
    printf("+1 +100  +100  +100  ==> %s %+25.9f \n", buff, yaya_time_fragment_convflt__(time_fr));

    time_fr = yaya_time_fragment_build(1, 500, 500, 500);
    yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
    printf("+1 +500  +500  +500  ==> %s %+25.9f \n", buff, yaya_time_fragment_convflt__(time_fr));

    time_fr = yaya_time_fragment_build(1, 1000, 1000, 1000);
    yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
    printf("+1 +1000 +1000 +1000 ==> %s %+25.9f \n", buff, yaya_time_fragment_convflt__(time_fr));

    time_fr = yaya_time_fragment_build(-0, -0, -0, -0);
    yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
    printf("-0 -0    -0    -0    ==> %s %+25.9f \n", buff, yaya_time_fragment_convflt__(time_fr));

    time_fr = yaya_time_fragment_build(-1, -100, -100, -100);
    yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
    printf("-1 -100  -100  -100  ==> %s %+25.9f \n", buff, yaya_time_fragment_convflt__(time_fr));

    time_fr = yaya_time_fragment_build(-1, -500, -500, -500);
    yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
    printf("-1 -500  -500  -500  ==> %s %+25.9f \n", buff, yaya_time_fragment_convflt__(time_fr));

    time_fr = yaya_time_fragment_build(-1, -1000, -1000, -1000);
    yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
    printf("-1 -1000 -1000 -1000 ==> %s %+25.9f \n", buff, yaya_time_fragment_convflt__(time_fr));

    time_fr = yaya_time_fragment_build(1, -1000, 0, 0);
    yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
    printf("+1 -1000 +0    +0    ==> %s %+25.9f \n", buff, yaya_time_fragment_convflt__(time_fr));

    time_fr = yaya_time_fragment_build(1, -1000, 0, +900);
    yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
    printf("+1 -1000 +0    +900  ==> %s %+25.9f \n", buff, yaya_time_fragment_convflt__(time_fr));

    time_fr = yaya_time_fragment_build(1, -1000, 0, -900);
    yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
    printf("+1 -1000 +0    -900  ==> %s %+25.9f \n", buff, yaya_time_fragment_convflt__(time_fr));

    printf("\n");
    printf("sys:\n");
    yaya_time_sys_t time_sys = {0};
    yaya_time_sys_convstr(time_sys, buff, BUFF_SIZE);
    printf("%s\n", buff);

    return 0;
}
