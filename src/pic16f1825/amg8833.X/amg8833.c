
#include <stdbool.h>
#include "i2c_util.h"
#include "amg8833.h"

void set_moving_average(bool enable) {
    uint8_t reg_addr_sequence[5] = {AMG8833_1F_ADDR, AMG8833_1F_ADDR, AMG8833_1F_ADDR,
        AMG8833_AVE_ADDR, AMG8833_1F_ADDR};

    uint8_t enable_sequence[5][2] = {
        {AMG8833_1F_ADDR, 0x50},
        {AMG8833_1F_ADDR, 0x45},
        {AMG8833_1F_ADDR, 0x57},
        {AMG8833_AVE_ADDR, 0x20},
        {AMG8833_1F_ADDR, 0x00}};

    uint8_t disable_sequence[5][2] = {
        {AMG8833_1F_ADDR, 0x50},
        {AMG8833_1F_ADDR, 0x45},
        {AMG8833_1F_ADDR, 0x57},
        {AMG8833_AVE_ADDR, 0x00},
        {AMG8833_1F_ADDR, 0x00}};

    if (enable) {
        for (int i = 0; i < 5; i++) {
            i2c_write(AMG8833_DEV_ADDR, enable_sequence + i, 2);
        }
    } else {
        for (int i = 0; i < 5; i++) {
            i2c_write(AMG8833_DEV_ADDR, disable_sequence + i, 2);
        }        
    }
}

void read_thermistor_temp(float *temp) {
    
}

void read_64pixels_temp(float *pbuf) {
    
}
