#include <apps/echo/echo.h>
#include <stdio.h>
#include "../lib/mos_sys_lib.h"
#include "../lib/mos_std_lib.h"

void echo(char* text[])
{

    print(text[0]);
    print("\r\n");

    exitProc(0);
}
