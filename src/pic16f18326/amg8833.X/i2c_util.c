#include "mcc_generated_files/mcc.h"
#include "i2c_util.h"

uint8_t i2c_write(uint16_t dev_addr, uint8_t *pbuf, uint8_t len) {
    I2C1_MESSAGE_STATUS status1;
    uint8_t write_status = 1;
    I2C1_MasterWrite(pbuf, len, dev_addr, &status1);
    while (status1 == I2C1_MESSAGE_PENDING);
    if (status1 == I2C1_MESSAGE_COMPLETE) write_status = 0;
    return write_status;
}

uint8_t i2c_read(uint16_t dev_addr, uint8_t reg_addr, uint8_t *pbuf, uint8_t len) {
    uint8_t reg[1];
    I2C1_MESSAGE_STATUS status1;
    uint8_t read_status;
    reg[0] = reg_addr;
    I2C1_MasterWrite(reg, 1, dev_addr, &status1);
    while (status1 == I2C1_MESSAGE_PENDING);
    if (status1 == I2C1_MESSAGE_COMPLETE) {
        I2C1_MasterRead(pbuf, len, dev_addr, &status1); 
        while (status1 == I2C1_MESSAGE_PENDING);
        if (status1 == I2C1_MESSAGE_COMPLETE) {
            read_status = 0;
        } else {
            read_status = 2;
        }
    } else {
        read_status = 1;
    }
    return read_status;
}
