#include "gpio-driver.h"

void setMuxMode(uint32_t* muxModeAddr, int startBit){
    *muxModeAddr &= ~(1 << startBit);
    *muxModeAddr &= ~(1 << (startBit + 1));
    *muxModeAddr |= (1 << (startBit + 2));
}
void setPowerModeClock(int gpioRegion){

    //turn on Powermode
    *PM_PWSTCTRL_PER |= (1 << 1) | (1 << 0);

    //turn on Clocks
    if(gpioRegion == 1){
      *CM_FCLKEN_WKUP |= (1 << CM_CLOCK_WKUP_BIT);
      *CM_ICLKEN_WKUP |= (1 << CM_CLOCK_WKUP_BIT);
    }else{
      *CM_ICLKEN_PER |= (1 << (gpioRegion + CM_CLOCK_PER_OFFSET));
    }

}
void mos_gpioLLD_init(int gpioPinNumber, int gpioPort, uint32_t* muxModeAddr, int startBit){
    setPowerModeClock(gpioPort);
    setMuxMode(muxModeAddr, startBit);
}

void mos_gpio_LLD_setDirection(uint32_t* gpioOE, int shift, int direction){
    if(direction > 0){
        //input Pin
        *gpioOE |= (1 << shift);
      }else{
        //output Pin
        *gpioOE &= ~(1 << shift);
      }
}

void mos_gpio_LLD_write(uint32_t* gpioDataOut, int shift, int mode){
    if(mode > 0){
         //turn on
        *gpioDataOut |= (1 << shift);
     }else{
         //turn off
        *gpioDataOut &= ~(1 << shift);
     }
}
int mos_gpio_LLD_read(int gpioPinNumber);

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
