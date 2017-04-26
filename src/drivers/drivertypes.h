#ifndef SRC_DRIVERS_DRIVERTYPES_H_
#define SRC_DRIVERS_DRIVERTYPES_H_

#include "src/FileSystem/FileTypes.h"

typedef struct {
    int (*driver_write) (const void*, int, generic_file_t*);
    int (*driver_read) (const void*, int, generic_file_t*);
    int (*driver_open) (generic_file_t*);
} driver_t;


#endif /* SRC_DRIVERS_DRIVERTYPES_H_ */
