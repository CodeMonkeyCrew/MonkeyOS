#include <src/io-handler/io-handler.h>

int mos_io_handler_write(int gpioPinNumber, int outputMode, int on){
    writeGPIO(gpioPinNumber, outputMode, on);
    return 1;
}