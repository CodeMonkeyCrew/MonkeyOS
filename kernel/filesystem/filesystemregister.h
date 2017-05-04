#ifndef KERNEL_FILESYSTEM_FILESYSTEMREGISTER_H_
#define KERNEL_FILESYSTEM_FILESYSTEMREGISTER_H_

#include "../../kernel/drivers/drivertypes.h"
#include <string.h>

/*
 * Registers a driver to the given file type. There can only be one
 * driver to a file type.
 */
int register_driver(file_types_t type, driver_t* driver);

/*
 * creates (allocates memory if possible) a file of the given file type and registers it in filesystem.
 */
generic_file_t* mos_fs_create_file(file_types_t file_type);

#endif /* KERNEL_FILESYSTEM_FILESYSTEMREGISTER_H_ */
