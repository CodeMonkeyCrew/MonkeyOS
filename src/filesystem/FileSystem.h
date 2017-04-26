#ifndef SRC_FILESYSTEM_FILESYSTEM_H_
#define SRC_FILESYSTEM_FILESYSTEM_H_

#include "FileTypes.h"

#define MAX_NO_OF_FILES 256
#define MAX_NO_OF_OPEN_FILES 32


/* File Descriptors explained:
 * https://linuxmeerkat.wordpress.com/2011/12/02/file-descriptors-explained/
 * http://stackoverflow.com/questions/5256599/what-are-file-descriptors-explained-in-simple-terms
 */

//for testing purposes...
int mos_fs_write_GPIO (int GPIO_Number, int IO, int State);

//actual system calls
//int mos_fs_create (const char* file_name, file_ext_type file_type);
int mos_fs_open (const char* file_name);
int mos_fs_close (int file_descriptor);

int mos_fs_read (int file_descriptor, const void* buf, int buffer_size);
int mos_fs_write (int file_descriptor, const void* buf, int buffer_size);


//drivers
typedef struct {
    int (*driver_write) (const void*, int);
    int (*driver_read) (const void*, int);
} driver_type;

//size depending on number of elements in file_ext_type enum in FileTypes.h!
driver_type* drivers[NO_OF_DRIVER_TYPES];

int register_driver(driver_type_enum type, driver_type* driver);


//util
static int create_entry(generic_file_type* file);
int add_new_file(generic_file_type* file);

#endif /* SRC_FILESYSTEM_FILESYSTEM_H_ */
