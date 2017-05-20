#ifndef KERNEL_FILESYSTEM_FILETYPES_MATRIXTYPE_H_
#define KERNEL_FILESYSTEM_FILETYPES_MATRIXTYPE_H_

#include "generictype.h"
#include <inttypes.h>

typedef struct {
    generic_file_t header;
    uint8_t id;
} MatrixFile_t;

#endif /* KERNEL_FILESYSTEM_FILETYPES_MATRIXTYPE_H_ */
