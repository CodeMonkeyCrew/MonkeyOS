#include "scheduler.h"
#include "proc.h"
#include "dispatcher.h"
#include "scheduler_timer.h"
#include <inttypes.h>

#define MAX_PROC_COUNT 16

// TODO: change this as soon as the timer is in place
// needed to set interrupt status and new interrupt agreement flags
static uint32_t* INTCPS_CONTROL = (uint32_t*) 0x48200048;

static PCB_t procs[MAX_PROC_COUNT];
static uint8_t runningPid;

static int scheduler_getFreeProcSlot(void)
{
    uint8_t pid;
    for (pid = 0; pid < MAX_PROC_COUNT; ++pid)
    {
        if (procs[pid].state == PROC_STATE_INVALID)
        {
            // found free process slot
            return pid;
        }
    }

    // no free process slot
    return -1;
}

static uint8_t scheduler_findNextProc(void)
{
    uint8_t pid = runningPid;
    uint8_t next_pid = INVALID_ID_THREAD;
    do
    {
        pid = (pid + 1) % MAX_PROC_COUNT;
        if (procs[pid].state == PROC_STATE_READY)
        {
            if (next_pid == INVALID_ID_THREAD)
            {
                next_pid = pid;
            }
            else
            {
                if (procs[pid].priority < procs[next_pid].priority)
                {
                    next_pid = pid;
                }
            }
        }
    }
    while (pid != runningPid);

    if (next_pid == INVALID_ID_THREAD)
    {
        return runningPid;
    }
    else if ((procs[runningPid].priority < procs[next_pid].priority)
            && ((procs[runningPid].state == PROC_STATE_RUNNING)
                    || (procs[runningPid].state == PROC_STATE_RUNNING)))
    {
        return runningPid;
    }
    //return next process
    return next_pid;
}

static int scheduler_runNextProc(void)
{
    uint8_t pid = scheduler_findNextProc();

    switch (procs[pid].state)
    {
    case PROC_STATE_RUNNING:
        // already running
        break;

    case PROC_STATE_READY:
        if (pid != runningPid)
        {
            uint8_t interruptedPid = runningPid;
            // otherwise it could be dead or blocked
            if (procs[interruptedPid].state == PROC_STATE_RUNNING)
            {
                procs[interruptedPid].state = PROC_STATE_READY;
            }

            runningPid = pid;
            procs[runningPid].state = PROC_STATE_RUNNING;

            // switch context
            return interruptedPid;
        }
        break;

    default:
        break;
    }

    // resume current process
    return -1;
}

void scheduler_init(void)
{
    //init scheduler
    scheduler_timer_init();
    // initialize empty PCBs
    uint8_t pid;
    for (pid = 0; pid < MAX_PROC_COUNT; ++pid)
    {
        procs[pid].pid = pid;
        procs[pid].state = PROC_STATE_INVALID;
    }

    // setup idle process
    runningPid = 0;
    procs[runningPid].parentPid = 0;
    procs[runningPid].state = PROC_STATE_RUNNING;
    procs[runningPid].priority = PROC_PRIO_LOW;
}

void scheduler_start(void)
{
    scheduler_timer_start();
}
void scheduler_run(void)
{

    // clear interrupt status flag
    schedulter_timer_clear_interrupt();
    // set new interrupt agreement flag
    *INTCPS_CONTROL |= (1 << 0);

    int interruptedPid = scheduler_runNextProc();
    if (interruptedPid >= 0)
    {
        dispatcher_switchContext(&procs[interruptedPid].context,
                                 &procs[runningPid].context);
    }
}

int scheduler_initProc(ProcEntryPoint_t entryPoint, Priority_t priority)
{
    int pid = scheduler_getFreeProcSlot();
    if (pid > 0)
    {
        procs[pid].parentPid = runningPid;
        procs[pid].state = PROC_STATE_READY;
        // enable irq and fiq, set user mode
        procs[pid].context.cpsr = 0x10;
        procs[pid].context.restartAddress = (uint32_t) entryPoint;
        // TODO: set proper stack pointer
        procs[pid].context.sp = 0x90000000 + ((pid - 1) * 0xFFFF);
        procs[pid].priority = priority;
        return pid;
    }

    // failed to initialize process
    return -1;
}
