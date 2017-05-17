
#ifndef KERNEL_DRIVERS_UART_HAL_UARTHAL_H_
#define KERNEL_DRIVERS_UART_HAL_UARTHAL_H_
#include <inttypes.h>
#include <stdio.h>

int uarthal_init(int uartNumber);
void uarthal_transmit(const char* buffer, int bufferSize, int uartNumber);
int uarthal_receive(char* buffer, int bufferSize, int uartNumber);





#endif /* KERNEL_DRIVERS_UART_HAL_UARTHAL_H_ */
