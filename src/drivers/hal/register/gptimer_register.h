#ifndef SRC_DRIVERS_HAL_INCLUDES_GPTIMER_REGISTER_H_
#define SRC_DRIVERS_HAL_INCLUDES_GPTIMER_REGISTER_H_

#include <inttypes.h>

#define GPTIMER1                0x48318000
#define GPTIMER_GENERAL_BASE    0x49032000


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
