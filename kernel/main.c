#include <inttypes.h>
#include "proc/proc.h"
#include "proc/scheduler.h"
#include "proc/mode.h"
#include "kernel/drivers/gpio_driver/gpiodriver.h"
#include <kernel/drivers/uart/uartdriver.h>
#include <kernel/drivers/timer_driver/timer_driver.h>
#include <kernel/drivers/matrix_driver/matrix_driver.h>
#include <kernel/drivers/timer_driver/hal/timer_hal.h>
#include <kernel/drivers/util/registerutil.h>
#include <kernel/filesystem/filesystem.h>
#include <stdio.h>

#define PM_PWSTCTRL_PER (volatile uint32_t*)0x483070E0

    char monkey[] = "                 __,__\r\n"
            "        .--.  .-\"     \"-.  .--.\r\n"
            "       / .. \\/  .-. .-.  \\/ .. \\\r\n"
            "      | |  '|  /   Y   \\  |'  | |\r\n"
            "      | |  '|  /   Y   \\  |'  | |\r\n"
            "      | \\   \\  \\ 0 | 0 /  /   / |\r\n"
            "       \\ '- ,\\.-\"`` ``\"-./, -' /\r\n"
            "        `'-' /_   ^ ^   _\\ '-'`\r\n"
            "        .--'|  \\._ _ _./  |'--. \r\n"
            "      /`    \\   \\.-.  /   /    `\\\r\n"
            "     /       '._/  |-' _.'       \\\r\n"
            "    /          ;  /--~'   |       \\\r\n"
            "   /        .'\\|.-\\--.     \\       \\\r\n"
            "   /   .'-. /.-.;\\  |\\|'~'-.|\\       \\\r\n"
            "  \\       -./`|_\\_/      `\\'.      \\\r\n"
            "   '.      ;     ___)        '.`;    /\r\n"
            "     '-.,_ ;     ___)          \\/   /\r\n"
            "      \\   `'------'\\       \\     /\r\n"
            "       '.    \\       '.      |   ;/_\r\n"
            "     ___>     '.       \\_ _ _/   ,  '--.\r\n"
            "   .'   '.   .-~~~~~-. /     |--'`~~-.  \\\r\n"
            "  // / .---'/  .-~~-._/ / / /---..__.'  /\r\n"
            " ((_(_/    /  /      (_(_(_(---.__    .'\r\n"
            "           | |     _              `~~`\r\n"
            "           | |     \\'.\r\n"
            "            \\ '....' |\r\n"
            "             '.,___.'\r\n"
            "\r\n"
            " ____    ____                  __                        ___     ______   \r\n"
            "|_   \\  /   _|                [  |  _                  .'   `. .' ____ \\  \r\n"
            "  |   \\/   |   .--.   _ .--.   | | / ] .---.   _   __ /  .-.  \\| (___ \\_| \r\n"
            "  | |\\  /| | / .'`\\ \\[ .-. |  | '' < / /__\\\\ [ \\ [  ]| |   | | _.____.  \r\n"
            " _| |_\\/_| |_| \\__. | | | | |  | |`\\ \\| \\__.,  \\ '/ / \\  `-'  /| \\____) | \r\n"
            "|_____||_____|'.__.' [___||__][__|  \\_]'.__.'[\\_:  /   `.___.'  \\______.' \r\n"
            "                                              \\__.'                       \r\n";

void testFromFSToDrivers();
void testTimerFromFS();

int dir_fd;
int val_fd;
int val_1 = 1;
void* pVal_1 = &val_1;

int val_0 = 0;
void* pVal_0 = &val_0;

void process1()
{
    volatile int i = 0;
    while (1)
    {
        printf("process 1: %i \n", ++i);
    }
}

void process2()
{
    volatile int i = 0;
    while (1)
    {
        printf("process 2: %i \n", ++i);
    }
}

void matrixDriverTest(void);

