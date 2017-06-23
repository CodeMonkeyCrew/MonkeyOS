#ifndef KERNEL_FILESYSTEM_FILETYPES_DATATYPE_H_
#define KERNEL_FILESYSTEM_FILETYPES_DATATYPE_H_

#include "generictype.h"
#include <inttypes.h>

typedef struct {
    generic_file_t header;
    uint8_t* data;
} data_file_t;

#endif /* KERNEL_FILESYSTEM_FILETYPES_DATATYPE_H_ */
