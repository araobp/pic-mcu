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
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.4
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
#include "mcc_generated_files/tmr0.h"
#include "mpu9250.h"
#include <stdint.h>

#define DEBUG false

sensor_data data;
void tmr0_interrupt_handler();

typedef enum {
    SYNCING_H,
    SYNCING_L,
    CMD_H_WAITING,
    CMD_L_WAITING,
    VALUE_WAITING
} cmd_state;

void process_cmd(uint8_t cmd_h, uint8_t cmd_l, uint8_t value) {
    switch(cmd_h) {
        case 'r':
            switch(cmd_l) {
                case 'a':
                     mpu9250_accel_set_range(value);              
                    break;
                case 'g':
                     mpu9250_gyro_set_range(value);
                    break;
            }
            break;
    }
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
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    __delay_ms(300);
    
    // MPU9250 initialization
    mpu9250_i2c_master_disable();
    mpu9250_pass_through_enable();
    ak8963_continous_measurement_mode_1_enable();

    // MPU9250 bandwidth (low pass filter) settings
    mpu9250_gyro_lpf();
    mpu9250_accel_lpf();
    
    TMR0_SetInterruptHandler(tmr0_interrupt_handler);
    
    uint8_t cmd_h;
    uint8_t cmd_l;
    uint8_t value;

    cmd_state state;
    
    state = SYNCING_H;
    
    while (1)
    {
        if (EUSART_DataReady) {
            uint8_t c = EUSART_Read();
            switch(state) {
                case SYNCING_H:
                    if (c == 0xA5) state = SYNCING_L;  // Header H
                    break;
                case SYNCING_L:
                    if (c == 0x5A) state = CMD_H_WAITING;  // Header L
                    break;
                case CMD_H_WAITING:
                    cmd_h = c;
                    state = CMD_L_WAITING;
                    break;
                case CMD_L_WAITING:
                    cmd_l = c;
                    state = VALUE_WAITING;
                    break;
                case VALUE_WAITING:
                    value = c;
                    process_cmd(cmd_h, cmd_l, value);
                    state = SYNCING_H;
                    break;
            }
        }
    }
}

void tmr0_interrupt_handler() {
    LED_Toggle();
    if (DEBUG) {
        mpu9250_accel_set_range(3); 
        mpu9250_gyro_set_range(3); 
        mpu9250_output_cfg_to_uart();
    }
    mpu9250_gyro_read(&data);
    mpu9250_accel_read(&data);
    ak8963_magneto_read(&data);
    mpu9250_output_to_uart(&data, DEBUG);
}

/**
 End of File
*/