
#include "GPIO-Driver.h"


//f.ex gpioPinNumber=GPIO_149, outputMode=0 (input), on=1 (turn on) Interface for IO-Handler, calls the following functions
int writeGPIO(int gpioPinNumber, int outputMode, int on){
    int* regionShift = calcGPIORegionAndShift(gpioPinNumber);
    int gpioRegion = regionShift[0];
    int shift = regionShift[1];

    setupSystem(gpioRegion);
    setMuxMode(gpioPinNumber);
    setGPIO(gpioRegion, outputMode, on, shift);
    return 1;
}

/**
 * returns the number of the GPIO Region, f.ex 5 for GPIO_5 and the shift of the gpioPin f.ex 21 for GPIO_149 (1 << 21)
 */
int* calcGPIORegionAndShift(int gpioPinNumber){

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
}

//powermode on, Clocks, wakeup,..
void setupSystem(int gpioRegion){
    //turn on Powermode
    // enable automatic power domain state transition (0x3)
   // *CM_CLKSTCTRL_PER |= ((1 << 1) | (1 << 0));
   // set power on (0x3)
    *PM_PWSTCTRL_PER |= (1 << 1) | (1 << 0);

    //turn on Clocks
    if(gpioRegion == 1){
      *CM_FCLKEN_WKUP |= (1 << CM_CLOCK_WKUP_BIT);
      *CM_ICLKEN_WKUP |= (1 << CM_CLOCK_WKUP_BIT);
    }else{
    //  *CM_FCLKEN_PER |= (1 << (gpioRegion + CM_CLOCK_PER_OFFSET));
      *CM_ICLKEN_PER |= (1 << (gpioRegion + CM_CLOCK_PER_OFFSET));
    }

}

void setMuxMode(int gpioNumber){
    *(MUX_MODE_ADDR[gpioNumber]) &= ~(1 << START_BITS[gpioNumber]);
    *(MUX_MODE_ADDR[gpioNumber]) &= ~(1 << (START_BITS[gpioNumber] + 1));
    *(MUX_MODE_ADDR[gpioNumber]) |= (1 << (START_BITS[gpioNumber] + 2));
}

int setGPIO(int gpioRegion, int outputMode, int on, int shift){

    if(outputMode > 0){
        //output Pin
        *(GPIO_OE[gpioRegion]) &= ~(1 << shift);
    }else{
        //input Pin
        *(GPIO_OE[gpioRegion]) |= (1 << shift);
    }

    if(on > 0){
        //turn on
        *(GPIO_DATAOUT[gpioRegion]) |= (1 << shift);
    }else{
        //turn off
        *(GPIO_DATAOUT[gpioRegion]) &= ~(1 << shift);
    }
    return 1;
}

