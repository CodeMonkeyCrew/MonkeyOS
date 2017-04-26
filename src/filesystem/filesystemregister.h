#ifndef SRC_FILESYSTEM_FILESYSTEMREGISTER_H_
#define SRC_FILESYSTEM_FILESYSTEMREGISTER_H_

#include "src/drivers/drivertypes.h"

int register_driver(file_types_t type, driver_t* driver);


#endif /* SRC_FILESYSTEM_FILESYSTEMREGISTER_H_ */
