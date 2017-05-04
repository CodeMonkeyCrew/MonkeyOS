#ifndef KERNEL_FILESYSTEM_FILESYSTEMREGISTER_H_
#define KERNEL_FILESYSTEM_FILESYSTEMREGISTER_H_

#include "../../kernel/drivers/drivertypes.h"

int register_driver(file_types_t type, driver_t* driver);


#endif /* KERNEL_FILESYSTEM_FILESYSTEMREGISTER_H_ */
