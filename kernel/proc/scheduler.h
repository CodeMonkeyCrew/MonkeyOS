#ifndef SCHEDULER__H
#define SCHEDULER__H

#include "proc.h"

void scheduler_init(void);
void scheduler_run(void);
void scheduler_start(void);
int scheduler_initProc(ProcEntryPoint_t entryPoint, Priority_t priority);

#endif
