#ifndef KERNEL_FILESYSTEM_FILESYSTEM_H_
#define KERNEL_FILESYSTEM_FILESYSTEM_H_

#include "../../kernel/drivers/drivertypes.h"
#include "filetypes/filetypes.h"
#include "filesystemregister.h"

#define MAX_NO_OF_FILES 256
#define MAX_NO_OF_OPEN_FILES 32

/* File Descriptors explained:
 * https://linuxmeerkat.wordpress.com/2011/12/02/file-descriptors-explained/
 * http://stackoverflow.com/questions/5256599/what-are-file-descriptors-explained-in-simple-terms
 */

/*
 * Initializes the file system. Should be called at boot time.
 */
int mos_fs_init(void);

//actual system calls
/*
 * Opens a file with the given file_name. Returns a file descriptor (int >= 0) if such a file is found.
 * Returns -1 if the file is already open.
 * Returns -2 if the file was not found.
 * Returns -3 if there are too many open files.
 */
int mos_fs_open (const char* file_name);

/*
 * Closes the file with the corresponding file_descriptor.
 */
int mos_fs_close (int file_descriptor);

/*
 * Calls the read function of the driver, which is responsible for handling given file (determined by file_descriptor).
 */
int mos_fs_read (int file_descriptor, void* buf, int buffer_size);

/*
 * Calls the write function of the driver, which is responsible for handling given file (determined by file_descriptor).
 * Returns -1 if the file_descriptor is not valid.
 */
int mos_fs_write (int file_descriptor, const void* buf, int buffer_size);


//util

/*
 * Tries to add the given Fileindex to the open_files array.
 */
static int create_entry(int fileIndex);


/*
 * Adds a file to the files-array
 */
static int add_new_file(generic_file_t* file);

/*
 * Searches for a file with the given file_descriptor and returns the file.
 */
generic_file_t* fs_get_open_file(int file_descriptor);

#endif /* KERNEL_FILESYSTEM_FILESYSTEM_H_ */
