#include <stdio.h>
#include <stdlib.h>
#include "uartdriver.h"
#include "../drivertypes.h"
#include "hal/uarthal.h"
#include "../../filesystem/filetypes/uarttype.h"
#include "../../filesystem/filesystemregister.h"

#define false 0
#define true !false

static int uartdriver_open(generic_file_t* file){
    uart_file_t* uartfile = (uart_file_t*)file;
    uarthal_init(uartfile->uartNumber);
    return 1;
}

static int uartdriver_write(const void* buffer, int bufSize, generic_file_t* file){
    uart_file_t* uartfile = (uart_file_t*)file;
    uarthal_transmit((char*) buffer, bufSize, uartfile->uartNumber);
    return 1;
}

static int uartdriver_read(void* buffer, int bufSize, generic_file_t* file){
    uart_file_t* uartfile = (uart_file_t*)file;
    return uarthal_receive(buffer, bufSize, uartfile->uartNumber);
   // return 1;
}

static void registerDriver(void){
    driver_t* pUartDriver = (driver_t*) malloc(sizeof(driver_t));
      if (pUartDriver != NULL) {
          pUartDriver->driver_read = uartdriver_read;
          pUartDriver->driver_write = uartdriver_write;
          pUartDriver->driver_open = uartdriver_open;
          register_driver(UART, pUartDriver);
      }
}



void createUART3File(void){
    uart_file_t* puart3 = (uart_file_t*) mos_fs_create_file(UART);
       if (puart3 != NULL) {
          strcpy(puart3->header.name, "uart3");
          puart3->header.is_open = false;
          puart3->header.f_type = UART;
          puart3->header.size = 0;
          //Attention: uartnumber is index of "uarts" array, which is 0-initialized
          puart3->uartNumber = 2;
       }
}
static void add_files(){
    createUART3File();
}

//should be called at boot time
void uartdriver_init(void){

    registerDriver();
    add_files();
}
