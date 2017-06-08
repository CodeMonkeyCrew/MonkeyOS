#ifndef KERNEL_FILESYSTEM_FILETYPES_EXETYPE_H_
#define KERNEL_FILESYSTEM_FILETYPES_EXETYPE_H_

#include "generictype.h"
#include "../../proc/proc.h"
#include <inttypes.h>

typedef struct {
    generic_file_t header;
    ProcEntryPoint_t entryPoint;
} exe_file_t;




#endif /* KERNEL_FILESYSTEM_FILETYPES_EXETYPE_H_ */
