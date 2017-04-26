#ifndef KERNEL_FILESYSTEM_FILESYSTEM_H_
#define KERNEL_FILESYSTEM_FILESYSTEM_H_

#include "../../kernel/drivers/drivertypes.h"
#include "../../kernel/filesystem/filetypes.h"

#define MAX_NO_OF_FILES 256
#define MAX_NO_OF_OPEN_FILES 32


/* File Descriptors explained:
 * https://linuxmeerkat.wordpress.com/2011/12/02/file-descriptors-explained/
 * http://stackoverflow.com/questions/5256599/what-are-file-descriptors-explained-in-simple-terms
 */

int mos_fs_init(void);

//actual system calls
//int mos_fs_create (const char* file_name, file_ext_type file_type);
int mos_fs_open (const char* file_name);
int mos_fs_close (int file_descriptor);

int mos_fs_read (int file_descriptor, const void* buf, int buffer_size);
int mos_fs_write (int file_descriptor, const void* buf, int buffer_size);

int register_driver(file_types_t type, driver_t* driver);

//util
static int create_entry(generic_file_t* file);
int add_new_file(generic_file_t* file);

#endif /* KERNEL_FILESYSTEM_FILESYSTEM_H_ */
