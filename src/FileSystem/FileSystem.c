#include "fs.h"
#include "../HAL/GPIO-HAL.h"
#include "../IO-Handler/IO-Handler.h"


int mos_fs_write_GPIO (int GPIO_Number, int IO, int State) {
    //TODO properly init HAL in bootfile?
    initHAL();
    mos_io_handler_write(GPIO_Number, IO, State);
    return 0;
}

//create files with fixed sizes as it is easier to allocate
//as we do not have to worry about growing files and running out of space between blocks (if we have blocks at all...)
int mos_fs_create(const char* file_name, file_ext_type file_type) {

    //TODO read file type from filename

    switch (file_type) {
        case LED:
            //create LED-sized file
            break;
        case BTN:
            //create BUTTON-sized file
            break;
        case TMR:
            //create TIMER-sized file
            break;
        default:
            //throw error
            break;
    }

    return 0;
}

int mos_fs_open(const char* file_name) {

    return 0;
}

int mos_fs_close(int file_descriptor) {

    return 0;
}

#define BUFFER_SIZE 512
char buffer[BUFFER_SIZE];

int mos_fs_read(int file_descriptor) {
    //read from file
    int bytes_read;
    int bytes_written;
    /*
    do {
        read bytes from I/O
        bytes_read = mos_io_read(file_descriptor, buffer, BUFFER_SIZE);

        write to console as default?
        bytes_written = write(file_descriptor, buffer, bytes_read);
    } while (bytes_read <= 0);
    */
    return 0;
}

int mos_fs_write(int file_descriptor, const char* data) {

    return 0;
}
