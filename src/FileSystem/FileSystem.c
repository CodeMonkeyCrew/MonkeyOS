#include <src/FileSystem/FileSystem.h>
#include <string.h>
#include "../HAL/GPIO-HAL.h"
#include "../IO-Handler/IO-Handler.h"

static int currentFileNo = 0;
static generic_file_type* files[MAX_NO_OF_FILES];
static generic_file_type* open_files[MAX_NO_OF_OPEN_FILES];


fs_init() {
    //initialize open-/files arrays
    int i = 0;
    while (i < MAX_NO_OF_FILES) {
        generic_file_type file;
        generic_file_type* pFile = &file;
        strcpy(pFile -> name, "dummyFile");
        pFile -> d_type = GPIO;
        pFile -> is_open = false;
        pFile -> size = 0;

        files[i] = pFile;

        if (i < MAX_NO_OF_OPEN_FILES) {
            open_files[i] = pFile;
        }
    }

    //init driver array
    driver_type dummyDriver;
    driver_type* pDummyDriver = &dummyDriver;
    i = 0;
    while (i < NO_OF_DRIVER_TYPES) {
        drivers[i] = pDummyDriver;
    }

    gpio_file_type gpio149;
    gpio_file_type* pGpio149 = &gpio149;
    strcpy(pGpio149 -> header.name, "gpio149");
    pGpio149 -> header.is_open = false;
    pGpio149 -> header.d_type = GPIO;
    pGpio149 -> header.size = 0;
    pGpio149 -> number = 149;

    add_new_file((generic_file_type*)pGpio149);
}

int add_new_file(generic_file_type* file) {
    files[currentFileNo] = file;
    ++currentFileNo;
    return 0;
}

//func for first testing ..
int mos_fs_write_GPIO (int GPIO_Number, int IO, int State) {
    initHAL();
    mos_io_handler_write(GPIO_Number, IO, State);
    return 0;
}

/*
int mos_fs_create(const char* file_name, file_ext_type file_type) {
    return 0;
}
*/

int mos_fs_open(const char* file_name) {
    int i = 0;

    while (i < MAX_NO_OF_FILES) {
        if (files[i] -> name == file_name) {
            //open file only once for now..
            if (files[i]->is_open) {
                return -1;
            }
            return create_entry(files[i]); //returns the file descriptor
        }
    }
    //file not found
    return -2;
}

/* Tries to add the given File to the open_files array.
 * Input: File that should be added to the array
 * Return: -3 if no free entry could be found or the index at which it was inserted
 */
static int create_entry(generic_file_type* pFile) {
    int i = 0;
    while (i < MAX_NO_OF_OPEN_FILES) {
        if (!open_files[i] -> is_open) {
            pFile -> is_open = true;
            open_files[i] = pFile;
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
    if (file_descriptor > 0 && file_descriptor <= MAX_NO_OF_OPEN_FILES) {
        generic_file_type* pFile = open_files[file_descriptor];
        return drivers[pFile -> d_type] -> driver_write(buf, buffer_size);
    }
    //no valid file_descriptor
    return -1;
}

int register_driver(driver_type_enum type, driver_type* pDriver) {
    drivers[type] = pDriver;
    return 0;
}

