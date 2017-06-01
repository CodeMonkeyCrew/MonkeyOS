#include <inttypes.h>
#include <kernel/drivers/gpio_driver/hal/gpiohal.h>
#include <kernel/drivers/register/cm_register.h>
#include <kernel/drivers/register/gpioregister.h>
#include <kernel/drivers/register/pw_register.h>
#include <kernel/drivers/util/registerutil.h>

#define CM_CLOCK_WKUP_BIT 13
#define CM_CLOCK_PER_OFFSET 11

void mos_gpio_LLD_set_mux_mode(uint32_t* muxModeAddr, int startBit){
    clear_bit(muxModeAddr, startBit);
    clear_bit(muxModeAddr, startBit + 1);
    set_bit(muxModeAddr, startBit + 2);
}
void mos_gpio_LLD_set_power_clock(int gpioRegion){

    //turn on Powermode
    set_bit((uint32_t*)PM_PWSTCTRL_PER, 0);
    set_bit((uint32_t*)PM_PWSTCTRL_PER, 1);

    //turn on Clocks
    if(gpioRegion == 1){
        set_bit((uint32_t*)CM_FCLKEN_WKUP, CM_CLOCK_WKUP_BIT);
        set_bit((uint32_t*)CM_ICLKEN_WKUP, CM_CLOCK_WKUP_BIT);

    }else{
        set_bit((uint32_t*)CM_ICLKEN_PER, (gpioRegion + CM_CLOCK_PER_OFFSET));
    }
}

void mos_gpio_LLD_init(int gpioPinNumber, int gpioPort, uint32_t* muxModeAddr, int startBit){
    mos_gpio_LLD_set_power_clock(gpioPort);
    mos_gpio_LLD_set_mux_mode(muxModeAddr, startBit);
}

void mos_gpio_LLD_set_direction(uint32_t* gpioOE, int shift, int direction){
    if(direction){
        //output Pin
        clear_bit(gpioOE, shift);
      }else{
        //input Pin
        set_bit(gpioOE, shift);
      }
}

void mos_gpio_LLD_set_value(uint32_t* gpioDataOut, int port, int shift, int mode){
    if(mode > 0){
         //turn on
        set_bit(gpioDataOut, shift);
     }else{
         //turn off
        uint32_t* ptr = (uint32_t*)(get_base_address(port) + GPIO_DATAOUT);
        //*ptr |= (1 << shift);
        clear_bit(ptr, shift);
     }
}

int mos_gpio_LLD_get_value(int gpioPinNumber);

/**
 * returns the number of the GPIO Region, f.ex 5 for GPIO_5 and the shift of the gpioPin f.ex 21 for GPIO_149 (1 << 21)
 * maybe used for future uses (if fileDescriptor will be changed)
 */
/**int* calcGPIORegionAndShift(int gpioPinNumber){

    int output[2];
    output[0] = -1;
    output[1] = -1;
    if(gpioPinNumber >= GPIO_1_LOWER_BOUND && gpioPinNumber <= GPIO_1_UPPER_BOUND){
        output[0] = 1;
        output[1] = gpioPinNumber - GPIO_1_LOWER_BOUND;
    }
    if(gpioPinNumber >= GPIO_2_LOWER_BOUND && gpioPinNumber <= GPIO_2_UPPER_BOUND){
        output[0] = 2;
        output[1] = gpioPinNumber - GPIO_2_LOWER_BOUND;
    }
    if(gpioPinNumber >= GPIO_3_LOWER_BOUND && gpioPinNumber <= GPIO_3_UPPER_BOUND){
        output[0] = 3;
        output[1] = gpioPinNumber - GPIO_3_LOWER_BOUND;
    }
    if(gpioPinNumber >= GPIO_4_LOWER_BOUND && gpioPinNumber <= GPIO_4_UPPER_BOUND){
        output[0] = 4;
        output[1] = gpioPinNumber - GPIO_4_LOWER_BOUND;
    }
    if(gpioPinNumber >= GPIO_5_LOWER_BOUND && gpioPinNumber <= GPIO_5_UPPER_BOUND){
        output[0] = 5;
        output[1] = gpioPinNumber - GPIO_5_LOWER_BOUND;
    }
    return output;
}*/
