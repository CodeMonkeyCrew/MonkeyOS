#include <stdio.h>
#include "FileSystem/fs.h"

/*
 * hello.c
 */
void main(void) {
	printf("Hello World!\n");
	
	mos_fs_write_GPIO(149, 1, 1);

}
