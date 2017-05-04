#include "../drivertypes.h"
#include "gpiodriver.h"
#include "hal/gpiohal.h"
#include "../../filesystem/filesystemregister.h"
#include "../../filesystem/filetypes/gpiotypes.h"
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

static void add_drivers();
static void add_files();

//should be called at boot time
void mos_gpio_driver_init(void){

    add_drivers();
    add_files();
}

static void add_drivers() {
    //use malloc as it would otherwise be put on the stack,
    //which will be freed when the functions ends.
    //new data will then maybe override the driver
    driver_t* pGpioDriver = (driver_t*) malloc(sizeof(driver_t));
    if (pGpioDriver != NULL) {
        pGpioDriver->driver_read = mos_gpio_driver_read;
        pGpioDriver->driver_write = mos_gpio_driver_write;
        pGpioDriver->driver_open = mos_gpio_driver_open;
        register_driver(GPIO_VAL, pGpioDriver);
        register_driver(GPIO_DIR, pGpioDriver);
    }
}

static void add_gpio_149();

static void add_files() {

    add_gpio_149();

}

static void add_gpio_149() {
    gpio_direction_file_t* pGpio149_dir = (gpio_direction_file_t*) mos_fs_create_file(GPIO_DIR);
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

    gpio_value_file_t* pGpio149_val = (gpio_value_file_t*) mos_fs_create_file(GPIO_VAL);
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
}
