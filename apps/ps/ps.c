#include <stdio.h>
#include "../lib/mos_sys_lib.h"
#include "../lib/mos_std_lib.h"
#include "ps.h"

#define SIZE 500
static char procs[SIZE];
void ps(void)
{

    printf("show all processes\n");

    getProcs(procs, SIZE);

  //  FILE* pstdout = stdout;
   // stdout->buf = "hello\n";
    //*stdout->buf = 'h';
    printf("%s\n", procs);
    print(procs);

    exitProc(0);

}
