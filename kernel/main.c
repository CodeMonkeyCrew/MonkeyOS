#include <inttypes.h>
#include "proc/proc.h"
#include "proc/scheduler.h"
#include "proc/mode.h"
#include <stdio.h>

#define PM_PWSTCTRL_PER (volatile uint32_t*)0x483070E0

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

void main(void)
{
    *PM_PWSTCTRL_PER |= ((1 << 0) | (1 << 1));

    mos_fs_init();


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




