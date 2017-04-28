#include "filesystem.h"
#include <string.h>
#include <stdlib.h>

static int currentFileNo = 0;
static generic_file_t* files[MAX_NO_OF_FILES];
static int open_files[MAX_NO_OF_OPEN_FILES];

//size depending on number of elements in file_types_t enum in fileTypes.h!
static driver_t* drivers[NO_OF_FILE_TYPES];

int mos_fs_init(void) {
    int i;

    for (i = 0; i < MAX_NO_OF_OPEN_FILES; ++i) {
        open_files[i] = -1;
    }

    //init driver array
    driver_t dummyDriver;
    driver_t* pDummyDriver = &dummyDriver;
    for (i = 0; i < NO_OF_FILE_TYPES; ++i) {
        drivers[i] = pDummyDriver;
    }

    gpio_direction_file_t* pGpio149_dir = (gpio_direction_file_t*) malloc(sizeof(gpio_direction_file_t));
    if (pGpio149_dir != NULL) {
       strcpy(pGpio149_dir->header.name, "gpio149_dir");
       pGpio149_dir->header.is_open = false;
       pGpio149_dir->header.f_type = GPIO_DIR;
       pGpio149_dir->header.size = 0;

       pGpio149_dir->gpio_info.number = 149;
       pGpio149_dir->gpio_info.port = 5;
       pGpio149_dir->gpio_info.start_bit = 16;
       pGpio149_dir->gpio_info.shift = 21;
       pGpio149_dir->gpio_info.mux_mode_addr = (uint32_t*) 0x4800217C;

       pGpio149_dir->OE = (uint32_t*) 0x49056034;
       add_new_file((generic_file_t*)pGpio149_dir);
    }

    gpio_value_file_t* pGpio149_val = (gpio_value_file_t*) malloc(sizeof(gpio_value_file_t));
    if (pGpio149_val != NULL) {
       strcpy(pGpio149_val->header.name, "gpio149_val");
       pGpio149_val->header.is_open = false;
       pGpio149_val->header.f_type = GPIO_VAL;
       pGpio149_val->header.size = 0;

       pGpio149_val->gpio_info.number = 149;
       pGpio149_val->gpio_info.port = 5;
       pGpio149_val->gpio_info.start_bit = 16;
       pGpio149_val->gpio_info.shift = 21;
       pGpio149_val->gpio_info.mux_mode_addr = (uint32_t*) 0x4800217C;

       pGpio149_val->data_out = (uint32_t*) 0x49056094;
       add_new_file((generic_file_t*)pGpio149_val);
    }
    return 0;
}

int add_new_file(generic_file_t* file) {
    if (currentFileNo < MAX_NO_OF_FILES) {
        files[currentFileNo++] = file;
    }
    return 0;
}

/*
int mos_fs_create(const char* file_name, file_ext_type file_type) {
    return 0;
}
*/

int mos_fs_open(const char* file_name) {
    int i;
    for (i = 0; i < currentFileNo; ++i) {
        if (strcmp(files[i]->name, file_name) == 0) {
            //open file only once for now..
            if (files[i]->is_open) {
                return -1;
            }
            drivers[files[i]->f_type]->driver_open(files[i]);

            int openFileIndex = create_entry(i); //returns the file descriptor
            if (openFileIndex >= 0) {
                return openFileIndex;
            }
        }
    }
    //file not found
    return -2;
}

/* Tries to add the given File to the open_files array.
 * Input: File that should be added to the array
 * Return: -3 if no free entry could be found or the index at which it was inserted
 */
static int create_entry(int fileIndex) {
    int i;
    for (i = 0; i < MAX_NO_OF_OPEN_FILES; ++i) {

        if (open_files[i] == -1 || !files[open_files[i]])->is_open) {
            open_files[i] = fileIndex;
            files[open_files[i]]->is_open = true;
            return i;
        }
    }
    //no free entry found
    return -3;
}

int mos_fs_close(int file_descriptor) {
    return 0;
}

int mos_fs_read(int file_descriptor, const void* buf, int buffer_size) {
    return 0;
}

int mos_fs_write(int file_descriptor, const void* buf, int buffer_size) {
    if (file_descriptor >= 0 && file_descriptor <= MAX_NO_OF_OPEN_FILES) {
        generic_file_t* pFile = open_files[file_descriptor];
        return drivers[pFile->f_type]->driver_write(buf, buffer_size, pFile);
    }
    //no valid file_descriptor
    return -1;
}

int register_driver(file_types_t f_type, driver_t* pDriver) {
    drivers[f_type] = pDriver;
    return 0;
}
