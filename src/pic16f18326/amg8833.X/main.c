/*
 * Motion detection with AMG8833 and TWELITE
 * 
 * Ver 1.1X      February XX, 2019
 */

#include "mcc_generated_files/mcc.h"
#include "amg8833.h"
#include "twelite.h"
#include <stdbool.h>

// FET control for power management
#define LOW 0  // Tunr off
#define HIGH 1 // Turn on
#define FET1_GATE LATCbits.LATC2  // Power supply to TWELITE-DIP
#define FET2_GATE LATAbits.LATA2  // Power supply to AMG8833

// Timers for power management
#define T_1 5       // 5 sec
#define T_2 180     // 180 sec (3 min)
#define T_3 180     // 180 sec (3 min)
#define T_4 5       // 5 sec
#define TWELITE_STARTUP_TIME 10       // 10 msec
#define TWELITE_TRANSMISSION_TIME 40  // 40 msec
#define AMG8833_STARTUP_TIME 200      // 200 msec

// Enabling/disabling power management by juper pin
#define POWER_MGMT_FLAG PORTAbits.RA4

// Addresses on PIC16F1's built-in EEPROM
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
    JUST_STARTED, NORMAL
} system_state;

// Operation mode: reactive or passive
typedef enum {
    REACTIVE, PASSIVE_MOTION, PASSIVE_DIFF
} operation_mode;

// Convert timers into 125msec time period unit for power management in reactive mode
const int t1 = T_1 * 8;
const int t2 = T_2 * 8;
const int t3 = T_3 * 8;
const int t4 = T_4 * 8;

// Buffers
/*
uint8_t buf[AMG8833_PIXELS_LENGTH];
uint8_t buf_prev[AMG8833_PIXELS_LENGTH / 2];
int8_t diff[AMG8833_PIXELS_LENGTH / 2];
*/

amg8833_instance A;

// Operation mode
operation_mode mode = REACTIVE;

// System state
system_state state = JUST_STARTED;

/**
 * Power management in reactive mode
 * @param reset true to reset the timeout counter
 */
void power_mgmt(state_machine_command command) {
    static int timeout_cnt = 0;
    static state_machine state = CONNECTING;

    //--- Power management disabled by jumper pin -----------------------------
    // "The digital-in pin is pulled up and the jumper pin is off"
    if (!POWER_MGMT_FLAG) return;

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
                __delay_ms(AMG8833_STARTUP_TIME);
                set_moving_average(&A, true);
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
    int notify_timer = 0;
    
    uint8_t *pbuf;
    int8_t *pdiff;
    int8_t *prow;

    state = JUST_STARTED;
    
    FET1_GATE = LOW;
    FET2_GATE = LOW;

    SYSTEM_Initialize();
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    // Initialization
    mode = DATAEE_ReadByte(EEPROM_HEAD_ADDR + MODE_ADDR);
    if (mode != REACTIVE && mode != PASSIVE_MOTION && mode != PASSIVE_DIFF) {
        mode = REACTIVE;
        DATAEE_WriteByte(EEPROM_HEAD_ADDR + MODE_ADDR, mode);
    }
    thres = DATAEE_ReadByte(EEPROM_HEAD_ADDR + THRES_ADDR);
    if (thres > 6) {
        thres = 0;
        DATAEE_WriteByte(EEPROM_HEAD_ADDR + THRES_ADDR, thres);
    }
    calibrate_threshold(thres);

    // Start supplying power to TWELITE-DIP and AMG8833 and initialize them 
    FET1_GATE = HIGH;
    FET2_GATE = HIGH;
    __delay_ms(AMG8833_STARTUP_TIME);  // AMG8833 takes time to start up
    set_moving_average(&A, true);

    while (1) {

        // Periodic task
        switch (mode) {
            case REACTIVE:
                power_mgmt(CHECK);
                break;
            
            case PASSIVE_MOTION:
                if (TMR0_HasOverflowOccured()) { // every 125msec
                    TMR0IF = 0;
                    if (update_line(&A)) {
                        if (POWER_MGMT_FLAG) {
                            FET1_GATE = HIGH;
                            __delay_ms(TWELITE_STARTUP_TIME);
                        }
                        if (state == JUST_STARTED) {
                            state = NORMAL;
                        } else {
                            twelite_uart_tx((uint8_t *) A.line, seq++, 8);
                        }
                        __delay_ms(TWELITE_TRANSMISSION_TIME);
                    }
                }
                if (POWER_MGMT_FLAG) {
                    FET1_GATE = LOW;
                }
                break;
            
            case PASSIVE_DIFF:
                if (TMR0_HasOverflowOccured()) { // every 125msec
                    TMR0IF = 0;
                    if (++notify_timer >= t4) {
                        if (POWER_MGMT_FLAG) {
                            FET2_GATE = HIGH;
                            __delay_ms(AMG8833_STARTUP_TIME);  // AMG8833 is slow to start up
                        }
                        if (update_diff(&A, true)) {
                            if (POWER_MGMT_FLAG) {
                                FET1_GATE = HIGH;
                                __delay_ms(TWELITE_STARTUP_TIME);
                            }
                            if (state == JUST_STARTED) {
                                state = NORMAL;
                            } else {
                                twelite_uart_tx((uint8_t *) A.diff, seq++, AMG8833_PIXELS_LENGTH_HALF);
                            }
                            __delay_ms(TWELITE_TRANSMISSION_TIME);
                        }
                        if (POWER_MGMT_FLAG) {
                            FET2_GATE = LOW;
                            FET1_GATE = LOW;
                        }                    
                        notify_timer = 0;
                    }
                }            
                break;
            default:
                break;
        }

        // Command from master node
        if (EUSART_DataReady) {
            c = EUSART_Read();
            if (twelite_uart_rx(c, &cmd, &seq)) {
                switch (cmd) {
                    /*** Reactive mode */
                    case 't': // Thermistor
                        update_thermistor(&A);
                        twelite_uart_tx(A.thermistor, seq, AMG8833_THERMISTOR_LENGTH);
                        break;
                    case 'p': // 64 pixels
                        update_pixels(&A);
                        twelite_uart_tx(A.pixels, seq, AMG8833_PIXELS_LENGTH_HALF);
                        break;
                    case 'd': // 64 pixels diff
                        update_diff(&A, false);
                        twelite_uart_tx((uint8_t *) A.diff, seq, AMG8833_PIXELS_LENGTH_HALF);
                        break;
                    case 'm': // Column-wise motion detection
                        update_diff_motion(&A);
                        twelite_uart_tx((uint8_t *) A.diff, seq, AMG8833_PIXELS_LENGTH_HALF);
                        break;
                    case 'M': // Motion count on a specific row
                        update_line(&A);
                        twelite_uart_tx((uint8_t *) A.line, seq, sizeof (prow));
                        break;
                    /*** Passive mode ***/
                    case 'n': // Notify motion count (passive mode)
                        mode = PASSIVE_MOTION;
                        DATAEE_WriteByte(EEPROM_HEAD_ADDR + MODE_ADDR, mode);
                        if (POWER_MGMT_FLAG) FET1_GATE = LOW;
                        break;
                    case 'o': // Notify object diff (passive mode)
                        mode = PASSIVE_DIFF;
                        DATAEE_WriteByte(EEPROM_HEAD_ADDR + MODE_ADDR, mode);
                        if (POWER_MGMT_FLAG) {
                            FET1_GATE = LOW;
                            FET2_GATE = LOW;
                        }
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