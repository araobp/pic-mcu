#include "mcc_generated_files/mcc.h"
#include "amg8833.h"
#include "twelite.h"
#include <stdbool.h>

uint8_t buf[AMG8833_PIXELS_LENGTH];
uint8_t buf_prev[AMG8833_PIXELS_LENGTH/2];
int8_t diff[AMG8833_PIXELS_LENGTH/2];
int8_t sum[8];

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
                    default:
                        break;
                }
            }
        }
    }
}