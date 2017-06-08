#include "test.h"
#include <stdio.h>
#include "../lib/mos_sys_lib.h"
void test(char* text[])
{

    printf(text[0]);
    printf("\n");

    exitProc(0);
}
