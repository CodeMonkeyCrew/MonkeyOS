#ifndef SRC_DRIVERS_HAL_TIMER_HAL_TIMER_HAL_H_
#define SRC_DRIVERS_HAL_TIMER_HAL_TIMER_HAL_H_

#include "../register/gptimer_register.h"

typedef enum mode{
    CAPTURE, COMPARE
}Mode_t;

/*      Methode Definition           */

uint32_t init_gptimer(const uint8_t gptimer);

uint32_t enable_compare_mode(const uint8_t gptimer, const uint8_t compareregister, const uint32_t value);

uint32_t enable_interrupt(const uint8_t gptimer);

uint32_t gptimer_start(const uint8_t gptimer);
uint32_t gptimer_stop(const uint8_t gptimer);
#endif /* SRC_DRIVERS_HAL_TIMER_HAL_TIMER_HAL_H_ */
