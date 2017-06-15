#include <stdio.h>
#include "../lib/mos_sys_lib.h"
#include "../lib/mos_std_lib.h"
#include "ps.h"

#define SIZE 500
static char procs[SIZE];
void ps(void)
{

    getProcs(procs, SIZE);
    print(procs);

    exitProc(0);

}
