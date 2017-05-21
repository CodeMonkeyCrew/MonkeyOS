#ifndef SRC_DRIVERS_GPIO_DRIVER_REGISTER_GPIOREGISTER_H_
#define SRC_DRIVERS_GPIO_DRIVER_REGISTER_GPIOREGISTER_H_

#include <inttypes.h>
/*CM & PM Defines*/
#define PM_PWSTCTRL_PER     0x483070E0

#define CM_ICLKEN_PER       0x48005010
#define CM_FCLKEN_PER       0x48005000

#define CM_ICLKEN1_CORE     0x48004A10
#define CM_FCLKEN1_CORE     0x48004A00

#define CM_FCLKEN_WKUP      0x48004C00
#define CM_ICLKEN_WKUP      0x48004C10

#define CM_CLKSEL_PER       0x48005040

/* GPIO DEFINES*/
#define GPIO1 0x48310000
#define GPIO_BASE 0x49050000

static inline uint32_t GPIOi(uint8_t gpio)
{
    return GPIO_BASE + ((gpio - 2) * (0x2000));
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

/* GPIO Region*/
#define GPIO1_L 0
#define GPIO1_H 31
#define GPIO2_L 34
#define GPIO2_H 63
#define GPIO3_H 95
#define GPIO4_H 127
#define GPIO5_H 159
/*Pin 187 is not defined in GPIO6*/
#define GPIO6_H 191

static inline uint8_t GPIO_REGION(pin)
{
    if (pin >= GPIO1_L && pin <= GPIO1_H)
    {
        return 1;
    }
    else if (pin >= GPIO2_L && pin <= GPIO2_H)
    {
        return 2;
    }
    else if (pin <= GPIO3_H)
    {
        return 3;
    }
    else if (pin <= GPIO4_H)
    {
        return 4;
    }
    else if (pin <= GPIO5_H)
    {
        return 5;
    }
    else if (pin <= GPIO6_H)
    {
        return 6;
    }
    return pin;
}

/*Offset of (-31) because of the starting bit0 and not bit1*/
static inline uint8_t GPIO_PIN_ON_REGION(pin)
{
    if (pin >= GPIO1_L && pin <= GPIO1_H)
    {
        return pin;
    }

    else if (pin >= GPIO2_L && pin <= GPIO2_H)
    {
        return pin - (GPIO2_L);
    }
    else if (pin <= GPIO3_H)
    {
        return pin -(GPIO3_H-31);
    }
    else if (pin <= GPIO4_H)
    {
        return pin -(GPIO4_H-31);
    }
    else if (pin <= GPIO5_H)
    {
        return pin -(GPIO5_H-31);
    }
    else if (pin <= GPIO6_H)
    {
        return pin -(GPIO6_H-31);
    }
    return pin;
}
/*MUX Mode*/
#define PAD_CONF_BASE_PIN130_PIN166 0x48002158

static inline uint32_t PAD_CONF_ADDRESS(pin)
{
    if (pin >= 130 && pin <= 166)
    {
        return (PAD_CONF_BASE_PIN130_PIN166 * (pin - 130));
    }
    return 0;
}

#endif /* SRC_DRIVERS_GPIO_DRIVER_REGISTER_GPIOREGISTER_H_ */
