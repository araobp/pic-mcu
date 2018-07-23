#include "mcc_generated_files/mcc.h"
#include "i2c2_util.h"

#define delay __delay_us(100)

// I2C write
uint8_t i2c2_write(uint16_t dev_addr, uint8_t *pbuf, uint8_t len) {
    I2C2_MESSAGE_STATUS status;
    uint8_t write_status = 1;
    I2C2_MasterWrite(pbuf, len, dev_addr, &status);
    while (status == I2C2_MESSAGE_PENDING) {delay;};
    if (status == I2C2_MESSAGE_COMPLETE) write_status = 0;
    return write_status;
}

// I2C read
uint8_t i2c2_read(uint16_t dev_addr, uint8_t *pbuf, uint8_t len) {
    I2C2_MESSAGE_STATUS status;
    uint8_t read_status = 1;
    I2C2_MasterRead(pbuf, len, dev_addr, &status); 
    while (status == I2C2_MESSAGE_PENDING) {delay;};
    if (status == I2C2_MESSAGE_COMPLETE) read_status = 0;
    return read_status;
}
