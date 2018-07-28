/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65.2
        Device            :  PIC16F1825
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include <stdlib.h>

#define LED_RED LATCbits.LATC3

#define THRESHOLD 50
#define MIDDLE 511
#define CONTINUITY 3

bool on = false;
uint16_t sum = 0;
uint16_t count = 0;
uint8_t threshold = THRESHOLD;
uint8_t continuity = CONTINUITY;

/*
                         Main application
 */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();  

    while (1)
    {
        if (TMR0_HasOverflowOccured()) {
            TMR0IF = 0;
            ADC_SelectChannel(channel_AN3);
            ADC_StartConversion();
            while(!ADC_IsConversionDone());
            adc_result_t out = ADC_GetConversionResult();
            uint16_t v = abs(out - MIDDLE);    
            /*
             *  Count condition
             *  "^": satisfied
             *  "V": unsatisfied
             * 
             *                                   +--> sum++ and blink LED
             *                                  |
             *  on              ^ ^ ^ ^ V ^ V V V     
             * 
             *  off   ^ V ^ ^ ^                   V V V ...
             * 
             *  count 1 0 1 2 3 0 0 0 0 1 0 1 2 3 0 0 0 ...
             * 
             *       -------------------------------------> Time
             */
            switch (on) {
                case false:
                    if (v >= THRESHOLD) count++;
                    else count = 0;
                    if (count >= CONTINUITY) {
                        count = 0;
                        on = true;
                    }
                    break;
                case true:
                    if (v < THRESHOLD) count++;
                    else count = 0;
                    if (count >= CONTINUITY) {
                        count = 0;
                        on = false;
                    LED_RED = 0;
                    __delay_ms(3);
                    LED_RED = 1;
                    sum++;
                    }
                    break;
            }
        }
    }
}

void I2C_StatusCallback(I2C_SLAVE_DRIVER_STATUS i2c_bus_state)
{
    switch (i2c_bus_state)
    {
        case I2C_SLAVE_READ_REQUEST:
            SSP1BUF = sum;
            sum = 0;
            break;

        case I2C_SLAVE_READ_COMPLETED:
        default:
            ;
    }
}

/**
 End of File
*/