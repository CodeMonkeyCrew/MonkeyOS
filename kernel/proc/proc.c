#include "proc.h"
char* proc_stateName(ProcState_t state){
    switch (state){
    case PROC_STATE_INVALID:
        return "INVALID";
    case PROC_STATE_READY:
        return "READY";
    case PROC_STATE_RUNNING:
        return "RUNNING";
    case PROC_STATE_NEW:
        return "NEW";
    case PROC_STATE_BLOCKED:
        return "BLOCKED";
    case PROC_STATE_EXIT:
        return "EXIT";
    }
    return "StateNotValid";
}


