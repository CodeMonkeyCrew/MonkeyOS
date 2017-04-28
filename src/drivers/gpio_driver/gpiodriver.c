#include "gpiodriver.h"
#include "hal/gpiohal.h"
#include "../drivertypes.h"
#include "../../filesystem/filetypes.h"
#include "../../filesystem/filesystemregister.h"
#include <stdlib.h>


static int mos_gpio_driver_open(generic_file_t* file){

    int number, port, start_bit;
    uint32_t* mux_mode_addr;

    switch(file->f_type) {
            case GPIO_DIR: {
                gpio_direction_file_t* dir_file = (gpio_direction_file_t*)file;
                number = dir_file->gpio_info.number;
                port = dir_file->gpio_info.port;
                start_bit = dir_file->gpio_info.start_bit;
                mux_mode_addr = dir_file->gpio_info.mux_mode_addr;
                break;
            }
            case GPIO_VAL: {
                gpio_value_file_t* val_file = (gpio_value_file_t*)file;
                number = val_file->gpio_info.number;
                port = val_file->gpio_info.port;
                start_bit = val_file->gpio_info.start_bit;
                mux_mode_addr = val_file->gpio_info.mux_mode_addr;
                break;
            }
    }

    mos_gpio_LLD_init(number, port, mux_mode_addr, start_bit);
    return 0;
}

static int mos_gpio_driver_write(const void* buffer, int bufSize, generic_file_t* file){
    //cast generic file type to gpio file
    file_types_t fileType = file->f_type;

    switch(fileType) {
        case GPIO_DIR: {
            //cast to direction file
            gpio_direction_file_t* directionFile = (gpio_direction_file_t*) file;
            int shift = directionFile->gpio_info.shift;
            uint32_t* gpioOE = directionFile->OE;
            mos_gpio_LLD_set_direction(gpioOE, shift, *((int*)buffer));
            break; }
        case GPIO_VAL: {
            //cast to value file
            gpio_value_file_t* valueFile = (gpio_value_file_t*) file;
            int shift = valueFile->gpio_info.shift;
            uint32_t* gpioDataOut = valueFile->data_out;
            mos_gpio_LLD_set_value(gpioDataOut, shift, *((int*)buffer));
            break; }
        default:
            return -1;
    }
    return 0;
}

static int mos_gpio_driver_read(const void* buffer, int bufSize, generic_file_t* file) {

    return 0;
}

//should be called at boot time
void mos_gpio_driver_init(void){
    //use malloc as it would otherwise be put on the stack,
    //which will be freed when the functions ends.
    //new data will then maybe override the driver
    driver_t* gpioDriver = (driver_t*) malloc(sizeof(driver_t));
    gpioDriver->driver_read = mos_gpio_driver_read;
    gpioDriver->driver_write = mos_gpio_driver_write;
    gpioDriver->driver_open = mos_gpio_driver_open;
    register_driver(GPIO_VAL, gpioDriver);
    register_driver(GPIO_DIR, gpioDriver);
}
