#include <stdint.h>
#include "eeprom.h"
#include "i2c2_util.h"

#define AT24C256B_I2C_ADDR 0b1010000U

void eeprom_byte_write(uint16_t data_address, uint8_t *pbuf, uint8_t len) {
    uint8_t buf[3];
    for (int c = 0; c < len; c++) {
        buf[0] = data_address >> 8;
        buf[1] = (uint8_t)(data_address);
        buf[2] = *pbuf++;
        i2c2_write(AT24C256B_I2C_ADDR, buf, 3);
        data_address++;
    }
}

void eeprom_sequential_read(uint16_t data_address, uint8_t *pbuf, uint8_t len) {
    uint8_t buf[3];
    buf[0] = data_address >> 8;
    buf[1] = (uint8_t)data_address;
    i2c2_write(AT24C256B_I2C_ADDR, buf, 2);
    i2c2_read(AT24C256B_I2C_ADDR, pbuf, len);
}
