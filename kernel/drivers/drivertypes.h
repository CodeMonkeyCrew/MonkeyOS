#ifndef KERNEL_DRIVERS_DRIVERTYPES_H_
#define KERNEL_DRIVERS_DRIVERTYPES_H_

#include "../filesystem/filetypes/generictype.h"

typedef struct {
    int (*driver_write) (const void*, int, generic_file_t*);
    int (*driver_read) (void*, int, generic_file_t*);
    int (*driver_open) (generic_file_t*);
} driver_t;


#endif /* KERNEL_DRIVERS_DRIVERTYPES_H_ */
