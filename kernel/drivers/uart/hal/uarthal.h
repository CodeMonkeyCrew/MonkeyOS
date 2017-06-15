
#ifndef KERNEL_DRIVERS_UART_HAL_UARTHAL_H_
#define KERNEL_DRIVERS_UART_HAL_UARTHAL_H_
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

int uarthal_init(int uartNumber);
void uarthal_transmit(const char* buffer, int bufferSize, int uartNumber);
int uarthal_receive(char* buffer, int bufferSize, int uartNumber);


/*Interrupt */
#define INTCPS_MIR_BASE 0x48200084

static inline uint32_t MIRn(uint8_t n){
    return INTCPS_MIR_BASE + (0x20 * n);
}

/*Power and Timer Managment*/
#define CM_ICLKEN_PER       0x48005010
#define CM_FCLKEN_PER       0x48005000

#define CM_ICLKEN1_CORE     0x48004A10
#define CM_FCLKEN1_CORE     0x48004A00

#define CM_FCLKEN_WKUP      0x48004C00
#define CM_ICLKEN_WKUP      0x48004C10

#define CM_CLKSEL_PER       0x48005040


#endif /* KERNEL_DRIVERS_UART_HAL_UARTHAL_H_ */
