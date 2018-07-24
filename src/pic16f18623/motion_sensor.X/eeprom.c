#include "mcc_generated_files/mcc.h"
#include <stdint.h>
#include <string.h>
#include "eeprom.h"
#include "i2c_util.h"

/*
 * Atmel AT24C256B EEPROM
 * Reference: http://ww1.microchip.com/downloads/en/devicedoc/doc5279.pdf
 */

#define AT24C256B_I2C_ADDR 0b1010000U
#define delay __delay_ms(6)  // t_wr 5 msec for writing data onto EEPROM

uint8_t eeprom_page_write(uint16_t data_address, uint8_t *pbuf, uint8_t len) {
    uint8_t buf[66];  // Max 64 bytes of data
    uint8_t status;
    buf[0] = data_address >> 8;
    buf[1] = (uint8_t)(data_address);
    memcpy(&buf[2], pbuf, len);
    status = i2c1_write(AT24C256B_I2C_ADDR, buf, len+2);
    delay;
    return status;
}

uint8_t eeprom_byte_write(uint16_t data_address, uint8_t *pbuf, uint8_t len) {
    uint8_t buf[3];
    uint8_t status;
    for (int c = 0; c < len; c++) {
        buf[0] = data_address >> 8;
        buf[1] = (uint8_t)(data_address);
        buf[2] = *pbuf++;
        status = i2c1_write(AT24C256B_I2C_ADDR, buf, 3);
        data_address++;
    }
    delay;
    return status;
}

uint8_t eeprom_sequential_read(uint16_t data_address, uint8_t *pbuf, uint8_t len) {
    uint8_t buf[3];
    uint8_t status;
    buf[0] = data_address >> 8;
    buf[1] = (uint8_t)data_address;
    status = i2c1_write(AT24C256B_I2C_ADDR, buf, 2);
    status = i2c1_read(AT24C256B_I2C_ADDR, pbuf, len);
    return status;
}
