#include "yaya_systime.h"

#include "stdio.h"

#define BUFF_SIZE 500
char buff[BUFF_SIZE];

#define CONSTANT_NANOS INT64_C(1000000000)
#define CONSTANT_MICRO INT64_C(1000000)
#define CONSTANT_MILLI INT64_C(1000)

int main(void) {
    {
        printf("fragment:\n");

        yaya_time_fragment_t time_fr = {0};

        time_fr = yaya_time_fragment_build(0, 0, 0, 0);
        yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
        printf("+0 +0    +0    +0    ==> %s\n", buff);

        time_fr = yaya_time_fragment_build(1, 100, 100, 100);
        yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
        printf("+1 +100  +100  +100  ==> %s\n", buff);

        time_fr = yaya_time_fragment_build(1, 500, 500, 500);
        yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
        printf("+1 +500  +500  +500  ==> %s\n", buff);

        time_fr = yaya_time_fragment_build(1, 1000, 1000, 1000);
        yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
        printf("+1 +1000 +1000 +1000 ==> %s\n", buff);

        time_fr = yaya_time_fragment_build(-0, -0, -0, -0);
        yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
        printf("-0 -0    -0    -0    ==> %s\n", buff);

        time_fr = yaya_time_fragment_build(-1, -100, -100, -100);
        yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
        printf("-1 -100  -100  -100  ==> %s\n", buff);

        time_fr = yaya_time_fragment_build(-1, -500, -500, -500);
        yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
        printf("-1 -500  -500  -500  ==> %s\n", buff);

        time_fr = yaya_time_fragment_build(-1, -1000, -1000, -1000);
        yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
        printf("-1 -1000 -1000 -1000 ==> %s\n", buff);

        time_fr = yaya_time_fragment_build(1, -1000, 0, 0);
        yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
        printf("+1 -1000 +0    +0    ==> %s\n", buff);

        time_fr = yaya_time_fragment_build(1, -1000, 0, +900);
        yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
        printf("+1 -1000 +0    +900  ==> %s\n", buff);

        time_fr = yaya_time_fragment_build(1, -1000, 0, -900);
        yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
        printf("+1 -1000 +0    -900  ==> %s\n", buff);

        time_fr = yaya_time_fragment_build(0, 0, 0, +1000000000);
        yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
        printf("+0 +0 +0 +1000000000 ==> %s\n", buff);

        time_fr = yaya_time_fragment_build(0, 0, 0, -1000000000);
        yaya_time_fragment_convstr(time_fr, buff, BUFF_SIZE);
        printf("+0 +0 +0 -1000000000 ==> %s\n", buff);
    }

    printf("\n");

    {
        printf("op:\n");

        yaya_time_fragment_t time_fr_1 = {0};
        yaya_time_fragment_t time_fr_2 = {0};
        yaya_time_fragment_t time_fr_r = {0};

        time_fr_1 = yaya_time_fragment_build(1, -1000, 0, +900);
        yaya_time_fragment_convstr(time_fr_1, buff, BUFF_SIZE);
        printf("fr1    %s\n", buff);

        time_fr_2 = yaya_time_fragment_build(1, -1000, 0, -900);
        yaya_time_fragment_convstr(time_fr_2, buff, BUFF_SIZE);
        printf("fr2    %s\n", buff);

        time_fr_r = yaya_time_fragment_sum(time_fr_1, time_fr_2);
        yaya_time_fragment_convstr(time_fr_r, buff, BUFF_SIZE);
        printf("sum 12 %s\n", buff);

        time_fr_r = yaya_time_fragment_sum(time_fr_2, time_fr_1);
        yaya_time_fragment_convstr(time_fr_r, buff, BUFF_SIZE);
        printf("sum 21 %s\n", buff);

        time_fr_r = yaya_time_fragment_dif(time_fr_1, time_fr_2);
        yaya_time_fragment_convstr(time_fr_r, buff, BUFF_SIZE);
        printf("dif 12 %s\n", buff);

        time_fr_r = yaya_time_fragment_dif(time_fr_2, time_fr_1);
        yaya_time_fragment_convstr(time_fr_r, buff, BUFF_SIZE);
        printf("dif 21 %s\n", buff);
    }

    printf("\n");
    {
        printf("sys:\n");

        yaya_time_sys_t time_sys = {0};

        yaya_time_sys_convstr(time_sys, buff, BUFF_SIZE);

        printf("%s\n", buff);
    }

    return 0;
}
