#ifndef SCHEDULER__H
#define SCHEDULER__H

#include "proc.h"

void scheduler_init(void);
void scheduler_run(void);
void scheduler_start(void);

void scheduler_exitProc(int status);
int scheduler_fork(void);
int scheduler_execv(const char *filename, char * const argv[]);
void scheduler_waitPid(int pid);
void scheduler_getProcs(char* procStrings, int size);


#endif
