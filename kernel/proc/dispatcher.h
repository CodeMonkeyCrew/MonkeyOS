#ifndef DISPATCHER__H
#define DISPATCHER__H

#include "proc.h"

void dispatcher_switchContext(ProcContext_t* oldContext, ProcContext_t* newContext);

void dispatcher_saveContextFork(ProcContext_t* context);

void dispatcher_saveContextWaitPid(ProcContext_t* context);

void dispatcher_loadContext(ProcContext_t* context);

void dispatcher_saveContext(ProcContext_t* context);


#endif
