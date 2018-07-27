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
        Device            :  PIC16F18326
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
#include "eeprom.h"
#include "mpu9250.h"
#include <stdint.h>

#define DIGIT_0 LATAbits.LATA0
#define DIGIT_1 LATAbits.LATA1
#define DIGIT_2 LATAbits.LATA2
#define LED_RED LATCbits.LATC3

#define BUTTON PORTCbits.RC2

void show_serial_num(uint8_t num) {
    DIGIT_0 = num & 0b0001U;
    DIGIT_1 = (num >> 1) & 0b0001U;
    DIGIT_2 = (num >> 2) & 0b0001U;
}

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

    __delay_ms(1000);
    printf("start\n");
    uint16_t data_address = 0;
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t k = 0;
    uint8_t l = 0;
    uint8_t data_buf[60];
    uint8_t test_buf[60];

    uint8_t who = mpu9250_who_am_i();
    if (who == WHO_AM_I_RESPONSE) {
        printf("MPU9255 OK\n");
    }
    
    uint8_t status;
    status = mpu9250_gyro_lpf();
    printf("gyro lpf status: %d\n", status);
    status = mpu9250_accel_lpf();
    printf("accel lpf status: %d\n", status);
    
    uint8_t gyro_data[6];
    uint8_t accel_data[6];
    
    while (1)
    {
        if (TMR0_HasOverflowOccured()) {
            TMR0IF = 0;
            mpu9250_gyro_read(gyro_data, 6);
            mpu9250_accel_read(accel_data, 6);
            printf("Gyro  %x:%x, %x:%x, %x:%x\n",\
                    gyro_data[0], gyro_data[1],\
                    gyro_data[2], gyro_data[3],\
                    gyro_data[4], gyro_data[5]);
            printf("Accel %x:%x, %x:%x, %x:%x\n",\
                    accel_data[0], accel_data[1],\
                    accel_data[2], accel_data[3],\
                    accel_data[4], accel_data[5]);
            
        }
        
        // while(BUTTON == 0);
        /*
        for (l = 0; l < 8; l++) {  // Max 8 sessions (8 * 3sec = 24sec)
            for (k = 0; k < 240; k++) {  // 80 (measurement/sec=Hz) * 3 (sec): total 23kbits
                // Sensor
                for (i = j; i < 12+j; i++) {  // 12 bytes of data
                    data_buf[i] = i;  // Dummy
                }
                j += 12;
                if (j >= 60) {
                    uint8_t status = eeprom_page_write(data_address, data_buf, 60);
                    eeprom_sequential_read(data_address, test_buf, 60);
                    printf("\nl: %u, k: %u\n", l, k);
                    for (i = 0; i < 60; i++) {
                        printf("%u ", test_buf[i]);
                    }
                    data_address += 64;
                    j = 0;
                    __delay_ms(1);
                    LED_RED ^= 1;
                }
            }
       }
       */
    }
}
/**
 End of File
*/