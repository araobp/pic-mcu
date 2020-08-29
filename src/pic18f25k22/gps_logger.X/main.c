#include "mcc_generated_files/mcc.h"
#include "twelite_driver.h"
#include "nema_receiver.h"
#include <string.h>

#define BUFSIZE 80  // TWELITE's max payload length is 80 bytes

typedef enum {
    SLEEPING,
    RECEIVING
} state_machine;

const int ONE_SECOND = 1000 / 125;
const int NEMA_SENTENCE_RECEIVE_TIMEOUT = ONE_SECOND * 3000; // 5min
const int TWELITE_TRANSFERRING_TIMER = ONE_SECOND / 4; // 0.25 sec
uint8_t gpsts[16] = "$GPSTS,";

int timer_cnt = 0;
state_machine state;

void next_state(state_machine s) {
    state = s;
    timer_cnt = 0;
}

void update_gps_status(int len) {
    uint8_t t = timer_cnt / ONE_SECOND;
    uint8_t t100 = t / 100;
    t -= t100 * 100;
    uint8_t t10 = t / 10;
    t -= t10 * 10;
    if (len > 0) {
        gpsts[7] = 'Y';
    } else if (len < 0) {
        gpsts[7] = 'N';
    } else {
        gpsts[7] = '?';
    }
    gpsts[8] = ',';
    gpsts[9] = (char) (0x30 + t100); // Convert it to ASCII code
    gpsts[10] = (char) (0x30 + t10);
    gpsts[11] = (char) (0x30 + t);
}

void blink_led() {
    LED_SetLow();
    __delay_ms(10);
    LED_SetHigh();
}

/*
                         Main application
 */
void main(void) {
    // Initialize the device
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    uint8_t tx_seq = 0;
    uint8_t cmd, rx_seq;

    uint8_t buf[BUFSIZE];

    // NEMA sentences receiving buffer initialization
    set_receiver_buf(buf, BUFSIZE);

    // LED off
    LED_SetHigh();

    // TWELITE-DIP start
    TW_M3_SetHigh();
    FET_TW_SetHigh(); // TWELITE Start 
    __delay_ms(300);
    TW_M3_SetLow();  // TWELITE Sleep
    
    // GPS module start
    FET_GPS_SetHigh();  // GPS Start
    
    // Initial state
    next_state(RECEIVING);

    while (1) {

        // Timer count up
        if (TMR0_HasOverflowOccured()) { // every 125 msec
            timer_cnt++;
            TMR0IF = 0;
            TMR0_Reload();
            ClrWdt(); // Clear Watch Dog Timer
        }

        // State machine
        switch (state) {
            case SLEEPING: // Just after WDT timeout
                FET_GPS_SetHigh();
                next_state(RECEIVING);
                break;
            case RECEIVING:
                if (EUSART2_DataReady) {
                    char c = EUSART2_Read();
                    int len = parse_nema_gpgga(c);
                    if (len > 0 || len < 0) {
                        
                        // Transfer GPSTS and GPGGA to TWELITE via UART
                        TW_M3_SetHigh();  // TWELITE wakeup
                        __delay_ms(TWELITE_WAKEUP_TIME);                 
                        update_gps_status(len);
                        twelite_uart_tx(gpsts, tx_seq++, 12);
                        if (len > 0) { // Check if it is 3D-fix
                            FET_GPS_SetLow();
                            twelite_uart_tx(buf, tx_seq++, len);
                        }
                        TW_M3_SetLow();  // TWELITE sleep
                        blink_led();
                        
                        // If fixed, sleep.
                        if (len > 0) {
                            next_state(SLEEPING);
                            SLEEP();
                        }
                    }
                } else if (timer_cnt > NEMA_SENTENCE_RECEIVE_TIMEOUT) {
                    FET_GPS_SetLow();
                    next_state(SLEEPING);
                    SLEEP();
                }
                break;
        }
    }
}
