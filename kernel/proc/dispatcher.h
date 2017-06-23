#ifndef DISPATCHER__H
#define DISPATCHER__H

#include "proc.h"

void dispatcher_switchContext(ProcContext_t* oldContext, ProcContext_t* newContext, int newPID);

void dispatcher_saveContextFork(ProcContext_t* context);

void dispatcher_saveContextWaitPid(ProcContext_t* context);

int dispatcher_loadContext(ProcContext_t* context);

void dispatcher_storeContext(ProcContext_t* context);


#endif
