#ifndef KERNEL_FILESYSTEM_FILETYPES_STDTYPE_H_
#define KERNEL_FILESYSTEM_FILETYPES_STDTYPE_H_

#include "generictype.h"
#include <inttypes.h>

typedef struct {
    generic_file_t header;
    char* buffer;
}std_file_t;



#endif /* KERNEL_FILESYSTEM_FILETYPES_STDTYPE_H_ */
