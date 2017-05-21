#include "timerdriver_test.h"
static void test_GPTimer2(void);

void test_timerdriver(void){
    test_GPTimer2();
}

static void test_GPTimer2(void)
{
    _disable_interrupts();

    mos_fs_init();
    timerdriver_init(); //creates and registers drivers and files

    int fd_timer2_int = mos_fs_open("GPTimer_2_INT");
    mos_fs_write(fd_timer2_int, 0, 0); //enables interrupt

    int fd_timer_2_int = mos_fs_open("GPTimer_2_MODE");
    int compareValue = 0xFFFFF;
    mos_fs_write(fd_timer_2_int, &compareValue, 1); //enable COMPARE mode and set compare-value

    int state = 1;
    int fd_timer2 = mos_fs_open("GPTimer_2");

    _enable_interrupts();
    _enable_IRQ();

    mos_fs_write(fd_timer2, &state, 1); //start gptimer2

    while (1)
    {
    }
}
