#ifndef GPIO_HAL_H
#define GPIO_HAL_H

#include <inttypes.h>

//Power Mode
#define PM_PWSTCTRL_PER      (uint32_t*) 0x483070E0


//Clocks
#define CM_FCLKEN_WKUP        (uint32_t*) 0x48004C00 //Functional Clock, GPIO 1,  BIT 3
#define CM_ICLKEN_WKUP        (uint32_t*) 0x48004C10 //Interface Clock GPIO 1, Bit 3
#define CM_ICLKEN_PER         (uint32_t*) 0x48005010 //Interface Clock GPIO 2- 6, Bit 13 - 17
#define CM_CLOCK_PER_OFFSET   11
#define CM_CLOCK_WKUP_BIT     13




//GPIO regions, remove this?
/**
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
*/

#endif
