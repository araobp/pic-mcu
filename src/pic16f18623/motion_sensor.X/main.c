#include "mcc_generated_files/mcc.h"
#include "eeprom.h"
#include "mpu9250.h"
#include <stdint.h>

#define DIGIT_0 LATAbits.LATA0
#define DIGIT_1 LATAbits.LATA1
#define DIGIT_2 LATAbits.LATA2
#define LED_RED LATCbits.LATC3

#define START_BUTTON PORTCbits.RC2

void show_serial_num(uint8_t num) {
    DIGIT_0 = num & 0b0001U;
    DIGIT_1 = (num >> 1) & 0b0001U;
    DIGIT_2 = (num >> 2) & 0b0001U;
}

void main(void)
{
    uint16_t data_address = 0;
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t data_buf[61];
    bool measure = false;
    uint8_t status;

    SYSTEM_Initialize();
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    /*
    uint8_t who = mpu9250_who_am_i();
    if (who == WHO_AM_I_RESPONSE) {
        printf("MPU9255 OK\n");
    }
    */
    
    __delay_ms(1000);
    
    status = mpu9250_gyro_lpf();
    //printf("gyro lpf status: %d\n", status);
    status = mpu9250_accel_lpf();
    //printf("accel lpf status: %d\n", status);
    
    while (1)
    {
        if (START_BUTTON == 0 && !measure) {  // check if start button is pressed
            measure = true;
            i = 0;
            j = 0;
            data_address = 0;
        }
        
        if (TMR0_HasOverflowOccured()) {  // every 12.5msec
            TMR0IF = 0;
            if (measure) {
                mpu9250_gyro_read(&data_buf[i], 6);
                i += 6;
                mpu9250_accel_read(&data_buf[i], 6);
                i += 6;
                if (i >= 60) {  // 5 times corresponds to 1/16 sec
                    // TODO: fetch speed pulese here
                    // data_buf[i] = speed pulses
                    uint8_t status = eeprom_page_write(data_address, data_buf, 61);
                    data_address += 64;
                    i = 0;
                    j += 1;
                }
                if (j >= 52) {  // 5 * 52 times corresponds to 3.25 sec (260 samples)
                    measure = false;
                }
            }
        }
        
        if (EUSART_DataReady && !measure) {
            char c = EUSART_Read();
            if (c == 'd') {
                data_address = 0;
                for (j = 0; j < 52; j++) {
                    status = eeprom_sequential_read(data_address, data_buf, 61);
                    data_address += 64;
                    for (i = 0; i < 60; i++) {
                        printf("%5d,", data_buf[i]);
                    }
                    printf("%5d\n", data_buf[60]);
                }
            }
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
