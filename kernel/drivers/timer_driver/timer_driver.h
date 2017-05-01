#ifndef SRC_DRIVERS_GPIO_DRIVER_TIMER_DRIVER_H_
#define SRC_DRIVERS_GPIO_DRIVER_TIMER_DRIVER_H_

#include "../drivertypes.h"

void mos_timer_driver_init(void);
static void mos_timer_read(const void* buffer, int bufSize, generic_file_t* file);
static void mos_timer_write(const void* buffer, int bufSize, generic_file_t* file);
static void mos_timer_open(generic_file_t* file);

#endif /* SRC_DRIVERS_GPIO_DRIVER_TIMER_DRIVER_H_ */
