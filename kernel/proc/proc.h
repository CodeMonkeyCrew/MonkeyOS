#ifndef PROC__H
#define PROC__H

#include <inttypes.h>

typedef enum {
    PROC_STATE_INVALID,
    PROC_STATE_NEW,
    PROC_STATE_READY,
    PROC_STATE_RUNNING,
    PROC_STATE_BLOCKED,
    PROC_STATE_EXIT
} ProcState_t;

typedef struct {
    uint32_t cpsr;
    uint32_t restartAddress;
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12;
    uint32_t sp;
    uint32_t lr;
} ProcContext_t;

typedef void (*ProcEntryPoint_t)(void);

typedef struct {
    uint8_t pid;
    uint8_t parentPid;
    ProcState_t state;
    ProcContext_t context;
    ProcEntryPoint_t entryPoint;
} PCB_t;

#endif