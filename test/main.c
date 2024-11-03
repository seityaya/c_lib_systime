#include "yaya_systime.h"

#include "stdio.h"

int main(void) {
    const int BUFF_SIZE       = 500;
    char      buff[BUFF_SIZE];

    {
        printf("fragment:\n");

        yaya_time_fragment_t time_fr = {0};

        time_fr = yaya_time_build(0, 0, 0, 0);
        yaya_time_convstr(time_fr, buff, BUFF_SIZE);
        printf("+0 +0    +0    +0    ==> %s\n", buff);

        time_fr = yaya_time_build(1, 100, 100, 100);
        yaya_time_convstr(time_fr, buff, BUFF_SIZE);
        printf("+1 +100  +100  +100  ==> %s\n", buff);

        time_fr = yaya_time_build(1, 500, 500, 500);
        yaya_time_convstr(time_fr, buff, BUFF_SIZE);
        printf("+1 +500  +500  +500  ==> %s\n", buff);

        time_fr = yaya_time_build(1, 1000, 1000, 1000);
        yaya_time_convstr(time_fr, buff, BUFF_SIZE);
        printf("+1 +1000 +1000 +1000 ==> %s\n", buff);

        time_fr = yaya_time_build(-0, -0, -0, -0);
        yaya_time_convstr(time_fr, buff, BUFF_SIZE);
        printf("-0 -0    -0    -0    ==> %s\n", buff);

        time_fr = yaya_time_build(-1, -100, -100, -100);
        yaya_time_convstr(time_fr, buff, BUFF_SIZE);
        printf("-1 -100  -100  -100  ==> %s\n", buff);

        time_fr = yaya_time_build(-1, -500, -500, -500);
        yaya_time_convstr(time_fr, buff, BUFF_SIZE);
        printf("-1 -500  -500  -500  ==> %s\n", buff);

        time_fr = yaya_time_build(-1, -1000, -1000, -1000);
        yaya_time_convstr(time_fr, buff, BUFF_SIZE);
        printf("-1 -1000 -1000 -1000 ==> %s\n", buff);

        time_fr = yaya_time_build(1, -1000, 0, 0);
        yaya_time_convstr(time_fr, buff, BUFF_SIZE);
        printf("+1 -1000 +0    +0    ==> %s\n", buff);

        time_fr = yaya_time_build(1, -1000, 0, +900);
        yaya_time_convstr(time_fr, buff, BUFF_SIZE);
        printf("+1 -1000 +0    +900  ==> %s\n", buff);

        time_fr = yaya_time_build(1, -1000, 0, -900);
        yaya_time_convstr(time_fr, buff, BUFF_SIZE);
        printf("+1 -1000 +0    -900  ==> %s\n", buff);

        time_fr = yaya_time_build(0, 0, 0, +1000000000);
        yaya_time_convstr(time_fr, buff, BUFF_SIZE);
        printf("+0 +0 +0 +1000000000 ==> %s\n", buff);

        time_fr = yaya_time_build(0, 0, 0, -1000000000);
        yaya_time_convstr(time_fr, buff, BUFF_SIZE);
        printf("+0 +0 +0 -1000000000 ==> %s\n", buff);

        time_fr = yaya_time_build(-1, +900, -900, +900);
        yaya_time_convstr(time_fr, buff, BUFF_SIZE);
        printf("-1 +900  -900  +900  ==> %s\n", buff);

        time_fr = yaya_time_build(+1, -900, +900, -900);
        yaya_time_convstr(time_fr, buff, BUFF_SIZE);
        printf("+1 -900  +900  -900  ==> %s\n", buff);

        time_fr = yaya_time_build(-1, +1100, -1100, +1100);
        yaya_time_convstr(time_fr, buff, BUFF_SIZE);
        printf("-1 +1100 -1100 +1100 ==> %s\n", buff);

        time_fr = yaya_time_build(+1, -1100, +1100, -1100);
        yaya_time_convstr(time_fr, buff, BUFF_SIZE);
        printf("+1 -1100 +1100 -1100 ==> %s\n", buff);
    }

    printf("\n");

    {
        printf("op:\n");

        yaya_time_fragment_t time_fr_1 = {0};
        yaya_time_fragment_t time_fr_2 = {0};
        yaya_time_fragment_t time_fr_r = {0};

        time_fr_1 = yaya_time_build(1, -1000, 0, +900);
        yaya_time_convstr(time_fr_1, buff, BUFF_SIZE);
        printf("fr1    %s\n", buff);

        time_fr_2 = yaya_time_build(1, -1000, 0, -900);
        yaya_time_convstr(time_fr_2, buff, BUFF_SIZE);
        printf("fr2    %s\n", buff);

        time_fr_r = yaya_time_sum(time_fr_1, time_fr_2);
        yaya_time_convstr(time_fr_r, buff, BUFF_SIZE);
        printf("sum 12 %s\n", buff);

        time_fr_r = yaya_time_sum(time_fr_2, time_fr_1);
        yaya_time_convstr(time_fr_r, buff, BUFF_SIZE);
        printf("sum 21 %s\n", buff);

        time_fr_r = yaya_time_dif(time_fr_1, time_fr_2);
        yaya_time_convstr(time_fr_r, buff, BUFF_SIZE);
        printf("dif 12 %s\n", buff);

        time_fr_r = yaya_time_dif(time_fr_2, time_fr_1);
        yaya_time_convstr(time_fr_r, buff, BUFF_SIZE);
        printf("dif 21 %s\n", buff);
    }

    printf("\n");
    {
        printf("sys:\n");
        yaya_time_sys_t time_sys = {0};
        yaya_time_sys_convstr(time_sys, buff, BUFF_SIZE);
        printf("%s\n", buff);
    }

    printf("\n");
    {
        printf("resolution:\n");

        yaya_time_fragment_t time = {0};

        time = yaya_time_get(YAYA_TIME_FRAGMENT_TYPE_RESOLUTION, YAYA_TIME_CLOCK_TYPE_REALTIME);
        yaya_time_convstr(time, buff, BUFF_SIZE);
        printf("REALTIME  %s\n", buff);

        time = yaya_time_get(YAYA_TIME_FRAGMENT_TYPE_RESOLUTION, YAYA_TIME_CLOCK_TYPE_MONOTONIC);
        yaya_time_convstr(time, buff, BUFF_SIZE);
        printf("MONOTONIC %s\n", buff);
    }

    printf("\n");
    {
        printf("realtime:\n");

        yaya_time_fragment_t time_1 = {0};
        yaya_time_fragment_t time_2 = {0};
        yaya_time_fragment_t time_3 = {0};

        time_1 = yaya_time_get(YAYA_TIME_FRAGMENT_TYPE_REAL, YAYA_TIME_CLOCK_TYPE_REALTIME);
        time_2 = yaya_time_get(YAYA_TIME_FRAGMENT_TYPE_REAL, YAYA_TIME_CLOCK_TYPE_REALTIME);

        yaya_time_convstr(time_1, buff, BUFF_SIZE);
        printf("REALTIME 1 %s\n", buff);

        yaya_time_convstr(time_2, buff, BUFF_SIZE);
        printf("REALTIME 2 %s\n", buff);

        time_3 = yaya_time_get(YAYA_TIME_FRAGMENT_TYPE_REAL, YAYA_TIME_CLOCK_TYPE_MONOTONIC);
        yaya_time_convstr(time_3, buff, BUFF_SIZE);
        printf("MONOTONIC  %s\n", buff);

        time_3 = yaya_time_dif(time_2, time_1);
        yaya_time_convstr(time_3, buff, BUFF_SIZE);
        printf("dif 21     %s\n", buff);
    }

    printf("\n");
    {
        printf("usertime:\n");

        yaya_time_fragment_t time_1 = {0};
        yaya_time_fragment_t time_2 = {0};
        yaya_time_fragment_t time_3 = {0};

        time_1 = yaya_time_get(YAYA_TIME_FRAGMENT_TYPE_USER, YAYA_TIME_CLOCK_TYPE_REALTIME);
        time_2 = yaya_time_get(YAYA_TIME_FRAGMENT_TYPE_USER, YAYA_TIME_CLOCK_TYPE_REALTIME);

        yaya_time_convstr(time_1, buff, BUFF_SIZE);
        printf("USERTIME 1 %s\n", buff);

        yaya_time_convstr(time_2, buff, BUFF_SIZE);
        printf("USERTIME 2 %s\n", buff);

        time_3 = yaya_time_get(YAYA_TIME_FRAGMENT_TYPE_USER, YAYA_TIME_CLOCK_TYPE_MONOTONIC);
        yaya_time_convstr(time_3, buff, BUFF_SIZE);
        printf("MONOTONIC  %s\n", buff);

        time_3 = yaya_time_dif(time_2, time_1);
        yaya_time_convstr(time_3, buff, BUFF_SIZE);
        printf("dif 21     %s\n", buff);
    }

    printf("\n");
    {
        printf("systime:\n");

        yaya_time_fragment_t time_1 = {0};
        yaya_time_fragment_t time_2 = {0};
        yaya_time_fragment_t time_3 = {0};

        time_1 = yaya_time_get(YAYA_TIME_FRAGMENT_TYPE_SYS, YAYA_TIME_CLOCK_TYPE_REALTIME);
        time_2 = yaya_time_get(YAYA_TIME_FRAGMENT_TYPE_SYS, YAYA_TIME_CLOCK_TYPE_REALTIME);

        yaya_time_convstr(time_1, buff, BUFF_SIZE);
        printf("USERTIME 1 %s\n", buff);

        yaya_time_convstr(time_2, buff, BUFF_SIZE);
        printf("USERTIME 2 %s\n", buff);

        time_3 = yaya_time_get(YAYA_TIME_FRAGMENT_TYPE_SYS, YAYA_TIME_CLOCK_TYPE_MONOTONIC);
        yaya_time_convstr(time_3, buff, BUFF_SIZE);
        printf("MONOTONIC  %s\n", buff);

        time_3 = yaya_time_dif(time_2, time_1);
        yaya_time_convstr(time_3, buff, BUFF_SIZE);
        printf("dif 21     %s\n", buff);
    }

    printf("\n");
    {
        printf("sleep:\n");

        yaya_time_fragment_t time_sleep = {0};
        yaya_time_sys_t      sys_time_1 = {0};
        yaya_time_sys_t      sys_time_2 = {0};
        yaya_time_sys_t      sys_time_3 = {0};

        time_sleep = yaya_time_build(1, 111, 0, 0);
        yaya_time_convstr(time_sleep, buff, BUFF_SIZE);
        printf("sleep %s\n\n", buff);

        sys_time_1 = yaya_time_sys_get(YAYA_TIME_CLOCK_TYPE_REALTIME);
        yaya_time_sys_convstr(sys_time_1, buff, BUFF_SIZE);
        printf("SYSTIME 1 \n%s\n\n", buff);

        yaya_time_sleep(time_sleep, YAYA_TIME_CLOCK_TYPE_REALTIME);

        sys_time_2 = yaya_time_sys_get(YAYA_TIME_CLOCK_TYPE_REALTIME);
        yaya_time_sys_convstr(sys_time_2, buff, BUFF_SIZE);
        printf("SYSTIME 2 \n%s\n\n", buff);

        sys_time_3 = yaya_time_sys_dif(sys_time_2, sys_time_1);
        yaya_time_sys_convstr(sys_time_3, buff, BUFF_SIZE);
        printf("dif 21    \n%s\n\n", buff);
    }

    printf("\n");
    {
        printf("delay:\n");

        yaya_time_fragment_t time_sleep = {0};
        yaya_time_sys_t      sys_time_1 = {0};
        yaya_time_sys_t      sys_time_2 = {0};
        yaya_time_sys_t      sys_time_3 = {0};

        time_sleep = yaya_time_build(1, 111, 0, 0);
        yaya_time_convstr(time_sleep, buff, BUFF_SIZE);
        printf("sleep %s\n\n", buff);

        sys_time_1 = yaya_time_sys_get(YAYA_TIME_CLOCK_TYPE_REALTIME);
        yaya_time_sys_convstr(sys_time_1, buff, BUFF_SIZE);
        printf("SYSTIME 1 \n%s\n\n", buff);

        yaya_time_delay(time_sleep, YAYA_TIME_CLOCK_TYPE_REALTIME);

        sys_time_2 = yaya_time_sys_get(YAYA_TIME_CLOCK_TYPE_REALTIME);
        yaya_time_sys_convstr(sys_time_2, buff, BUFF_SIZE);
        printf("SYSTIME 2 \n%s\n\n", buff);

        sys_time_3 = yaya_time_sys_dif(sys_time_2, sys_time_1);
        yaya_time_sys_convstr(sys_time_3, buff, BUFF_SIZE);
        printf("dif 21    \n%s\n\n", buff);
    }

    return 0;
}
