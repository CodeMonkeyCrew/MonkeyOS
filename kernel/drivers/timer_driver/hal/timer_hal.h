#ifndef SRC_DRIVERS_HAL_TIMER_HAL_TIMER_HAL_H_
#define SRC_DRIVERS_HAL_TIMER_HAL_TIMER_HAL_H_
#include <inttypes.h>
#include "../timer_modes.h"

/* ---------------------------------------------------- *
 * GTIMER - HAL                                         *
 * ---------------------------------------------------- */
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
void timer_init(const uint8_t nr);
/**
 * Enable compare mode for a GPTimer(n)
 * @param nr number of the gptimer
 * @param compareregister not used atm
 * @param value used to compare with the internal counter
 *  return
 *      0 - a value was not set correctly
 *      1 - everything worked fine
 */
void timer_enable_compare_mode(const uint8_t nr, const uint32_t value);
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
void timer_set_interrupt_mode(const uint8_t nr, const uint8_t mode);
/**
 * Disable interrupt for gtimer
 * @param nr number of the gptimer
 */

void timer_disable_interrupt(const uint8_t nr);
/**
 * Enable the interrupt for a GPTimer
 * @param nr number of the gptimer
 * @param autoreload
 *  0 - not reset the internal counter
 *  1 - set internal counter to 0
 */
void timer_enable_interrupt(const uint8_t nr, const uint8_t autoreload);

/**
 * clear interrupt bit for gptimer
 * @param nr number of the gptimer
 */
void timer_clear_interrupt(const uint8_t nr);
/**
 * Start GPTimer
 * @param nr number of the gptimer
 */
void timer_start(const uint8_t nr);
/**
 * Stop GPTimer
 * @param nr number of the gptimer
 */
void timer_stop(const uint8_t nr);


/**
 * Set the internal clock for a gptimer
 * @param nr number of the gptimer
 * @param clock_mode
 *          0 - 32k
 *          1 - Sys-Clock
 */
void timer_set_clock(const uint8_t nr, const uint8_t clock_mode);

#endif /* SRC_DRIVERS_HAL_TIMER_HAL_TIMER_HAL_H_ */
