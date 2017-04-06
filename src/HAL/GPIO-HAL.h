#ifndef GPIO_HAL_H
#define GPIO_HAL_H

//GPIO regions
#define GPIO_1_LOWER_BOUND    0
#define GPIO_1_UPPER_BOUND    31
#define GPIO_2_LOWER_BOUND    34
#define GPIO_2_UPPER_BOUND    63
#define GPIO_3_LOWER_BOUND    64
#define GPIO_3_UPPER_BOUND    95
#define GPIO_4_LOWER_BOUND    96
#define GPIO_4_UPPER_BOUND    127
#define GPIO_5_LOWER_BOUND    128
#define GPIO_5_UPPER_BOUND    159
#define GPIO_6_LOWER_BOUND    160
#define GPIO_6_UPPER_BOUND    191

//Power Mode
#define CM_CLKSTCTRL_PER     (long*) 0x48005048
#define PM_PWSTCTRL_PER      (long*) 0x483070E0


//Clocks
#define CM_FCLKEN_WKUP        (long*) 0x48004C00 //Functional Clock, GPIO 1,  BIT 3
#define CM_FCLKEN_PER         (long*) 0x48005000  //Functional Clock, GPIO 2- 6, Bit 13 - 17
#define CM_ICLKEN_WKUP        (long*) 0x48004C10 //Interface Clock GPIO 1, Bit 3
#define CM_ICLKEN_PER         (long*) 0x48005010 //Interface Clock GPIO 2- 6, Bit 13 - 17
#define CM_CLOCK_PER_OFFSET   11
#define CM_CLOCK_WKUP_BIT     13


//GPIOs
#define GPIO5_DATAOUT         (long*) 0x4905603C
#define GPIO5_OE              (long*) 0x49056034
long* GPIO_DATAOUT[6];
long* GPIO_OE[6];


//Mux Mode
#define MUX_MODE_GPIO_149     (long*) 0x4800217C
long* MUX_MODE_ADDR[186];
int START_BITS[186];


void initHAL(void);

#endif
