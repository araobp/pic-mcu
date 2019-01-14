#include "mcc_generated_files/mcc.h"
#include "amg8833.h"
#include "twelite.h"
#include <stdbool.h>

uint8_t buf[AMG8833_PIXELS_LENGTH];

void main(void) {

    uint8_t c, cmd;
    uint8_t seq;

    SYSTEM_Initialize();
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    set_moving_average(true);
    
    while (1) {
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
                    default:
                        break;
                }
            }
        }
    }
}
