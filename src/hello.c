#include <stdio.h>
#include "IO-Handler/IO-Handler.h"
#include "HAL/GPIO-HAL.h"

/*
 * hello.c
 */
void main(void) {
	printf("Hello World!\n");
	initHAL();
	mos_io_handler_write(149, 1, 1);

	
}
