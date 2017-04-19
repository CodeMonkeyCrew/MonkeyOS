
#include <src/drivers/gpio-driver/lowLevelGPIO/gpio-LLDriver.h>
//include FileTypes
//include fileSystem for register function


//should be called at boot time
void mos_gpio_driver_init(void){
    driver_type gpioDriver;
    gpioDriver.driver_read = mos_gpio_driver_read;
    gpioDriver.driver_write = mos_gpio_driver_write;
    gpioDriver.driver_open = mos_gpio_driver_open;
    register_driver(gpio_file_type, gpioDriver);
}
int mos_gpio_driver_open(generic_file_type* file){
    gpio_file_type* gpioFile = (gpio_file_type*) file;
    mos_gpio_LLD_init(gpioFile->gpioPinNumber, gpioFile->gpioPort, gpioFile->muxModeAddr, gpioFile->startBit);
    return 0;
}

int mos_gpio_driver_write(const void* buffer, int bufSize, generic_file_type* file){
    //cast generic file type to gpio file
    gpio_file_type* gpioFile = (gpio_file_type*) file;
    gpio_file_type_t fileType = file->fileType;
    int shift = file->shift

    if (fileType == "DIR"){
        //cast to direction file
        gpio_direction_file_type* directionFile = (gpio_direction_file_type*) gpioFile;
        uint32_t gpioOE = directionFile->gpioOE;
        mos_gpio_LLD_set_direction(gpioOE, shift, buffer[0]);
    }else if(fileType == "VAL"){
        //cast to value file
        gpio_value_file_type* valueFile = (gpio_value_file_type*) gpioFile;
       unit32_t gpioDataOut = valueFile->gpioDataOut
       mos_gpio_LLD_set_value(gpioDataOut, shift, buffer[0]);
    }else{
        return -1
    }

    return 0;
}

int mos_gpio_driver_read(const void* buffer, int bufSize, generic_file_type* file);
