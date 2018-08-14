#include "i2c_util.h"

#define SPEED_SENSOR_I2C_ADDR 0x08

uint8_t fetch_speed_pulses(void) {
    uint8_t buf[1] = { 0x00 };
    uint8_t status = i2c1_read(SPEED_SENSOR_I2C_ADDR, buf, 1);
    if (status == 0) {
        return buf[0];
    } else {
        return 0;
    }
}

