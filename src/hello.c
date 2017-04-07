#include <src/filesystem/filesystem.h>

#pragma SWI_ALIAS(put, 0);
int put(int value);

int putISR(int value) {
    return 4711;
}

void main(void) {
	mos_fs_write_GPIO(149, 1, 1);
	int res = put(10);
}
