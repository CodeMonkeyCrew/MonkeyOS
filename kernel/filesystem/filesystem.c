#include <kernel/filesystem/filesystem.h>
#include "filesystemregister.h"
#include <stdlib.h>

static int currentFileNo = 0;
static generic_file_t* files[MAX_NO_OF_FILES];
static int open_files[MAX_NO_OF_OPEN_FILES];

//size depending on number of elements in file_types_t enum in generictype.h!
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

/*
 * Checks if a new file is valid and sets basic fields.
 */
static generic_file_t* check_if_file_is_valid(generic_file_t* pFile, file_types_t file_type) {
    if (pFile != NULL) {
        if (add_new_file(pFile) >= 0) {
            pFile->f_type = file_type;
            pFile->is_open = false;
            pFile->size = 0;
            return pFile;
        } else {
            free(pFile);
        }
    }
    return NULL;
}

generic_file_t* mos_fs_create_file(file_types_t file_type) {

    generic_file_t* pNewFile;

    switch (file_type) {
        case GPIO_DIR:
            pNewFile = (generic_file_t*)malloc(sizeof(gpio_direction_file_t));
            break;
        case GPIO_VAL:
            pNewFile = (generic_file_t*)malloc(sizeof(gpio_value_file_t));
            break;
        case TIMER:
            pNewFile = (generic_file_t*)malloc(sizeof(timer_file_t));
            break;
        case TIMER_INT:
            pNewFile = (generic_file_t*)malloc(sizeof(timer_interrupt_file_t));
            break;
        case TIMER_MODE:
            pNewFile = (generic_file_t*)malloc(sizeof(timer_mode_file_t));
            break;
        case UART:
            pNewFile = (generic_file_t*)malloc(sizeof(uart_file_t));
            break;
    }
    return check_if_file_is_valid(pNewFile, file_type);
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
    generic_file_t* pFile = get_open_file(file_descriptor);
    if (pFile != NULL) {
        pFile->is_open = false;
    }
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
