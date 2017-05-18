#ifndef KERNEL_FILESYSTEM_FILETYPES_UARTTYPE_H_
#define KERNEL_FILESYSTEM_FILETYPES_UARTTYPE_H_


#include "generictype.h"
#include <inttypes.h>

typedef struct {
    generic_file_t header;
    int uartNumber;
} uart_file_t;


#endif /* KERNEL_FILESYSTEM_FILETYPES_UARTTYPE_H_ */
