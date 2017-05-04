#include <kernel/filesystem/filesystem.h>
#include <string.h>
#include <stdlib.h>

static int currentFileNo = 0;
static generic_file_t* files[MAX_NO_OF_FILES];
static int open_files[MAX_NO_OF_OPEN_FILES];

//size depending on number of elements in file_types_t enum in fileTypes.h!
static driver_t* drivers[NO_OF_FILE_TYPES];

int mos_fs_init(void) {
    int i;

    //-1 to signal that there is no file mapped yet
    for (i = 0; i < MAX_NO_OF_OPEN_FILES; ++i) {
        open_files[i] = -1;
    }

    //init driver array
    driver_t dummyDriver;
    driver_t* pDummyDriver = &dummyDriver;
    for (i = 0; i < NO_OF_FILE_TYPES; ++i) {
        drivers[i] = pDummyDriver;
    }

    gpio_direction_file_t* pGpio149_dir = (gpio_direction_file_t*) mos_fs_create(GPIO_DIR);
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
    }

    gpio_value_file_t* pGpio149_val = (gpio_value_file_t*) mos_fs_create(GPIO_VAL);
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
    }

    return 0;
}

static int add_new_file(generic_file_t* file) {
    if (currentFileNo < MAX_NO_OF_FILES) {
        files[currentFileNo++] = file;
    } else {
        return -1;
    }
    return 0;
}

generic_file_t* mos_fs_create(file_types_t file_type) {

    generic_file_t* pNew_file;

    switch (file_type) {
        case GPIO_DIR:
            pNew_file = (generic_file_t*)malloc(sizeof(gpio_direction_file_t));
            if (pNew_file != NULL) {
                if (add_new_file(pNew_file) >= 0) {
                    return pNew_file;
                } else {
                    free(pNew_file);
                }
            }
            break;

        case GPIO_VAL:
            pNew_file = (generic_file_t*)malloc(sizeof(gpio_value_file_t));
            if (pNew_file != NULL) {
                if (add_new_file(pNew_file) >= 0) {
                    return pNew_file;
                } else {
                    free(pNew_file);
                }
            }
            break;
    }
    return NULL;
}


int mos_fs_open(const char* file_name) {
    int i;
    for (i = 0; i < currentFileNo; ++i) {
        if (strcmp(files[i]->name, file_name) == 0) {
            //open file only once for now..
            if (files[i]->is_open) {
                return -1;
            }
            int openFileIndex = create_entry(i); //returns the file descriptor; -3 if there is no space for another file
            if (openFileIndex >= 0) {
                drivers[files[i]->f_type]->driver_open(files[i]);
            }
            return openFileIndex;
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
        if (open_files[i] == -1 || !files[open_files[i]]->is_open) {
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
    generic_file_t* pFile = get_open_file(file_descriptor);
    if (pFile != NULL) {
        return drivers[pFile->f_type]->driver_write(buf, buffer_size, pFile);
    }
    //no valid file_descriptor
    return -1;
}

int register_driver(file_types_t f_type, driver_t* pDriver) {
    drivers[f_type] = pDriver;
    return 0;
}

static generic_file_t* get_open_file(int file_descriptor) {
    if (file_descriptor >= 0 && file_descriptor <= MAX_NO_OF_OPEN_FILES) {
        generic_file_t* pFile = files[open_files[file_descriptor]];
        if (pFile != NULL) {
            return pFile;
        }
    }
    return NULL;
}
