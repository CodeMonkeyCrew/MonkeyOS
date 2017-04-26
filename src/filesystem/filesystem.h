#ifndef SRC_FILESYSTEM_FILESYSTEM_H_
#define SRC_FILESYSTEM_FILESYSTEM_H_

#include "filesystemregister.h"
#include "filetypes.h"
#include "src/Drivers/DriverTypes.h"

#define MAX_NO_OF_FILES 64
#define MAX_NO_OF_OPEN_FILES 32


/* File Descriptors explained:
 * https://linuxmeerkat.wordpress.com/2011/12/02/file-descriptors-explained/
 * http://stackoverflow.com/questions/5256599/what-are-file-descriptors-explained-in-simple-terms
 */

int mos_fs_init(void);

//for testing purposes...
int mos_fs_write_GPIO (int GPIO_Number, int IO, int State);

//actual system calls
//int mos_fs_create (const char* file_name, file_ext_type file_type);
int mos_fs_open (const char* file_name);
int mos_fs_close (int file_descriptor);

int mos_fs_read (int file_descriptor, const void* buf, int buffer_size);
int mos_fs_write (int file_descriptor, const void* buf, int buffer_size);



//util
static int create_entry(generic_file_t* file);
int add_new_file(generic_file_t* file);

#endif /* SRC_FILESYSTEM_FILESYSTEM_H_ */
