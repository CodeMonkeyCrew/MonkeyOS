#include "filesystem.h"
#include <string.h>

static int currentFileNo = 0;
static generic_file_t* files[MAX_NO_OF_FILES];
static generic_file_t* open_files[MAX_NO_OF_OPEN_FILES];

//size depending on number of elements in file_types_t enum in FileTypes.h!
static driver_t* drivers[NO_OF_FILE_TYPES];

int mos_fs_init(void) {
    //initialize open-/files arrays
    int i;
    for (i = 0; i < MAX_NO_OF_FILES; ++i) {

        gpio_direction_file_t dummy_file;

        gpio_info_t gpio_dummy;
        gpio_info_t* pGpio_dummy = &gpio_dummy;
        pGpio_dummy -> number = 149;
        pGpio_dummy -> port = 5;
        pGpio_dummy -> start_bit = 16;
        pGpio_dummy -> shift = 21;
        pGpio_dummy -> mux_mode_addr = (uint32_t*) 0x4800217C;

        generic_file_t file;
        generic_file_t* pFile = &file;
        char fname[16] = "dummyFile";
        strcpy(pFile -> name, fname);
        pFile -> f_type = GPIO_VAL;
        pFile -> is_open = false;
        pFile -> size = 0;

        dummy_file.header = file;
        dummy_file.gpio_info = pGpio_dummy;
        dummy_file.OE = (uint32_t*)0x12345678;

        files[i] = (generic_file_t*)&dummy_file;


        if (i < MAX_NO_OF_OPEN_FILES) {
            open_files[i] = (generic_file_t*)&dummy_file;
        }
    }

    //init driver array
    driver_t dummyDriver;
    driver_t* pDummyDriver = &dummyDriver;
    for (i = 0; i < NO_OF_FILE_TYPES; ++i) {
        drivers[i] = pDummyDriver;
    }

    gpio_info_t gpio149;
    gpio_info_t* pGpio149 = &gpio149;
    pGpio149 -> number = 149;
    pGpio149 -> port = 5;
    pGpio149 -> start_bit = 16;
    pGpio149 -> shift = 21;
    pGpio149 -> mux_mode_addr = (uint32_t*) 0x4800217C;


    generic_file_t gpio149_dir_header;
    strcpy(gpio149_dir_header.name, "gpio149_dir\0");
    gpio149_dir_header.is_open = false;
    gpio149_dir_header.f_type = GPIO_DIR;
    gpio149_dir_header.size = 0;

    gpio_direction_file_t gpio149_dir;
    gpio149_dir.header = gpio149_dir_header;
    gpio149_dir.gpio_info = pGpio149;
    gpio149_dir.OE = (uint32_t*) 0x49056034;

    generic_file_t* test = (generic_file_t*)&gpio149_dir;
    add_new_file(test);


    generic_file_t gpio149_val_header;
    strcpy(gpio149_val_header.name, "gpio149_val\0");
    gpio149_val_header.is_open = false;
    gpio149_val_header.f_type = GPIO_VAL;
    gpio149_val_header.size = 0;

    gpio_value_file_t gpio149_val;
    gpio149_val.header = gpio149_val_header;
    gpio149_val.gpio_info = pGpio149;
    gpio149_val.data_out = (uint32_t*)0x49056094;

    add_new_file((generic_file_t*)&gpio149_val);
    return 0;
}

int add_new_file(generic_file_t* file) {
    files[currentFileNo] = file;
    ++currentFileNo;
    return 0;
}

/*
int mos_fs_create(const char* file_name, file_ext_type file_type) {
    return 0;
}
*/

int mos_fs_open(const char* file_name) {
    int i;
    for (i = 0; i < MAX_NO_OF_FILES; ++i) {
        if (strcmp(files[i] -> name, file_name) == 0) {
            //open file only once for now..
            if (files[i]->is_open) {
                return -1;
            }

            drivers[files[i]->f_type]->driver_open(files[i]);
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
static int create_entry(generic_file_t* pFile) {
    int i;
    for (i = 0; i < MAX_NO_OF_OPEN_FILES; ++i) {
        if (open_files[i] -> is_open == false) {
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
