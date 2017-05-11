#ifndef SRC_DRIVERS_HAL_TIMER_HAL_TIMER_HAL_H_
#define SRC_DRIVERS_HAL_TIMER_HAL_TIMER_HAL_H_

#include "../../../../kernel/drivers/register/gptimer_register.h"
#include "../timer_modes.h"

/*      Methode Definition           */

/**
 * Initialize a timer:
 *  1. Power on gptimer register
 *  2. Stop timer
 *  3. Clear the internal counter
 *  @param nr number of the gptimer, which should be initialised
 *  return
 *      0 - a value was not set correctly
 *      1 - everything worked fine
 */
uint32_t init_gptimer(const uint8_t nr);
/**
 * Enable compare mode for a GPTimer(n)
 * @param nr number of the gptimer
 * @param compareregister not used atm
 * @param value used to compare with the internal counter
 *  return
 *      0 - a value was not set correctly
 *      1 - everything worked fine
 */
uint32_t enable_compare_mode(const uint8_t nr, const uint32_t value);
/**
 * Set the mode of the interrupt
 * mode:
 *  0 - Enable match interrupt
 *  1 - Enable flow interrupt
 *  2 - Enable capture interrupt
 *  return
 *      0 - a value was not set correctly
 *      1 - everything worked fine
 */
uint32_t set_interrupt_mode(const uint8_t nr, const uint8_t mode);
/**
 * Disable interrupt for gtimer
 * @param nr number of the gptimer
 */

uint32_t disable_timer_interrupt(const uint8_t nr);
/**
 * Enable the interrupt for a GPTimer
 * @param nr number of the gptimer
 * @param autoreload
 *  0 - not reset the internal counter
 *  1 - set internal counter to 0
 */
uint32_t enable_timer_interrupt(const uint8_t nr, const uint8_t autoreload);

/**
 * Start GPTimer
 * @param nr number of the gptimer
 */
uint32_t gptimer_start(const uint8_t nr);
/**
 * Stop GPTimer
 * @param nr number of the gptimer
 */
uint32_t gptimer_stop(const uint8_t nr);
#endif /* SRC_DRIVERS_HAL_TIMER_HAL_TIMER_HAL_H_ */
