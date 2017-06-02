#ifndef SRC_DRIVERS_HAL_INCLUDES_GPTIMER_REGISTER_H_
#define SRC_DRIVERS_HAL_INCLUDES_GPTIMER_REGISTER_H_

#include <inttypes.h>
/*----------------------------------------------------*
 *              CM & PM Register                      *
 *              Clock Mode and Powermanagment         *
 *----------------------------------------------------*/
#define CM_ICLKEN_PER       0x48005010
#define CM_FCLKEN_PER       0x48005000

#define CM_ICLKEN1_CORE     0x48004A10
#define CM_FCLKEN1_CORE     0x48004A00

#define CM_FCLKEN_WKUP      0x48004C00
#define CM_ICLKEN_WKUP      0x48004C10

#define CM_CLKSEL_PER       0x48005040
/*----------------------------------------------------*
 *              Interrupt Register                    *
 *                                                    *
 *----------------------------------------------------*/
#define INTCPS_MIR_BASE 0x48200084

inline uint32_t MIRn(uint8_t n){
    return INTCPS_MIR_BASE + (0x20 * n);
}
/*----------------------------------------------------*
 *              GPTIMER Register                      *
 *                                                    *
 *----------------------------------------------------*/
#define GPTIMER1                0x48318000
#define GPTIMER_GENERAL_BASE    0x49032000

#define GPTIMER(n) (GPTIMER_GENERAL_BASE + (0x2000 * (n - 2)))


#define TIDR        (uint32_t) 0x0     //This register contains the IP revision code.
#define TIOCP_CFG   (uint32_t) 0x10    //This register controls the various parameters of the GP timer L4 interface.
#define TISTAT      (uint32_t) 0x14    //This register provides status information about the module, excluding the interrupt status
//information.
#define TISR        (uint32_t) 0x18    //This register shows which interrupt events are pending inside the module.
#define TIER        (uint32_t) 0x1C    //This register controls (enable/disable) the interrupt events.
#define TWER        (uint32_t) 0x20    //This register controls (enable/disable) the wake-up feature on specific interrupt events.
#define TCLR        (uint32_t) 0x24    //This register controls optional features specific to the timer functionality.
#define TCRR        (uint32_t) 0x28
#define TLDR        (uint32_t) 0x02C
#define TTGR        (uint32_t) 0x030
#define TWPS        (uint32_t) 0x034
#define TMAR        (uint32_t) 0x038
#define TCAR1       (uint32_t) 0x03C
#define TSICR       (uint32_t) 0x040
#define TCAR2       (uint32_t) 0x044
#define TPIR        (uint32_t) 0x048
#define TNIR        (uint32_t) 0x04C
#define TCVR        (uint32_t) 0x050
#define TOCR        (uint32_t) 0x054
#define TOWR        (uint32_t) 0x058

#endif /* SRC_DRIVERS_HAL_INCLUDES_GPTIMER_REGISTER_H_ */
