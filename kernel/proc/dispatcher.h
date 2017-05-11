#ifndef DISPATCHER__H
#define DISPATCHER__H

#include "proc.h"

void dispatcher_switchContext(ProcContext_t* oldContext, ProcContext_t* newContext);

#endif
