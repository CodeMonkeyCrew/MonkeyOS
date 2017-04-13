/*
 * register_util.h
 *
 *  Created on: Apr 12, 2017
 *      Author: mod
 */

#ifndef SRC_DRIVERS_HAL_INCLUDES_REGISTER_UTIL_H_
#define SRC_DRIVERS_HAL_INCLUDES_REGISTER_UTIL_H_

#include <inttypes.h>

inline int clear_32(uint32_t* address){
    uint32_t clear_field = (0 << 32);
    *address &= clear_field;
    if(!(*address & clear_field)){
        return 1;
    }
    return 0;
}

inline int clearBit(uint32_t* address, uint8_t pos){
    *address &= (0 << pos);
    if(!(*address & (0 << pos))){
        return 1;
    }
    return 0;
}

inline int setBit(uint32_t* address, uint8_t pos){
    *address |= (1 << pos);
    if(*address & (1 << pos)){
         return 1;
     }
     return 0;
}
#endif /* SRC_DRIVERS_HAL_INCLUDES_REGISTER_UTIL_H_ */
