#include "scheduler.h"
#include "proc.h"
#include "dispatcher.h"
#include "scheduler_timer.h"
#include "../filesystem/filesystem.h"
#include <inttypes.h>
#include <stdio.h>

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
    if (interruptedPid >= 0)
    {
        dispatcher_switchContext(&procs[interruptedPid].context,
                                 &procs[runningPid].context);
    }
}

int scheduler_initProc(ProcEntryPoint_t entryPoint, Priority_t priority)
//add int argc, char* argv as parameter
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
        procs[pid].context.sp = 0x80494000 + ((pid - 1) * 0xFFFFF);
        procs[pid].context.sp = 0x90000000 - ((pid - 1) * 0x100);
        procs[pid].priority = priority;
        return pid;
        //write argc in context.R0 and argv in context.R1 (oder anders rum)
    }

    // failed to initialize process
    return -1;
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
        //TODO: refactor this when MMU stuff done!
        uint32_t stackStartParent = 0x90000000 - ((runningPid - 1) * 0x100);
        uint32_t stackStartChild = 0x90000000 - ((pid - 1) * 0x100);
        uint32_t stackSize = stackStartParent - procs[runningPid].context.sp;
        procs[pid].context.sp = stackStartChild - stackSize;
        memcpy((uint32_t*) procs[pid].context.sp,
               (uint32_t*) procs[runningPid].context.sp, stackSize);
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
        exe_file_t* exeFile = (exe_file_t*) file;
        procs[runningPid].context.restartAddress =
                (uint32_t) exeFile->entryPoint;
        procs[runningPid].context.r0 = (uint32_t) argv;
        mos_fs_close(fd);
        dispatcher_loadContext(&procs[runningPid].context);
    }
    return fd;
}

void scheduler_exitProc(int status)
{
    if (procs[procs[runningPid].parentPid].waitForPid == runningPid)
    {
        procs[runningPid].state = PROC_STATE_INVALID;
        procs[procs[runningPid].parentPid].waitForPid = 0;
        procs[procs[runningPid].parentPid].state = PROC_STATE_READY;
    }else{
        procs[runningPid].state = PROC_STATE_EXIT;
    }
    int interruptedPid = scheduler_runNextProc();
    if (interruptedPid >= 0)
    {
        dispatcher_loadContext(&procs[runningPid].context);
    }
}

void scheduler_waitPid(int pid)
{
    if(procs[pid].state == PROC_STATE_EXIT){
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

void scheduler_getProcs(char* procStrings, int size){
    int i;
    char* str = "pid: %i\t->\tstate: %s\r\n";
    int strSize = 0;
    int remainingSize = size;
    int curPos = 0;
    for(i = 0; i < MAX_PROC_COUNT; i++){
      remainingSize = size - curPos;
      if (remainingSize > 0) {
          strSize = snprintf(NULL, 0, str, procs[i].pid, proc_stateName(procs[i].state));
          if (strSize < remainingSize) {
              if (sprintf(procStrings + curPos, str,  procs[i].pid, proc_stateName(procs[i].state)) > 0) {
                  curPos += strSize;
              }
          }
      }
    }
}


