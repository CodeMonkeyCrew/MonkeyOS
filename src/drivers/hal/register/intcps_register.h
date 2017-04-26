/*
 * intcps_register.h
 *
 *  Created on: Apr 20, 2017
 *      Author: mod
 */

#ifndef SRC_DRIVERS_HAL_INCLUDES_INTCPS_REGISTER_H_
#define SRC_DRIVERS_HAL_INCLUDES_INTCPS_REGISTER_H_

#define INTCPS_MIR_BASE 0x48200084

inline uint32_t MIRn(uint8_t n){
    return INTCPS_MIR_BASE + (0x20 * n);
}

#endif /* SRC_DRIVERS_HAL_INCLUDES_INTCPS_REGISTER_H_ */