#include "../../register/cm_register.h"
#include "uarthal.h"

#define UART_NUM 4
#define UART1_BASE_ADDRESS 0x4806A000
#define UART2_BASE_ADDRESS 0x4806C000
#define UART3_BASE_ADDRESS 0x49020000
#define UART4_BASE_ADDRESS 0x49042000
#define DLL_REG 0x0
#define RHR_REG 0x0
#define THR_REG 0x0
#define DLH_REG 0x4
#define IER_REG 0x4
#define FCR_REG 0x8
#define EFR_REG 0x8
#define LCR_REG 0xC
#define MCR_REG 0x10
#define LSR_REG 0x14
#define TCR_REG 0x18
#define TLR_REG 0x1C
#define MDR1_REG 0x20
#define SCR_REG 0x40
#define SYSC_REG 0x54
#define SYSS_REG 0x58
#define EN_UART1 13
#define EN_UART2 14
#define EN_UART3 11
#define EN_UART4 16

typedef struct
{
    uint32_t* FCLKEN;
    uint32_t* ICLKEN;
    uint32_t* SYSC;
    uint32_t* SYSS;
    uint32_t* LCR;
    uint32_t* EFR;
    uint32_t* MCR;
    uint32_t* FCR;
    uint32_t* TLR;
    uint32_t* SCR;
    uint32_t* MDR1;
    uint32_t* IER;
    uint32_t* DLL;
    uint32_t* DLH;
    uint32_t* TCR;
    uint32_t* LSR;
    uint32_t* THR;
    char* RHR;
    uint32_t EN;
} UART_T;

#define createUart(fclken, iclken, baseAddress, enBit) {\
    (uint32_t*) fclken,\
    (uint32_t*) iclken,\
    (uint32_t*) (baseAddress + SYSC_REG),\
    (uint32_t*) (baseAddress + SYSS_REG),\
    (uint32_t*) (baseAddress + LCR_REG),\
    (uint32_t*) (baseAddress + EFR_REG),\
    (uint32_t*) (baseAddress + MCR_REG),\
    (uint32_t*) (baseAddress + FCR_REG),\
    (uint32_t*) (baseAddress + TLR_REG),\
    (uint32_t*) (baseAddress + SCR_REG),\
    (uint32_t*) (baseAddress + MDR1_REG),\
    (uint32_t*) (baseAddress + IER_REG),\
    (uint32_t*) (baseAddress + DLL_REG),\
    (uint32_t*) (baseAddress + DLH_REG),\
    (uint32_t*) (baseAddress + TCR_REG),\
    (uint32_t*) (baseAddress + LSR_REG),\
    (uint32_t*) (baseAddress + THR_REG),\
    (char*) (baseAddress + RHR_REG),\
    (uint32_t) (enBit)\
}

static UART_T uarts[UART_NUM] =
        {
        createUart(CM_FCLKEN1_CORE, CM_ICLKEN1_CORE, UART1_BASE_ADDRESS, EN_UART1),
          createUart(CM_FCLKEN1_CORE, CM_ICLKEN1_CORE, UART2_BASE_ADDRESS,EN_UART2),
          createUart(CM_FCLKEN_PER, CM_ICLKEN_PER, UART3_BASE_ADDRESS, EN_UART3),
          createUart(CM_FCLKEN_PER, CM_ICLKEN_PER, UART4_BASE_ADDRESS, EN_UART4) };
static const char *keyWords[] = { "execute", "pc" };

int uarthal_receive(char* buffer, int bufferSize, int uartNumber)
{

    UART_T uart = uarts[uartNumber];
    int end = 0;
    char startSign[1] = ">";

    int i;
    for (i = 0; ((i < bufferSize - 1) && (!end)); i++)
    {
        while (!(*uart.LSR & (1 << 0)))
        {
            //wait until there is something to read
        }

        buffer[i] = *uart.RHR;
        if (buffer[i] == '\n' || buffer[i] == '\r')
        {
            end = 1;

        }

    }

    buffer[i] = '\0';
    if (end)
    {
        uarthal_transmit(startSign, 1, uartNumber);
    }
    printf(buffer);
    printf("\n");
    return i;

}

int interpretMessage(char* buffer, int bufferSize)
{
    //split message
    char* split = strtok(buffer, " ");
    printf(split);
  /*  switch (split[0])
    {
    case "execute":
        printf("execute");
        break;
    case "pc":
        printf("pc");
        break;
    }*/

    return 1;
}
void uarthal_transmit(const char* buffer, int bufferSize, int uartNumber)
{
    UART_T uart = uarts[uartNumber];
    int i;
    for (i = 0; i < bufferSize; i++)
    {
        while (!(*uart.LSR & (1 << 5)))
        {

        }
        *uart.THR = buffer[i];
    }

}

void softwareReset(UART_T uart)
{
    //UART Steps described on SPRUNG Page 2928
    //Software Reset
    *uart.SYSC |= (1 << 1);
    uint32_t RESETDONE = (1 << 0);
    while (!(*uart.SYSS & RESETDONE))
    {
        //wait till buffer has enough space to write into it
    }
}

