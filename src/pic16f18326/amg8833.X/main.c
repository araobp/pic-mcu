/*
 * Motion detection with AMG8833 and TWELITE
 * 
 * Ver 1.0      January 30, 2019
 */

#include "mcc_generated_files/mcc.h"
#include "amg8833.h"
#include "twelite.h"
#include <stdbool.h>

// FET control
#define LOW 0
#define HIGH 1
#define FET1_GATE LATCbits.LATC2
#define FET2_GATE LATAbits.LATA2

// Timers for power management
#define T_1 5       // 5 sec
#define T_2 180     // 180 sec (3 min)
#define T_3 180     // 180 sec (3 min)

// Addresses on Built-in EEPROM
#define EEPROM_HEAD_ADDR 0x7000
#define MODE_ADDR 0
#define THRES_ADDR 1

// Power saving state machine

typedef enum {
    CONNECTING, RUNNING, SLEEPING
} state_machine;

typedef enum {
    CHECK, KEEP_ON
} state_machine_command;

typedef enum {
    REACTIVE, PASSIVE
} operation_mode;

// Convert timers into 125msec time period unit
const int t1 = T_1 * 8;
const int t2 = T_2 * 8;
const int t3 = T_3 * 8;

// Buffers
uint8_t buf[AMG8833_PIXELS_LENGTH];
uint8_t buf_prev[AMG8833_PIXELS_LENGTH / 2];
int8_t diff[AMG8833_PIXELS_LENGTH / 2];
int8_t map[AMG8833_PIXELS_LENGTH / 2];

// Mode
operation_mode mode = REACTIVE;

/**
 * Power management
 * @param reset true to reset the timeout counter
 */
void power_mgmt(state_machine_command command) {
    static int timeout_cnt = 0;
    static state_machine state = CONNECTING;

    //--- Power management disabled by jumper pin -----------------------------
    // "Weak Pull Up (WPU) is enabled and jumper pin is off"
    if (PORTAbits.RA4 == LOW) return;

    //--- command: KEEP_ON ----------------------------------------------------
    if (command == KEEP_ON && (state == CONNECTING || state == RUNNING)) {
        timeout_cnt = 0;
        state = RUNNING;
        return;
    }

    //--- command: CHECK ------------------------------------------------------
    if (TMR0_HasOverflowOccured()) { // every 125msec
        timeout_cnt++;
        TMR0IF = 0;
    }

    switch (state) {
        case CONNECTING:
            if (timeout_cnt >= t1) {
                timeout_cnt = 0;
                FET1_GATE = LOW;
                state = SLEEPING;
            }
            break;
        case RUNNING:
            if (timeout_cnt >= t2) {
                timeout_cnt = 0;
                FET1_GATE = LOW;
                state = SLEEPING;
            }
            break;
        case SLEEPING:
            if (timeout_cnt >= t3) {
                FET1_GATE = HIGH;
                __delay_ms(500);
                set_moving_average(true);
                timeout_cnt = 0;
                state = CONNECTING;
            }
            break;
        default:
            break;
    }
}

void main(void) {

    uint8_t c, cmd, thres, settings[2];
    uint8_t seq;
    int8_t sum[8] = {0};
    int8_t row[8] = {0};
    bool notify_flag;

    FET1_GATE = LOW;
    FET2_GATE = LOW;

    SYSTEM_Initialize();
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    // Initialization
    mode = DATAEE_ReadByte(EEPROM_HEAD_ADDR + MODE_ADDR);
    if (mode != REACTIVE && mode != PASSIVE) {
        mode = REACTIVE;
        DATAEE_WriteByte(EEPROM_HEAD_ADDR + MODE_ADDR, mode);
    }
    thres = DATAEE_ReadByte(EEPROM_HEAD_ADDR + THRES_ADDR);
    if (thres > 6) {
        thres = 0;
        DATAEE_WriteByte(EEPROM_HEAD_ADDR + THRES_ADDR, thres);
    }
    calibrate_threshold(thres);

    // Start supplying power to TWELITE-DIP and AMG8833 
    FET1_GATE = HIGH;
    FET2_GATE = HIGH;
    __delay_ms(100);
    set_moving_average(true);

    while (1) {

        // Periodic task
        if (mode == PASSIVE) {
            if (TMR0_HasOverflowOccured()) { // every 250msec
                TMR0IF = 0;
                read_motion(buf, buf_prev, diff, row);
                notify_flag = false;
                for (int i = 0; i < 8; i++) {
                    if (row[i] != 0) notify_flag = true;
                }
                if (notify_flag) {
                    twelite_uart_tx((uint8_t *) row, seq, 8);
                    notify_flag = false;
                }
            }
        } else if (mode == REACTIVE) {
            power_mgmt(CHECK);
        }

        // Command from master node
        if (EUSART_DataReady) {
            c = EUSART_Read();
            if (twelite_uart_rx(c, &cmd, &seq)) {
                switch (cmd) {
                    /*** Reactive mode */
                    case 't': // Thermistor
                        read_thermistor(buf);
                        twelite_uart_tx(buf, seq, AMG8833_THERMISTOR_LENGTH);
                        break;
                    case 'p': // 64 pixels
                        read_pixels(buf);
                        twelite_uart_tx(buf, seq, AMG8833_PIXELS_LENGTH_HALF);
                        break;
                    case 'd': // 64 pixels diff
                        read_pixels_diff(buf, buf_prev, diff);
                        twelite_uart_tx((uint8_t *) diff, seq, AMG8833_PIXELS_LENGTH_HALF);
                        break;
                    case 'm': // Column-wise motion detection
                        read_pixels_motion(buf, buf_prev, diff);
                        twelite_uart_tx((uint8_t *) diff, seq, AMG8833_PIXELS_LENGTH_HALF);
                        break;
                    case 'M': // Motion count on a specific row
                        read_motion(buf, buf_prev, diff, row);
                        twelite_uart_tx((uint8_t *) row, seq, sizeof (row));
                        break;
                    case 'o':  // Object map
                        read_object(buf, buf_prev, diff, map);
                        twelite_uart_tx((uint8_t *) map, seq, sizeof (map));
                        break;                        
                    /*** Passive mode ***/
                    case 'n': // Notify motion count (reactive -> passive mode)
                        mode = PASSIVE;
                        DATAEE_WriteByte(EEPROM_HEAD_ADDR + MODE_ADDR, mode);
                        break;
                    case 'N': // Disable notifications (passive -> reactive mode)
                        mode = REACTIVE;
                        DATAEE_WriteByte(EEPROM_HEAD_ADDR + MODE_ADDR, mode);
                        break;
                    /*** Management ***/                        
                    case 's': // Dump setting parameters
                        settings[MODE_ADDR] = DATAEE_ReadByte(EEPROM_HEAD_ADDR + MODE_ADDR);
                        settings[THRES_ADDR]
                                = DATAEE_ReadByte(EEPROM_HEAD_ADDR + THRES_ADDR);
                        twelite_uart_tx((uint8_t *) settings, seq, sizeof (settings));
                        break;
                    default: // Calibrate motion detection parameters
                        if (cmd >= '0' && cmd <= '9') {
                            thres = cmd - 0x30; // ASCII code - 0x30
                            calibrate_threshold(thres);
                            DATAEE_WriteByte(EEPROM_HEAD_ADDR + THRES_ADDR, thres);
                        }
                        break;
                }
                power_mgmt(KEEP_ON);
            }
        }
    }
}