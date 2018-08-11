#include "mcc_generated_files/mcc.h"
#include "eeprom.h"
#include "mpu9250.h"
#include "speed.h"
#include <stdint.h>

// false: EEPROM output mode (default)
// true: UART output mode
#define UART_OUTPUT false

// Serial number indicator (Green LEDs)
#define DIGIT_0 LATAbits.LATA0
#define DIGIT_1 LATAbits.LATA1
#define DIGIT_2 LATAbits.LATA2
// Operation status indicator (RED LED)
#define LED_RED LATCbits.LATC3
// LED on/off
#define ON 1U
#define OFF 0U

// Short the pins with a tactile switch to start a measurement
#define START_BUTTON PORTCbits.RC2

// Built-in EEPROM NVM start address
#define EEPROM_NVMADR 0x7000U

// Nine-axis motion sensor sampling rate
#define SAMPLING_RATE 80U  // 80Hz
// Nyquist rate
const uint8_t HALF = SAMPLING_RATE / 2;

// Three green LEDs as serial number
void show_serial_num(uint8_t num) {
    DIGIT_0 = ((num & 0b00000001U) > 0) ? ON: OFF;
    DIGIT_1 = ((num & 0b00000010U) > 0) ? ON: OFF;
    DIGIT_2 = ((num & 0b00000100U) > 0) ? ON: OFF;
}

void output_to_uart(uint32_t cnt, char label, uint8_t *pbuf) {
    static uint32_t c = 0xffffU;
    int16_t gx, gy, gz, ax, ay, az;
    if (label == 'g') {
        c = cnt;
        gx = (int16_t)((pbuf[0] << 8) | pbuf[1]);
        gy = (int16_t)((pbuf[2] << 8) | pbuf[3]);
        gz = (int16_t)((pbuf[4] << 8) | pbuf[5]);
        printf("%ld,%d,%d,%d,", cnt, gx, gy, gz);            
    } else if (label == 'a' && c == cnt) {
        ax = (int16_t)((pbuf[0] << 8) | pbuf[1]);
        ay = (int16_t)((pbuf[2] << 8) | pbuf[3]);
        az = (int16_t)((pbuf[4] << 8) | pbuf[5]);
        printf("%d,%d,%d\n", ax, ay, az);            
    }
}

void main(void)
{
    uint16_t data_address = 0;
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t k = 0;
    uint8_t data_buf[61];
    bool measure = false;
    uint8_t status;
    uint32_t cnt=0;

    SYSTEM_Initialize();
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    /*
    uint8_t who = mpu9250_who_am_i();
    if (who == WHO_AM_I_RESPONSE) {
        printf("MPU9255 OK\n");
    }
    */
    
    LED_RED = ON;
    __delay_ms(1000);
    LED_RED = OFF;
    
    status = mpu9250_gyro_lpf();
    //printf("gyro lpf status: %d\n", status);
    status = mpu9250_accel_lpf();
    //printf("accel lpf status: %d\n", status);
    
    while (1)
    {
        if (START_BUTTON == 0 && !measure) {  // check if start button is pressed
            if (k < 8) {
                i = 0;
                j = 0;
                measure = true;
                show_serial_num(k);
            } else {  // LED blink as alert
                for (i = 0; i < 6; i++) {
                    show_serial_num(0);
                    __delay_ms(250);
                    show_serial_num(7);
                    __delay_ms(250);                    
                }
            }
        }
        
        if (TMR0_HasOverflowOccured()) {  // every 12.5msec
            TMR0IF = 0;
            if (UART_OUTPUT) {
                mpu9250_gyro_read(&data_buf[0], 6);
                output_to_uart(cnt, 'g', &data_buf[0]);
                mpu9250_accel_read(&data_buf[6], 6);
                output_to_uart(cnt++, 'a', &data_buf[6]);
                if (++i >= HALF) {
                    LED_RED ^= 1;
                    i = 0;
                    show_serial_num(j++);
                }
            }
            else if (measure) {
                LED_RED = ON;
                mpu9250_gyro_read(&data_buf[i], 6);
                i += 6;
                mpu9250_accel_read(&data_buf[i], 6);
                i += 6;
                if (i >= 60) {  // 5 times corresponds to 1/16 sec
                    //data_buf[i] = fetch_speed_pulses();
                    status = eeprom_page_write(data_address, data_buf, 61);
                    data_address += 64;
                    i = 0;
                    j += 1;
                }
                if (j >= 52) {  // 5 * 52 times corresponds to 3.25 sec (260 samples)
                    measure = false;
                    DATAEE_WriteByte(EEPROM_NVMADR, k++);
                    LED_RED = OFF;
                }
            }
        }
        
        if (EUSART_DataReady && !measure) {
            char c = EUSART_Read();
            if (c == 'd') {
                LED_RED = ON;
                data_address = 0;
                uint8_t measurements = (DATAEE_ReadByte(EEPROM_NVMADR) & 0b00000111) + 1;
                for (k = 0; k < measurements; k++) {
                    cnt = 0;
                    printf("id,cnt,gx,gy,gz,ax,ay,az,pls\n");
                    for (j = 0; j < 52; j++) {
                        status = eeprom_sequential_read(data_address, data_buf, 61);
                        data_address += 64;
                        i = 0;
                        uint8_t pls = data_buf[60];
                        for (int h = 0; h < 5; h++) {
                            printf("%d,%ld,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                                    k, cnt++,
                                    data_buf[i], data_buf[i+1], data_buf[i+2], data_buf[i+3], data_buf[i+4], data_buf[i+5],
                                    data_buf[i+6], data_buf[i+7], data_buf[i+8], data_buf[i+9], data_buf[i+10], data_buf[i+11],
                                    pls
                                    );
                            i += 12;
                        }
                    }
                    printf("\n");                   
                }
                LED_RED = OFF;
            }
        }
    }
}
