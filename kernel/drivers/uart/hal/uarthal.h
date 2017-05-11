
#ifndef KERNEL_DRIVERS_UART_HAL_UARTHAL_H_
#define KERNEL_DRIVERS_UART_HAL_UARTHAL_H_
#include <inttypes.h>

int uarthal_init(int uartNumber);
void uarthal_transmit(const char* buffer, int bufferSize, int uartNumber);
char* uarthal_receive(int uartNumber);




#endif /* KERNEL_DRIVERS_UART_HAL_UARTHAL_H_ */
