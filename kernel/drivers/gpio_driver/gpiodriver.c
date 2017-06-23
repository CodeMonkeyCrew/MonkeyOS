#include "../drivertypes.h"
#include "gpiodriver.h"
#include "hal/gpiohal.h"
#include "../../filesystem/filesystemregister.h"
#include "../../filesystem/filetypes/gpiotypes.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define false 0
#define true !false

static int open(generic_file_t* file);
static int write(const void* buffer, int bufSize, generic_file_t* file);
static int read(void* buffer, int bufSize, generic_file_t* file);
static void create_gpioFiles(void);

static void create_gpio_file_bundle(uint8_t pinNumber);
static void create_gpio_file(uint8_t pinNumber, file_types_t filetype);

static driver_t gpioDriver = { .driver_open = open, .driver_read = read,
                               .driver_write = write };

//should be called at boot time
void gpiodriver_init(void)
{

    register_driver(GPIO_VAL, &gpioDriver);
    register_driver(GPIO_DIR, &gpioDriver);
    create_gpioFiles();
}

/*TODO: PLS REWORK THIS SWITCH*/
static int open(generic_file_t* file)
{

    volatile int number;

    switch (file->f_type)
    {
    case GPIO_DIR:
    {
        gpio_direction_file_t* dir_file = (gpio_direction_file_t*) file;
        number = dir_file->gpio_info.number;
        break;
    }
    case GPIO_VAL:
    {
        gpio_value_file_t* val_file = (gpio_value_file_t*) file;
        number = val_file->gpio_info.number;
        break;
    }
    }
    gpiohal_pinInit(number);
    return 0;
}

static int write(const void* buffer, int bufSize, generic_file_t* file)
{
    //cast generic file type to gpio file
    file_types_t fileType = file->f_type;
    switch (fileType)
    {
    case GPIO_DIR:
    {
        //cast to direction file
        gpio_direction_file_t* directionFile = (gpio_direction_file_t*) file;
        gpiohal_pinSetDirection(directionFile->gpio_info.number,
                                *((int*) buffer));
        break;
    }
    case GPIO_VAL:
    {
        //cast to value file
        gpio_value_file_t* valueFile = (gpio_value_file_t*) file;
        gpiohal_pinSetValue(valueFile->gpio_info.number, *((uint8_t*) buffer));
        break;
    }
    default:
        return -1;
    }
    return 0;
}

static int read(void* buffer, int bufSize, generic_file_t* file)
{
//cast generic file type to gpio file
    file_types_t fileType = file->f_type;
    uint8_t pinNumber;
    switch (fileType)
    {
    case GPIO_DIR:
    {
        //cast to direction file
        gpio_value_file_t* valueFile = (gpio_value_file_t*) file;
        pinNumber = valueFile->gpio_info.number;
        break;
    }
    }
    return gpiohal_pinGetValue(pinNumber);
}

static void create_gpioFiles()
{
    create_gpio_file_bundle(130);
    create_gpio_file_bundle(131);
    create_gpio_file_bundle(132);
    create_gpio_file_bundle(133);
    create_gpio_file_bundle(134);
    create_gpio_file_bundle(135);
    create_gpio_file_bundle(136);
    create_gpio_file_bundle(145);
    create_gpio_file_bundle(148);
    create_gpio_file_bundle(149);
    create_gpio_file_bundle(150);
    create_gpio_file_bundle(158);
    create_gpio_file_bundle(159);
    create_gpio_file_bundle(161);
    create_gpio_file_bundle(183);
}

static void create_gpio_file_bundle(uint8_t pinNumber)
{
    create_gpio_file(pinNumber, GPIO_DIR);
    create_gpio_file(pinNumber, GPIO_VAL);
}
static void create_gpio_file(uint8_t pinNumber, file_types_t filetype)
{
    gpio_direction_file_t* gpioDir;
    char strBuffer[12];
    if (filetype == GPIO_DIR)
    {
        gpioDir = (gpio_direction_file_t*) mos_fs_create_file(GPIO_DIR);
        gpioDir->header.f_type = GPIO_DIR;
        sprintf(strBuffer, "gpio%i_DIR", pinNumber);
    }
    else if (filetype == GPIO_VAL)
    {
        gpioDir = (gpio_direction_file_t*) mos_fs_create_file(GPIO_VAL);
        gpioDir->header.f_type = GPIO_VAL;
        sprintf(strBuffer, "gpio%i_VAL", pinNumber);
    }

    strcpy(gpioDir->header.name, strBuffer);
    gpioDir->header.is_open = false;
    gpioDir->gpio_info.number = pinNumber;
    gpioDir->header.size = 0;
}
