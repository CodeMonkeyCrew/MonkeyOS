#ifndef SRC_DRIVERS_GPIO_DRIVER_REGISTER_GPIOREGISTER_H_
#define SRC_DRIVERS_GPIO_DRIVER_REGISTER_GPIOREGISTER_H_

#include <inttypes.h>

#define GPIO1 0x48310000
#define GPIO_BASE 0x49050000

static inline uint32_t get_base_address(uint8_t gpio)
{
    return GPIO_BASE + ((gpio-2) * (0x2000));
}

#define GPIO_REVISION           0x000
#define GPIO_SYSCONFIG          0x010
#define GPIO_SYSSTATUS          0x014
#define GPIO_IRQSTATUS1         0x018
#define GPIO_IRQENABLE1         0x01C
#define GPIO_WAKEUPENABLE       0x020
#define GPIO_IRQSTATUS2         0x028
#define GPIO_IRQENABLE2         0x02C
#define GPIO_CTRL               0x030
#define GPIO_OE                 0x034
#define GPIO_DATAIN             0x038
#define GPIO_DATAOUT            0x03C
#define GPIO_LEVELDETECT0       0x040
#define GPIO_LEVELDETECT1       0x044
#define GPIO_RISINGDETECT       0x048
#define GPIO_FALLINGDETECT      0x04C
#define GPIO_DEBOUNCENABLE      0x050
#define GPIO_DEBOUNCINGTIME     0x054
#define GPIO_CLEARIRQENABLE1    0x060
#define GPIO_SETIRQENABLE1      0x064
#define GPIO_CLEARIRQENABLE2    0x070
#define GPIO_SETIRQENABLE2      0x074
#define GPIO_CLEARWKUENA        0x080
#define GPIO_SETWKUENA          0x084
#define GPIO_CLEARDATAOUT       0x090
#define GPIO_SETDATAOUT         0x094

#endif /* SRC_DRIVERS_GPIO_DRIVER_REGISTER_GPIOREGISTER_H_ */
