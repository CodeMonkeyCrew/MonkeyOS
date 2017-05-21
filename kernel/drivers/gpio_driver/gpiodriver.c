#include "../drivertypes.h"
#include "gpiodriver.h"
#include "hal/gpiohal.h"
#include "../../filesystem/filesystemregister.h"
#include "../../filesystem/filetypes/gpiotypes.h"
#include <stdlib.h>

/*TODO: PLS REWORK THIS SWITCH*/
static int gpiodriver_open(generic_file_t* file)
{

    int number;

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

static int gpiodriver_write(const void* buffer, int bufSize,
                            generic_file_t* file)
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

static int gpiodriver_read(void* buffer, int bufSize, generic_file_t* file)
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

static void add_drivers();
static void add_files();

//should be called at boot time
void mos_gpio_driver_init(void)
{

    add_drivers();
    add_files();
}

static void add_drivers()
{
//use malloc as it would otherwise be put on the stack,
//which will be freed when the functions ends.
//new data will then maybe override the driver
    driver_t* pGpioDriver = (driver_t*) malloc(sizeof(driver_t));
    if (pGpioDriver != NULL)
    {
        pGpioDriver->driver_read = gpiodriver_read;
        pGpioDriver->driver_write = gpiodriver_write;
        pGpioDriver->driver_open = gpiodriver_open;
        register_driver(GPIO_VAL, pGpioDriver);
        register_driver(GPIO_DIR, pGpioDriver);
    }
}

static void add_gpio_130();
static void add_gpio_131();
static void add_gpio_132();
static void add_gpio_133();
static void add_gpio_134();
static void add_gpio_135();
static void add_gpio_136();
static void add_gpio_145();
static void add_gpio_149();
static void add_gpio_158();
static void add_gpio_159();
static void add_gpio_161();
static void add_gpio_162();
static void add_gpio_183();

static void add_files()
{
    add_gpio_130();
    add_gpio_131();
    add_gpio_132();
    add_gpio_133();
    add_gpio_134();
    add_gpio_135();
    add_gpio_136();
    add_gpio_145();
    add_gpio_149();
    add_gpio_158();
    add_gpio_159();
    add_gpio_161();
    add_gpio_162();
    add_gpio_183();
}

static void add_gpio_130()
{
    gpio_direction_file_t* pGpioDir =
            (gpio_direction_file_t*) mos_fs_create_file(GPIO_DIR);
    if (pGpioDir != NULL)
    {
        strcpy(pGpioDir->header.name, "gpio130_dir");
        pGpioDir->header.is_open = false;
        pGpioDir->header.f_type = GPIO_DIR;
        pGpioDir->header.size = 0;

        pGpioDir->gpio_info.number = 130;
        pGpioDir->gpio_info.port = 5;
        pGpioDir->gpio_info.start_bit = 0;
        pGpioDir->gpio_info.shift = 2;
        pGpioDir->gpio_info.mux_mode_addr = (uint32_t*) 0x48002158;

        pGpioDir->OE = (uint32_t*) 0x49056034;
    }

    gpio_value_file_t* pGpioVal = (gpio_value_file_t*) mos_fs_create_file(
            GPIO_VAL);
    if (pGpioVal != NULL)
    {
        strcpy(pGpioVal->header.name, "gpio130_val");
        pGpioVal->header.is_open = false;
        pGpioVal->header.f_type = GPIO_VAL;
        pGpioVal->header.size = 0;

        pGpioVal->gpio_info.number = 130;
        pGpioVal->gpio_info.port = 5;
        pGpioVal->gpio_info.start_bit = 0;
        pGpioVal->gpio_info.shift = 2;
        pGpioVal->gpio_info.mux_mode_addr = (uint32_t*) 0x48002158;

        pGpioVal->data_out = (uint32_t*) 0x49056094;
    }
}

static void add_gpio_131()
{
    gpio_direction_file_t* pGpioDir =
            (gpio_direction_file_t*) mos_fs_create_file(GPIO_DIR);
    if (pGpioDir != NULL)
    {
        strcpy(pGpioDir->header.name, "gpio131_dir");
        pGpioDir->header.is_open = false;
        pGpioDir->header.f_type = GPIO_DIR;
        pGpioDir->header.size = 0;

        pGpioDir->gpio_info.number = 131;
        pGpioDir->gpio_info.port = 5;
        pGpioDir->gpio_info.start_bit = 16;
        pGpioDir->gpio_info.shift = 3;
        pGpioDir->gpio_info.mux_mode_addr = (uint32_t*) 0x48002158;

        pGpioDir->OE = (uint32_t*) 0x49056034;
    }

    gpio_value_file_t* pGpioVal = (gpio_value_file_t*) mos_fs_create_file(
            GPIO_VAL);
    if (pGpioVal != NULL)
    {
        strcpy(pGpioVal->header.name, "gpio131_val");
        pGpioVal->header.is_open = false;
        pGpioVal->header.f_type = GPIO_VAL;
        pGpioVal->header.size = 0;

        pGpioVal->gpio_info.number = 131;
        pGpioVal->gpio_info.port = 5;
        pGpioVal->gpio_info.start_bit = 16;
        pGpioVal->gpio_info.shift = 3;
        pGpioVal->gpio_info.mux_mode_addr = (uint32_t*) 0x48002158;

        pGpioVal->data_out = (uint32_t*) 0x49056094;
    }
}

static void add_gpio_132()
{
    gpio_direction_file_t* pGpioDir =
            (gpio_direction_file_t*) mos_fs_create_file(GPIO_DIR);
    if (pGpioDir != NULL)
    {
        strcpy(pGpioDir->header.name, "gpio132_dir");
        pGpioDir->header.is_open = false;
        pGpioDir->header.f_type = GPIO_DIR;
        pGpioDir->header.size = 0;

        pGpioDir->gpio_info.number = 132;
        pGpioDir->gpio_info.port = 5;
        pGpioDir->gpio_info.start_bit = 0;
        pGpioDir->gpio_info.shift = 4;
        pGpioDir->gpio_info.mux_mode_addr = (uint32_t*) 0x4800215C;

        pGpioDir->OE = (uint32_t*) 0x49056034;
    }

    gpio_value_file_t* pGpioVal = (gpio_value_file_t*) mos_fs_create_file(
            GPIO_VAL);
    if (pGpioVal != NULL)
    {
        strcpy(pGpioVal->header.name, "gpio132_val");
        pGpioVal->header.is_open = false;
        pGpioVal->header.f_type = GPIO_VAL;
        pGpioVal->header.size = 0;

        pGpioVal->gpio_info.number = 132;
        pGpioVal->gpio_info.port = 5;
        pGpioVal->gpio_info.start_bit = 0;
        pGpioVal->gpio_info.shift = 4;
        pGpioVal->gpio_info.mux_mode_addr = (uint32_t*) 0x4800215C;

        pGpioVal->data_out = (uint32_t*) 0x49056094;
    }
}

static void add_gpio_133()
{
    gpio_direction_file_t* pGpioDir =
            (gpio_direction_file_t*) mos_fs_create_file(GPIO_DIR);
    if (pGpioDir != NULL)
    {
        strcpy(pGpioDir->header.name, "gpio133_dir");
        pGpioDir->header.is_open = false;
        pGpioDir->header.f_type = GPIO_DIR;
        pGpioDir->header.size = 0;

        pGpioDir->gpio_info.number = 133;
        pGpioDir->gpio_info.port = 5;
        pGpioDir->gpio_info.start_bit = 16;
        pGpioDir->gpio_info.shift = 5;
        pGpioDir->gpio_info.mux_mode_addr = (uint32_t*) 0x4800215C;

        pGpioDir->OE = (uint32_t*) 0x49056034;
    }

    gpio_value_file_t* pGpioVal = (gpio_value_file_t*) mos_fs_create_file(
            GPIO_VAL);
    if (pGpioVal != NULL)
    {
        strcpy(pGpioVal->header.name, "gpio133_val");
        pGpioVal->header.is_open = false;
        pGpioVal->header.f_type = GPIO_VAL;
        pGpioVal->header.size = 0;

        pGpioVal->gpio_info.number = 133;
        pGpioVal->gpio_info.port = 5;
        pGpioVal->gpio_info.start_bit = 16;
        pGpioVal->gpio_info.shift = 5;
        pGpioVal->gpio_info.mux_mode_addr = (uint32_t*) 0x4800215C;

        pGpioVal->data_out = (uint32_t*) 0x49056094;
    }
}

static void add_gpio_134()
{
    gpio_direction_file_t* pGpioDir =
            (gpio_direction_file_t*) mos_fs_create_file(GPIO_DIR);
    if (pGpioDir != NULL)
    {
        strcpy(pGpioDir->header.name, "gpio134_dir");
        pGpioDir->header.is_open = false;
        pGpioDir->header.f_type = GPIO_DIR;
        pGpioDir->header.size = 0;

        pGpioDir->gpio_info.number = 134;
        pGpioDir->gpio_info.port = 5;
        pGpioDir->gpio_info.start_bit = 0;
        pGpioDir->gpio_info.shift = 6;
        pGpioDir->gpio_info.mux_mode_addr = (uint32_t*) 0x48002160;

        pGpioDir->OE = (uint32_t*) 0x49056034;
    }

    gpio_value_file_t* pGpioVal = (gpio_value_file_t*) mos_fs_create_file(
            GPIO_VAL);
    if (pGpioVal != NULL)
    {
        strcpy(pGpioVal->header.name, "gpio134_val");
        pGpioVal->header.is_open = false;
        pGpioVal->header.f_type = GPIO_VAL;
        pGpioVal->header.size = 0;

        pGpioVal->gpio_info.number = 134;
        pGpioVal->gpio_info.port = 5;
        pGpioVal->gpio_info.start_bit = 0;
        pGpioVal->gpio_info.shift = 6;
        pGpioVal->gpio_info.mux_mode_addr = (uint32_t*) 0x48002160;

        pGpioVal->data_out = (uint32_t*) 0x49056094;
    }
}

static void add_gpio_135()
{
    gpio_direction_file_t* pGpioDir =
            (gpio_direction_file_t*) mos_fs_create_file(GPIO_DIR);
    if (pGpioDir != NULL)
    {
        strcpy(pGpioDir->header.name, "gpio135_dir");
        pGpioDir->header.is_open = false;
        pGpioDir->header.f_type = GPIO_DIR;
        pGpioDir->header.size = 0;

        pGpioDir->gpio_info.number = 135;
        pGpioDir->gpio_info.port = 5;
        pGpioDir->gpio_info.start_bit = 16;
        pGpioDir->gpio_info.shift = 7;
        pGpioDir->gpio_info.mux_mode_addr = (uint32_t*) 0x48002160;

        pGpioDir->OE = (uint32_t*) 0x49056034;
    }

    gpio_value_file_t* pGpioVal = (gpio_value_file_t*) mos_fs_create_file(
            GPIO_VAL);
    if (pGpioVal != NULL)
    {
        strcpy(pGpioVal->header.name, "gpio135_val");
        pGpioVal->header.is_open = false;
        pGpioVal->header.f_type = GPIO_VAL;
        pGpioVal->header.size = 0;

        pGpioVal->gpio_info.number = 135;
        pGpioVal->gpio_info.port = 5;
        pGpioVal->gpio_info.start_bit = 16;
        pGpioVal->gpio_info.shift = 7;
        pGpioVal->gpio_info.mux_mode_addr = (uint32_t*) 0x48002160;

        pGpioVal->data_out = (uint32_t*) 0x49056094;
    }
}

static void add_gpio_136()
{
    gpio_direction_file_t* pGpioDir =
            (gpio_direction_file_t*) mos_fs_create_file(GPIO_DIR);
    if (pGpioDir != NULL)
    {
        strcpy(pGpioDir->header.name, "gpio136_dir");
        pGpioDir->header.is_open = false;
        pGpioDir->header.f_type = GPIO_DIR;
        pGpioDir->header.size = 0;

        pGpioDir->gpio_info.number = 136;
        pGpioDir->gpio_info.port = 5;
        pGpioDir->gpio_info.start_bit = 0;
        pGpioDir->gpio_info.shift = 8;
        pGpioDir->gpio_info.mux_mode_addr = (uint32_t*) 0x48002164;

        pGpioDir->OE = (uint32_t*) 0x49056034;
    }

    gpio_value_file_t* pGpioVal = (gpio_value_file_t*) mos_fs_create_file(
            GPIO_VAL);
    if (pGpioVal != NULL)
    {
        strcpy(pGpioVal->header.name, "gpio136_val");
        pGpioVal->header.is_open = false;
        pGpioVal->header.f_type = GPIO_VAL;
        pGpioVal->header.size = 0;

        pGpioVal->gpio_info.number = 136;
        pGpioVal->gpio_info.port = 5;
        pGpioVal->gpio_info.start_bit = 0;
        pGpioVal->gpio_info.shift = 8;
        pGpioVal->gpio_info.mux_mode_addr = (uint32_t*) 0x48002164;

        pGpioVal->data_out = (uint32_t*) 0x49056094;
    }
}

static void add_gpio_145()
{
    gpio_direction_file_t* pGpioDir =
            (gpio_direction_file_t*) mos_fs_create_file(GPIO_DIR);
    if (pGpioDir != NULL)
    {
        strcpy(pGpioDir->header.name, "gpio145_dir");
        pGpioDir->header.is_open = false;
        pGpioDir->header.f_type = GPIO_DIR;
        pGpioDir->header.size = 0;

        pGpioDir->gpio_info.number = 145;
        pGpioDir->gpio_info.port = 5;
        pGpioDir->gpio_info.start_bit = 16;
        pGpioDir->gpio_info.shift = 17;
        pGpioDir->gpio_info.mux_mode_addr = (uint32_t*) 0x48002174;

        pGpioDir->OE = (uint32_t*) 0x49056034;
    }

    gpio_value_file_t* pGpioVal = (gpio_value_file_t*) mos_fs_create_file(
            GPIO_VAL);
    if (pGpioVal != NULL)
    {
        strcpy(pGpioVal->header.name, "gpio145_val");
        pGpioVal->header.is_open = false;
        pGpioVal->header.f_type = GPIO_VAL;
        pGpioVal->header.size = 0;

        pGpioVal->gpio_info.number = 145;
        pGpioVal->gpio_info.port = 5;
        pGpioVal->gpio_info.start_bit = 16;
        pGpioVal->gpio_info.shift = 17;
        pGpioVal->gpio_info.mux_mode_addr = (uint32_t*) 0x48002174;

        pGpioVal->data_out = (uint32_t*) 0x49056094;
    }
}

static void add_gpio_149()
{
    gpio_direction_file_t* pGpioDir =
            (gpio_direction_file_t*) mos_fs_create_file(GPIO_DIR);
    if (pGpioDir != NULL)
    {
        strcpy(pGpioDir->header.name, "gpio149_dir");
        pGpioDir->header.is_open = false;
        pGpioDir->header.f_type = GPIO_DIR;
        pGpioDir->header.size = 0;

        pGpioDir->gpio_info.number = 149;
        pGpioDir->gpio_info.port = 5;
        pGpioDir->gpio_info.start_bit = 16;
        pGpioDir->gpio_info.shift = 21;
        pGpioDir->gpio_info.mux_mode_addr = (uint32_t*) 0x4800217C;

        pGpioDir->OE = (uint32_t*) 0x49056034;
    }

    gpio_value_file_t* pGpioVal = (gpio_value_file_t*) mos_fs_create_file(
            GPIO_VAL);
    if (pGpioVal != NULL)
    {
        strcpy(pGpioVal->header.name, "gpio149_val");
        pGpioVal->header.is_open = false;
        pGpioVal->header.f_type = GPIO_VAL;
        pGpioVal->header.size = 0;

        pGpioVal->gpio_info.number = 149;
        pGpioVal->gpio_info.port = 5;
        pGpioVal->gpio_info.start_bit = 16;
        pGpioVal->gpio_info.shift = 21;
        pGpioVal->gpio_info.mux_mode_addr = (uint32_t*) 0x4800217C;

        pGpioVal->data_out = (uint32_t*) 0x49056094;
    }
}

static void add_gpio_158()
{
    gpio_direction_file_t* pGpioDir =
            (gpio_direction_file_t*) mos_fs_create_file(GPIO_DIR);
    if (pGpioDir != NULL)
    {
        strcpy(pGpioDir->header.name, "gpio158_dir");
        pGpioDir->header.is_open = false;
        pGpioDir->header.f_type = GPIO_DIR;
        pGpioDir->header.size = 0;

        pGpioDir->gpio_info.number = 158;
        pGpioDir->gpio_info.port = 5;
        pGpioDir->gpio_info.start_bit = 0;
        pGpioDir->gpio_info.shift = 30;
        pGpioDir->gpio_info.mux_mode_addr = (uint32_t*) 0x48002190;

        pGpioDir->OE = (uint32_t*) 0x49056034;
    }

    gpio_value_file_t* pGpioVal = (gpio_value_file_t*) mos_fs_create_file(
            GPIO_VAL);
    if (pGpioVal != NULL)
    {
        strcpy(pGpioVal->header.name, "gpio158_val");
        pGpioVal->header.is_open = false;
        pGpioVal->header.f_type = GPIO_VAL;
        pGpioVal->header.size = 0;

        pGpioVal->gpio_info.number = 158;
        pGpioVal->gpio_info.port = 5;
        pGpioVal->gpio_info.start_bit = 0;
        pGpioVal->gpio_info.shift = 30;
        pGpioVal->gpio_info.mux_mode_addr = (uint32_t*) 0x48002190;

        pGpioVal->data_out = (uint32_t*) 0x49056094;
    }
}

static void add_gpio_159()
{
    gpio_direction_file_t* pGpioDir =
            (gpio_direction_file_t*) mos_fs_create_file(GPIO_DIR);
    if (pGpioDir != NULL)
    {
        strcpy(pGpioDir->header.name, "gpio159_dir");
        pGpioDir->header.is_open = false;
        pGpioDir->header.f_type = GPIO_DIR;
        pGpioDir->header.size = 0;

        pGpioDir->gpio_info.number = 159;
        pGpioDir->gpio_info.port = 5;
        pGpioDir->gpio_info.start_bit = 16;
        pGpioDir->gpio_info.shift = 31;
        pGpioDir->gpio_info.mux_mode_addr = (uint32_t*) 0x48002190;

        pGpioDir->OE = (uint32_t*) 0x49056034;
    }

    gpio_value_file_t* pGpioVal = (gpio_value_file_t*) mos_fs_create_file(
            GPIO_VAL);
    if (pGpioVal != NULL)
    {
        strcpy(pGpioVal->header.name, "gpio159_val");
        pGpioVal->header.is_open = false;
        pGpioVal->header.f_type = GPIO_VAL;
        pGpioVal->header.size = 0;

        pGpioVal->gpio_info.number = 159;
        pGpioVal->gpio_info.port = 5;
        pGpioVal->gpio_info.start_bit = 16;
        pGpioVal->gpio_info.shift = 31;
        pGpioVal->gpio_info.mux_mode_addr = (uint32_t*) 0x48002190;

        pGpioVal->data_out = (uint32_t*) 0x49056094;
    }
}

static void add_gpio_161()
{
    gpio_direction_file_t* pGpioDir =
            (gpio_direction_file_t*) mos_fs_create_file(GPIO_DIR);
    if (pGpioDir != NULL)
    {
        strcpy(pGpioDir->header.name, "gpio161_dir");
        pGpioDir->header.is_open = false;
        pGpioDir->header.f_type = GPIO_DIR;
        pGpioDir->header.size = 0;

        pGpioDir->gpio_info.number = 161;
        pGpioDir->gpio_info.port = 6;
        pGpioDir->gpio_info.start_bit = 16;
        pGpioDir->gpio_info.shift = 1;
        pGpioDir->gpio_info.mux_mode_addr = (uint32_t*) 0x48002194;

        pGpioDir->OE = (uint32_t*) 0x49058034;
    }

    gpio_value_file_t* pGpioVal = (gpio_value_file_t*) mos_fs_create_file(
            GPIO_VAL);
    if (pGpioVal != NULL)
    {
        strcpy(pGpioVal->header.name, "gpio161_val");
        pGpioVal->header.is_open = false;
        pGpioVal->header.f_type = GPIO_VAL;
        pGpioVal->header.size = 0;

        pGpioVal->gpio_info.number = 161;
        pGpioVal->gpio_info.port = 6;
        pGpioVal->gpio_info.start_bit = 16;
        pGpioVal->gpio_info.shift = 1;
        pGpioVal->gpio_info.mux_mode_addr = (uint32_t*) 0x48002194;

        pGpioVal->data_out = (uint32_t*) 0x49058094;
    }
}

static void add_gpio_162()
{
    gpio_direction_file_t* pGpioDir =
            (gpio_direction_file_t*) mos_fs_create_file(GPIO_DIR);
    if (pGpioDir != NULL)
    {
        strcpy(pGpioDir->header.name, "gpio162_dir");
        pGpioDir->header.is_open = false;
        pGpioDir->header.f_type = GPIO_DIR;
        pGpioDir->header.size = 0;

        pGpioDir->gpio_info.number = 162;
        pGpioDir->gpio_info.port = 6;
        pGpioDir->gpio_info.start_bit = 0;
        pGpioDir->gpio_info.shift = 2;
        pGpioDir->gpio_info.mux_mode_addr = (uint32_t*) 0x48002198;

        pGpioDir->OE = (uint32_t*) 0x49058034;
    }

    gpio_value_file_t* pGpioVal = (gpio_value_file_t*) mos_fs_create_file(
            GPIO_VAL);
    if (pGpioVal != NULL)
    {
        strcpy(pGpioVal->header.name, "gpio162_val");
        pGpioVal->header.is_open = false;
        pGpioVal->header.f_type = GPIO_VAL;
        pGpioVal->header.size = 0;

        pGpioVal->gpio_info.number = 162;
        pGpioVal->gpio_info.port = 6;
        pGpioVal->gpio_info.start_bit = 0;
        pGpioVal->gpio_info.shift = 2;
        pGpioVal->gpio_info.mux_mode_addr = (uint32_t*) 0x48002198;

        pGpioVal->data_out = (uint32_t*) 0x49058094;
    }
}

static void add_gpio_183()
{
    gpio_direction_file_t* pGpioDir =
            (gpio_direction_file_t*) mos_fs_create_file(GPIO_DIR);
    if (pGpioDir != NULL)
    {
        strcpy(pGpioDir->header.name, "gpio183_dir");
        pGpioDir->header.is_open = false;
        pGpioDir->header.f_type = GPIO_DIR;
        pGpioDir->header.size = 0;

        pGpioDir->gpio_info.number = 183;
        pGpioDir->gpio_info.port = 6;
        pGpioDir->gpio_info.start_bit = 0;
        pGpioDir->gpio_info.shift = 23;
        pGpioDir->gpio_info.mux_mode_addr = (uint32_t*) 0x480021C0;

        pGpioDir->OE = (uint32_t*) 0x49058034;

    }

    gpio_value_file_t* pGpioVal = (gpio_value_file_t*) mos_fs_create_file(
            GPIO_VAL);
if (pGpioVal != NULL)
{
    strcpy(pGpioVal->header.name, "gpio183_val");
    pGpioVal->header.is_open = false;
    pGpioVal->header.f_type = GPIO_VAL;
    pGpioVal->header.size = 0;
    gpio_value_file_t* pGpio149_val = (gpio_value_file_t*) mos_fs_create_file(
            GPIO_VAL);
    if (pGpio149_val != NULL)
    {
        strcpy(pGpio149_val->header.name, "gpio149_val");
        pGpio149_val->header.is_open = false;
        pGpio149_val->header.f_type = GPIO_VAL;
        pGpio149_val->header.size = 0;

        pGpioVal->gpio_info.number = 183;
        pGpioVal->gpio_info.port = 6;
        pGpioVal->gpio_info.start_bit = 0;
        pGpioVal->gpio_info.shift = 23;
        pGpioVal->gpio_info.mux_mode_addr = (uint32_t*) 0x480021C0;
        pGpio149_val->gpio_info.number = 149;
        pGpio149_val->gpio_info.port = 5;
        pGpio149_val->gpio_info.start_bit = 16;
        pGpio149_val->gpio_info.shift = 21;
        pGpio149_val->gpio_info.mux_mode_addr = (uint32_t*) 0x4800217C;

        pGpioVal->data_out = (uint32_t*) 0x49058094;
        pGpio149_val->data_out = (uint32_t*) 0x49056094;
    }
}
