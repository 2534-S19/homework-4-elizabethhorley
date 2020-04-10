#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "homework4.h"


typedef enum {none, case2, case25, case253} state_machine;
unsigned int myStringLength(char *str);
int main(void)
{
    char rChar;
    char *response = "\n\n\r2534 is the best course in the curriculum!\r\n\n";



    // TODO: Declare the variables that main uses to interact with your state machine.
    bool rTrue;
    int rFalse;
    // Stops the Watchdog timer.
    initBoard();
    // TODO: Declare a UART config struct as defined in uart.h.
    //       To begin, configure the UART for 9600 baud, 8-bit payload (LSB first), no parity, 1 stop bit.
    eUSCI_UART_Config uartConfig = {
             EUSCI_A_UART_CLOCKSOURCE_SMCLK,               // SMCLK Clock Source = 3MHz
             19,                                           // UCBR = 19
             8,                                            // UCBRF = 8
             0xAA,                                         // UCBRS = 0xAA
             EUSCI_A_UART_NO_PARITY,                       // No Parity
             EUSCI_A_UART_LSB_FIRST,                       // LSB First
             EUSCI_A_UART_ONE_STOP_BIT,                    // One stop bit
             EUSCI_A_UART_MODE,                            // UART mode
             EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION // Oversampling
        };


    // TODO: Make sure Tx AND Rx pins of EUSCI_A0 work for UART and not as regular GPIO pins.
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);


    // TODO: Initialize EUSCI_A0
   UART_initModule(EUSCI_A0_BASE, &uartConfig);

    // TODO: Enable EUSCI_A0
   UART_enableModule(EUSCI_A0_BASE);

    while(1)
    {

        bool FSM_state;
        // TODO: Check the receive interrupt flag to see if a received character is available.
        //       Return 0xFF if no character is available.
        if (UART_getInterruptStatus (EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) == EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
            {
                rTrue = true;
                rChar = UART_receiveData(EUSCI_A0_BASE);
            }
        else
            rTrue = false;

        // TODO: If an actual character was received, echo the character to the terminal AND use it to update the FSM.
        //       Check the transmit interrupt flag prior to transmitting the character.
        if (rTrue)
            if (UART_getInterruptStatus (EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)
            {
                UART_transmitData(EUSCI_A0_BASE, rChar);
            }



        // TODO: If the FSM indicates a successful string entry, transmit the response string.
        //       Check the transmit interrupt flag prior to transmitting each character and moving on to the next one.
        //       Make sure to reset the success variable after transmission.
        FSM_state = charFSM(rChar);
        if (FSM_state == true)
        {
            int i = 0;
            while (response[i] != '\0')
            {
                if (UART_getInterruptStatus (EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)
                            {
                                UART_transmitData(EUSCI_A0_BASE, response[i]);
                            }
                i++;
            }
            FSM_state = false;

        }
    }
}

void initBoard()
{
    WDT_A_hold(WDT_A_BASE);
}

// TODO: FSM for detecting character sequence.
bool charFSM(char rChar)
{
    bool finished = false;
     static state_machine changeState = none;
     switch (changeState)
     {
     case none:
         if (rChar == '2')
             changeState = case2;
         break;
     case case2:
         if (rChar == '5')
             changeState = case25;
         else
             changeState = none;
         break;
     case case25:
         if (rChar == '3')
             changeState = case253;
         else
             changeState = none;
         break;
     case case253:
         if (rChar == '4')
             finished = true;
             changeState = none;
         break;

     }


    return finished;
}

