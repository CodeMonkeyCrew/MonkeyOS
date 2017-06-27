#include "scheduler.h"
#include "proc.h"
#include "dispatcher.h"
#include "loader.h"
#include "scheduler_timer.h"
#include "../filesystem/filesystem.h"
#include "../memorymanagement/mmu.h"
#include <inttypes.h>
#include <stdio.h>

#define MAX_PROC_COUNT 16

#define PROC_IMG_VADDR 0x80494000
#define MAX_PROC_IMG_SIZE 0x100000

// needed to set interrupt status and new interrupt agreement flags
static uint32_t* INTCPS_CONTROL = (uint32_t*) 0x48200048;

static PCB_t procs[MAX_PROC_COUNT];
static uint8_t runningPid;

#pragma DATA_SECTION(procImgBuffer, ".proc_img")
static uint8_t procImgBuffer[MAX_PROC_IMG_SIZE];

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
    do
    {
        pid = (pid + 1) % MAX_PROC_COUNT;
        if (procs[pid].state == PROC_STATE_READY)
        {
            // found next ready process
            return pid;
        }
    }
    while (pid != runningPid);

    // no other ready process
    return runningPid;
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
    // initialize empty PCBs
    uint8_t pid;
    for (pid = 0; pid < MAX_PROC_COUNT; ++pid)
    {
        procs[pid].pid = pid;
        procs[pid].state = PROC_STATE_INVALID;
        procs[pid].priority = PROC_PRIO_MIDDLE;
    }

    // setup idle process
    runningPid = 0;
    procs[runningPid].parentPid = 0;
    procs[runningPid].state = PROC_STATE_RUNNING;
    // enable irq and fiq, set user mode
    procs[runningPid].context.cpsr = 0x10;
    procs[runningPid].priority = PROC_PRIO_LOW;

    // initialize timer
    scheduler_timer_init();
}

void scheduler_start(void)
{
    scheduler_timer_start();
}

void scheduler_run(void)
{
    // clear interrupt status flag
    scheduler_timer_clear_interrupt();
    // set new interrupt agreement flag
    *INTCPS_CONTROL |= (1 << 0);

    int interruptedPid = scheduler_runNextProc();
    if (interruptedPid >= 0 && runningPid != interruptedPid)
    {
        mmu_flush_tlb();
        mmu_flush_cache();
        mmu_load_task_region(runningPid);
        dispatcher_switchContext(&procs[interruptedPid].context,
                                 &procs[runningPid].context);
    }
}

void scheduler_exitProc(int status)
{
    if (procs[procs[runningPid].parentPid].waitForPid == runningPid)
    {
        procs[runningPid].state = PROC_STATE_INVALID;
        procs[procs[runningPid].parentPid].waitForPid = 0;
        procs[procs[runningPid].parentPid].state = PROC_STATE_READY;
    }
    else
    {
        procs[runningPid].state = PROC_STATE_EXIT;
    }
    int interruptedPid = scheduler_runNextProc();
    if (interruptedPid >= 0)
    {
        dispatcher_loadContext(&procs[runningPid].context);
    }
}

int scheduler_fork(void)
{
    int pid = scheduler_getFreeProcSlot();
    if (pid >= 0)
    {
        dispatcher_saveContextFork(&procs[runningPid].context);
        memcpy(&procs[pid], &procs[runningPid], sizeof(PCB_t));

        procs[pid].state = PROC_STATE_READY;
        procs[pid].context.r0 = 0;

        // store process image in buffer
        memcpy(procImgBuffer, (uint8_t*) PROC_IMG_VADDR, MAX_PROC_IMG_SIZE);

        // set pid on mmu
        mmu_load_task_region(procs[pid].pid);

        // load process image from buffer
        memcpy((uint8_t*) PROC_IMG_VADDR, procImgBuffer, MAX_PROC_IMG_SIZE);

        // set pid on mmu
        mmu_load_task_region(procs[runningPid].pid);

        procs[pid].parentPid = runningPid;
        procs[pid].pid = pid;
    }
    return pid;
}

int scheduler_execv(const char *filename, char * const argv[])
{
    int fd = mos_fs_open(filename);
    if (fd >= 0)
    {
        generic_file_t* file = fs_get_open_file(fd);
        data_file_t* exeFile = (data_file_t*) file;
        int isLoaded = loadElf(&procs[runningPid], (Elf32_Ehdr*) exeFile->data);
        mos_fs_close(fd);
        if (isLoaded >= 0)
        {
            procs[runningPid].context.r0 = (uint32_t) argv;
            dispatcher_loadContext(&procs[runningPid].context);
        }
        return isLoaded;
    }
    return fd;
}

void scheduler_waitPid(int pid)
{
    if (procs[pid].state == PROC_STATE_EXIT)
    {
        procs[pid].state = PROC_STATE_INVALID;
        return;
    }
    procs[runningPid].waitForPid = pid;
    procs[runningPid].state = PROC_STATE_BLOCKED;
    int interruptedPid = scheduler_runNextProc();
    if (interruptedPid >= 0)
    {
        dispatcher_saveContextWaitPid(&procs[interruptedPid].context);
        dispatcher_loadContext(&procs[runningPid].context);

    }
}

void scheduler_getProcs(char* procStrings, int size)
{
    int i;
    char* str = "pid: %i\t->\tstate: %s\r\n";
    int strSize = 0;
    int remainingSize = size;
    int curPos = 0;
    for (i = 0; i < MAX_PROC_COUNT; i++)
    {
        remainingSize = size - curPos;
        if (remainingSize > 0)
        {
            strSize = snprintf(NULL, 0, str, procs[i].pid,
                               proc_stateName(procs[i].state));
            if (strSize < remainingSize)
            {
                if (sprintf(procStrings + curPos, str, procs[i].pid,
                            proc_stateName(procs[i].state)) > 0)
                {
                    curPos += strSize;
                }
            }
        }
    }
}

