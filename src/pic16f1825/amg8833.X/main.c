#include "mcc_generated_files/mcc.h"
#include "amg8833.h"
#include <stdbool.h>

#define EOT 0x04

void main(void) {

    char c;
    uint8_t state = 0;

    SYSTEM_Initialize();
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    set_moving_average(true);

    while (1) {
        if (EUSART_DataReady) {
            c = EUSART_Read();
            if (state == 9) {

                switch (c) {
                    case 't': // Thermistor
                        read_thermistor_temp();
                        break;
                    case 'p': // 64 pixels
                        read_64pixels_temp();
                        break;
                    default:
                        break;
                }
            } else if (state > 10 && c == EOT) {
                c = 0;
            }
            state++;
        }
    }
}