void fifoDMASettings(UART_T uart)
{
    //Fifos and DMA settings
    uint32_t lcr_reg_before = *uart.LCR;
    *uart.LCR = 0x00BF;
    uint32_t EFR_MASK = (1 << 4);
    uint32_t efr_reg_before = (*uart.EFR & EFR_MASK);
    *uart.EFR |= (1 << 4);
    *uart.LCR = 0x0080;
    uint32_t MCR_MASK = (1 << 6);
    uint32_t mcr_reg_before = *uart.MCR & MCR_MASK;
    *uart.MCR |= (1 << 6);

    *uart.FCR &= ~(1 << 0);      //changed!!
    *uart.FCR |= (1 << 3);
    *uart.FCR |= (1 << 4);
    *uart.FCR |= (1 << 5);
    *uart.FCR |= (1 << 6);
    *uart.FCR |= (1 << 7);

    *uart.LCR = 0x00BF;
    *uart.TLR &= ~(1 << 0);
    *uart.TLR &= ~(1 << 1);
    *uart.TLR &= ~(1 << 2);
    *uart.TLR |= (1 << 3);
    *uart.TLR &= ~(1 << 4);
    *uart.TLR &= ~(1 << 5);
    *uart.TLR &= ~(1 << 6);
    *uart.TLR |= (1 << 7);

    *uart.SCR |= (1 << 0);
    //0x1: DMA mode 1 (UARTi_DMA_TX, UARTi_DMA_RX)
    *uart.SCR |= (1 << 1);
    *uart.SCR &= ~(1 << 2);

    *uart.SCR |= (1 << 6);
    *uart.SCR |= (1 << 7);
    if (efr_reg_before == 0)
    {
        *uart.EFR &= ~(1 << 4);

    }
    else
    {
        *uart.EFR |= (1 << 4);
    }
    *uart.LCR = 0x0080;
    if (mcr_reg_before == 0)
    {
        *uart.MCR &= ~(1 << 6);

    }
    else
    {
        *uart.MCR |= (1 << 6);
    }
    *uart.LCR = lcr_reg_before;
}

void connectionSettings(UART_T uart)
{
    //19.5.1.1.3 Protocol, Baud Rate, and Interrupt Settings
    *uart.MDR1 |= (1 << 0);
    *uart.MDR1 |= (1 << 1);
    *uart.MDR1 |= (1 << 2);

    *uart.LCR = 0x00BF;
    uint32_t EFR_MASK = (1 << 4);
    uint32_t efr_reg_before = (*uart.EFR & EFR_MASK);
    *uart.EFR |= (1 << 4);
    *uart.LCR = 0x0000;
    *uart.IER = 0x0000;
    *uart.LCR = 0x00BF;

    //Seite 2913
    *uart.DLH |= (1 << 0);
    *uart.DLH &= ~(1 << 1);
    *uart.DLH &= ~(1 << 2);
    *uart.DLH &= ~(1 << 3);
    *uart.DLH &= ~(1 << 4);
    *uart.DLH &= ~(1 << 5);

    *uart.DLL &= ~(1 << 0);
    *uart.DLL &= ~(1 << 1);
    *uart.DLL &= ~(1 << 2);
    *uart.DLL |= (1 << 3);
    *uart.DLL |= (1 << 4);
    *uart.DLL |= (1 << 5);
    *uart.DLL &= ~(1 << 6);
    *uart.DLL &= ~(1 << 7);

    *uart.LCR = 0x0000;
    //disable all interrupts
    *uart.IER &= ~(1 << 0);
    *uart.IER &= ~(1 << 1);    //THR interrupt
    *uart.IER &= ~(1 << 2);
    *uart.IER &= ~(1 << 3);
    *uart.IER &= ~(1 << 4);
    *uart.IER &= ~(1 << 5);
    *uart.IER &= ~(1 << 6);
    *uart.IER &= ~(1 << 7);

    *uart.LCR = 0x00BF;
    //restore
    if (efr_reg_before == 0)
    {
        *uart.EFR &= ~(1 << 4);

    }
    else
    {
        *uart.EFR |= (1 << 4);
    }
    *uart.LCR &= ~(1 << 6);
    *uart.LCR &= ~(1 << 7);

    *uart.LCR |= (1 << 0);
    *uart.LCR |= (1 << 1);
    //NB_STOP = 1 Stop bit
    *uart.LCR &= ~(1 << 2);
    //PARITY_EN no parity bit
    *uart.LCR &= ~(1 << 3);
    //set UART Mode
    *uart.MDR1 &= ~(1 << 0);
    *uart.MDR1 &= ~(1 << 1);
    *uart.MDR1 &= ~(1 << 2);
}

void hardwareFlowControl(UART_T uart)
{
    //Hardware flow control
    uint32_t lcr_reg_before = *uart.LCR;
    *uart.LCR = 0x0080;
    uint32_t MCR_MASK = (1 << 6);
    uint32_t mcr_reg_before = *uart.MCR & MCR_MASK;
    *uart.MCR |= (1 << 6);
    *uart.LCR = 0x00BF;
    uint32_t EFR_MASK = (1 << 4);
    uint32_t efr_reg_before = *uart.EFR & EFR_MASK;
    *uart.EFR |= (1 << 4);

    *uart.TCR = 0xF0;

    *uart.EFR |= (1 << 6);
    *uart.EFR |= (1 << 7);
    if (efr_reg_before == 0)
    {
        *uart.EFR &= ~(1 << 4);
    }
    else
    {
        *uart.EFR |= (1 << 4);
    }
    *uart.LCR = 0x0080;
    if (mcr_reg_before == 0)
    {
        *uart.MCR &= ~(1 << 6);
    }
    else
    {
        *uart.MCR |= (1 << 6);
    }
    *uart.LCR = lcr_reg_before;
}

int uarthal_init(int uartNumber)
{
    if (uartNumber < 0 || uartNumber > UART_NUM)
    {
        return 0;
    }
    UART_T uart = uarts[uartNumber];

    *uart.ICLKEN |= (1 << uart.EN);
    *uart.FCLKEN |= (1 << uart.EN);
    softwareReset(uart);
    fifoDMASettings(uart);
    connectionSettings(uart);
    hardwareFlowControl(uart);
    return 1;
}

