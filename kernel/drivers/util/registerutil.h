#ifndef SRC_DRIVERS_GPIO_DRIVER_UTIL_REGISTERUTIL_H_
#define SRC_DRIVERS_GPIO_DRIVER_UTIL_REGISTERUTIL_H_

#include <inttypes.h>

static inline int clear_32(uint32_t* address){
    uint32_t clear_field = (0 << 31);
    *address &= clear_field;
    return 0;
}

static inline int clear_bit(uint32_t* address, uint8_t pos){
    *address &= ~(1 << pos);
    return 0;
}

static inline int set_bit(uint32_t* address, uint8_t pos){
    *address |= (1 << pos);
     return 0;
}

static inline uint32_t set_value_32(uint32_t* address, uint32_t value)
{
    *address = value;
    return 0;
}

static inline uint32_t* get_address(uint32_t address, uint32_t offset){
    return (uint32_t*)(address + offset);
}
#endif /* SRC_DRIVERS_GPIO_DRIVER_UTIL_REGISTERUTIL_H_ */