void main(void)
{


//    *PM_PWSTCTRL_PER |= ((1 << 0) | (1 << 1));
//
//    /*Blink LED*/
//    mos_fs_init();
//    mos_gpio_driver_init();
//
//    dir_fd = mos_fs_open("gpio149_dir");
//    val_fd = mos_fs_open("gpio149_val");
//    mos_fs_write(dir_fd, pVal_1, 1);
//
//    /*Scheduler*/
//    scheduler_init();
//    scheduler_initProc(process1, PROC_PRIO_MIDDLE);
//    //scheduler_initProc(process2, PROC_PRIO_MIDDLE);
//    scheduler_start();
//    // set user mode and enable interrupts
//    mode_setUserMode();
//
//    // idle loop
//    while (1)
//    {
//        printf("idle loop\n");
//    }

    matrixDriverTest();

    *PM_PWSTCTRL_PER |= ((1 << 0) | (1 << 1));

    /*Blink LED*/
    mos_fs_init();
    //gpiodriver_init();

    dir_fd = mos_fs_open("gpio149_dir");
    val_fd = mos_fs_open("gpio149_val");
    mos_fs_write(dir_fd, pVal_1, 1);

    /*Scheduler*/
    scheduler_init();
    scheduler_initProc(process1, PROC_PRIO_MIDDLE);
    //scheduler_initProc(process2, PROC_PRIO_MIDDLE);
    scheduler_start();
    // set user mode and enable interrupts
    mode_setUserMode();

    // idle loop
    while (1)
    {
        printf("idle loop\n");
    }

}

void testFromFSToDrivers()
{
    mos_fs_init();
    //gpiodriver_init();

    int dir_fd = mos_fs_open("gpio130_dir");
    int val_fd = mos_fs_open("gpio130_val");

    int val_1 = 1;
    void* pVal_1 = &val_1;

    mos_fs_write(dir_fd, pVal_1, 1);

    int val_0 = 0;
    void* pVal_0 = &val_0;

    volatile int i;
    while (1)
    {
        mos_fs_write(val_fd, pVal_1, 1);
        for (i = 0; i < 200000; i++)
        {

        }
        mos_fs_write(val_fd, pVal_0, 1);
        for (i = 0; i < 200000; i++)
        {
        }
    }
}

//void testTimer()
//{
//    _disable_interrupts();
//    *PM_PWSTCTRL_PER |= ((1 << 0) | (1 << 1));
//
//    init_gptimer(2);
//    enable_compare_mode(2, 0, 0xfffff);
//    enable_interrupt(2);
//
//    _enable_interrupts();
//    _enable_IRQ();
//    gptimer_start(2);
//    while (1)
//    {
//
//    }
//}

void testTimerFromFS()
{
    _disable_interrupts();
    *PM_PWSTCTRL_PER |= ((1 << 0) | (1 << 1));

    mos_fs_init();
    mos_timer_driver_init(); //creates and registers drivers and files

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

#define BUFFER_SIZE 512
static uint8_t buffer[BUFFER_SIZE];

void matrixDriverTest(void) {
    mos_fs_init();
    gpiodriver_init();
    matrix_driver_init();

    int matrix0Fd = mos_fs_open("matrix0");

    int i;
    for(i = 0; i < BUFFER_SIZE; ++i) {
        buffer[i] = 0xFF;
    }

    mos_fs_write(matrix0Fd, buffer, BUFFER_SIZE);

    matrix_driver_update();
    matrix_driver_update();

    while(1) {}
}
void testUARTDriverFromFS()
{
    mos_fs_init();
    uartdriver_init();

    int uart_fd = mos_fs_open("uart3");

    char writeSign[1] = ">";
    mos_fs_write(uart_fd, monkey, strlen(monkey));
    mos_fs_write(uart_fd, writeSign, 1);

    char readBuff[50];
    while(1){
        mos_fs_read(uart_fd, readBuff, 50);
        volatile int i;

    }
}
