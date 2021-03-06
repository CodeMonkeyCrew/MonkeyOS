#include "gpiohal.h"
#include "gpioregister.h"
#include "../../util/registerutil.h"


void matrix_gpiohal_pinInit(const uint8_t pinNumber)
{
    /*Clear higher part(bit16-bit32) of 32bit register if its even */
    uint8_t offset = 16;
    if (!(pinNumber % 2))
    {
        offset = 0;

    }

    uint8_t gpioRegion = GPIO_REGION(pinNumber);
    if (gpioRegion == 1)
    {
        /* Enable the access on the GPIO1 register*/
       // set_bit((uint32_t*) CM_FCLKEN_WKUP, 3);
        set_bit((uint32_t*) CM_ICLKEN_WKUP, 3);

    }
    else
    {
        /*  Functional Clock for GPIO is in range from bit3 to bit10 in the register CM_ICKLEN_PER
         *
         * */
        set_bit((uint32_t*) CM_FCLKEN_PER, (11+gpioRegion ));
        set_bit((uint32_t*) CM_ICLKEN_PER, (11+gpioRegion));
    }
    uint32_t* pad_conf_address = (uint32_t*)PAD_CONF_ADDRESS(pinNumber);
    clear_bit(pad_conf_address, offset);
    clear_bit(pad_conf_address, offset + 1);
    set_bit(pad_conf_address, offset + 2);
}

void matrix_gpiohal_pinSetDirection(const uint8_t pinNumber,const uint8_t direction)
{
    uint8_t pinNumber_Region = GPIO_PIN_ON_REGION(pinNumber);
    uint32_t* gpioOE = get_address(GPIOi(GPIO_REGION(pinNumber)),GPIO_OE);
    if (direction)
    {
        //input Pin
              set_bit(gpioOE, pinNumber_Region);
    }
    else
    {
        //output Pin
               clear_bit(gpioOE, pinNumber_Region);

    }
}

void matrix_gpiohal_pinSetValue(const uint8_t pinNumber, const uint8_t value){
    uint8_t pinNr_on_regio = GPIO_PIN_ON_REGION(pinNumber);
    if(value){
        set_bit(get_address(GPIOi(GPIO_REGION(pinNumber)), GPIO_DATAOUT), pinNr_on_regio);
    }else{
        clear_bit(get_address(GPIOi(GPIO_REGION(pinNumber)), GPIO_DATAOUT), pinNr_on_regio);
    }
}

uint8_t matrix_gpiohal_pinGetValue(const uint8_t pinNumber){
    uint8_t result = 0;
     if(*get_address(GPIOi(GPIO_REGION(pinNumber)), GPIO_DATAOUT) & (1 << GPIO_PIN_ON_REGION(pinNumber))){
         result = 1;
     }

    return result;
}
