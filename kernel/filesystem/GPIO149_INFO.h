#ifndef GPIO149_INFO_H
#define GPIO149_INFO_H

#include <inttypes.h>

//these stuff should be defined in the file system. after defining all the files, this one can be removed
#define GPIO5_DATAOUT         (uint32_t*) 0x4905603C
#define GPIO5_OE              (uint32_t*) 0x49056034

#define GPIO_PORT              5
#define MUX_MODE_GPIO_149     (uint32_t*) 0x4800217C
#define SHIFT				   21
#define START_BIT              16

#endif
