#include "mcc_generated_files/mcc.h"
#include "amg8833.h"
#include "twelite.h"
#include <stdbool.h>

#define LOW 0
#define HIGH 1
#define FET_GATE LATCbits.LATC2

// Timers
#define T_1 2       // 2 sec
#define T_2 3       // 3 sec
#define T_3 180     // 180 sec (3 min)
#define T_4 6000    // 6000 sec (10 min)

// Power saving state machine
typedef enum {
    WAKING_UP, CONNECTING, RUNNING, SLEEPING
} state_machine;

typedef enum {
    CHECK, RUN, KEEP_ON
} state_machine_command;

// Convert timers into 250msec time period unit
const int t1 = T_1 * 4;
const int t2 = T_2 * 4;
const int t3 = T_3 * 4;
const int t4 = T_4 * 4;

// Buffers
uint8_t buf[AMG8833_PIXELS_LENGTH];
uint8_t buf_prev[AMG8833_PIXELS_LENGTH/2];
int8_t diff[AMG8833_PIXELS_LENGTH/2];
int8_t sum[8];

/**
 * Power management
 * @param reset true to reset the timeout counter
 */
void power_mgmt(state_machine_command command) {
    static int timeout_cnt = 0;
    static state_machine state = WAKING_UP;
    uint8_t hello[1] = { 'h' };
    
    //--- Power management disabled by jumper pin -----------------------------
    if (PORTAbits.RA4 == LOW) return;  // WPU is enabled and jumper pin is off
    
    //--- command: RUN or KEEP_ON ---------------------------------------------
    if (command == RUN && state == CONNECTING) {
        timeout_cnt = 0;
        state = RUNNING;
        return;
    } else if (command == KEEP_ON && state == RUNNING) {
        timeout_cnt = 0;
        return;
    }
    
    //--- command: CHECK ------------------------------------------------------
    if (TMR0_HasOverflowOccured()) {  // every 250msec
        timeout_cnt++;
        TMR0IF = 0;
    }
    
    switch(state) {
        case WAKING_UP:
            if (timeout_cnt >= t1) {
                twelite_uart_tx(hello, 0, 1);  // Send 'h(ello)'
                timeout_cnt = 0;
                state = CONNECTING;
            }
            break;
        case CONNECTING:
            if (timeout_cnt >= t2) {
                timeout_cnt = 0;
                FET_GATE = LOW;
                state = SLEEPING;
            }
            break;
        case RUNNING:
            if (timeout_cnt >= t3) {
                timeout_cnt = 0;
                FET_GATE = LOW;
                state = SLEEPING;
            }            
            break;
        case SLEEPING:
            if (timeout_cnt >= t4) {
                timeout_cnt = 0;
                FET_GATE = HIGH;
                state = WAKING_UP;
            }            
            break;
        default:
            break;
    }
}

void main(void) {

    uint8_t c, cmd;
    uint8_t seq;

    SYSTEM_Initialize();
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    // Start supplying power to TWELITE-DIP and AMG8833 
    FET_GATE = HIGH;
    
    __delay_ms(500);
    
    //set_moving_average(true);
    
    while (1) {
        power_mgmt(CHECK);
        if (EUSART_DataReady) {
            c = EUSART_Read();
            if (twelite_uart_rx(c, &cmd, &seq)) {
                switch (cmd) {
                    case 't': // Thermistor
                        read_thermistor(buf);
                        twelite_uart_tx(buf, seq, AMG8833_THERMISTOR_LENGTH);
                        break;
                    case 'p': // 64 pixels
                        read_pixels(buf);
                        twelite_uart_tx(buf, seq, AMG8833_PIXELS_LENGTH/2);
                        break;
                    case 'd': // 64 pixels diff
                        read_pixels_diff(buf, buf_prev, diff);
                        twelite_uart_tx((uint8_t *)diff, seq, AMG8833_PIXELS_LENGTH/2);
                        break;
                    case 'D': // Averages of 64 pixels diff
                        read_pixels_diff(buf, buf_prev, diff);
                        int16_t s;
                        for (int i=0; i<8; i++) {
                            s = 0;
                            for (int j=0; j < 8; j++) {
                                s += diff[i*8+j];
                            }
                            sum[i] = (int8_t)(s/8);
                        }
                        twelite_uart_tx((uint8_t *)sum, seq, 8);
                        break;
                    case 'r':
                        power_mgmt(RUN);
                        break;
                    default:
                        break;
                }
                power_mgmt(KEEP_ON);
            }
        }
    }
}