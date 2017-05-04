#ifndef SRC_FILESYSTEM_FILESYSTEM_H_
#define SRC_FILESYSTEM_FILESYSTEM_H_

#include "filetypes.h"
#include "../drivers/drivertypes.h"

#define MAX_NO_OF_FILES 256
#define MAX_NO_OF_OPEN_FILES 32


/* File Descriptors explained:
 * https://linuxmeerkat.wordpress.com/2011/12/02/file-descriptors-explained/
 * http://stackoverflow.com/questions/5256599/what-are-file-descriptors-explained-in-simple-terms
 */

int mos_fs_init(void);

//actual system calls
generic_file_t* mos_fs_create (file_types_t file_type);
int mos_fs_open (const char* file_name);
int mos_fs_close (int file_descriptor);

int mos_fs_read (int file_descriptor, const void* buf, int buffer_size);
int mos_fs_write (int file_descriptor, const void* buf, int buffer_size);

int register_driver(file_types_t type, driver_t* driver);

//util
static int create_entry(int fileIndex);
static int add_new_file(generic_file_t* file);
static generic_file_t* get_open_file(int file_descriptor);

#endif /* SRC_FILESYSTEM_FILESYSTEM_H_ */
