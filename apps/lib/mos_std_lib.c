#include "mos_sys_lib.h"
#include "mos_std_lib.h"
#include <string.h>


void print(char* str){
    write(1, str, strlen(str));
}
