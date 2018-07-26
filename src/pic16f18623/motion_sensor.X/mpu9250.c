#include "i2c_util.h"

#define MPU9250_I2C_ADDR 0b1101000U

// I2C slave register addresses
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG2 0x1D
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H 0x43
#define WHO_AM_I 0x75

/* Low-Pass Filter setting
 * Datasheet: https://www.invensense.com/wp-content/uploads/2015/02/RM-MPU-9250A-00-v1.6.pdf
 */
// (P13) Gyro: bandwidth 41Hz, delay 5.9msec
#define FCHOICE_B 0U;
#define DLPF_CFG 3U;
// (P15) Accel: bandwidth 44.8Hz, delay 4.88msec
#define ACCEL_FCHOICE_B 0U;
#define A_DLPF_CFG 3U;

uint8_t mpu9250_who_am_i(void) {
    uint8_t data[1];
    uint8_t status = i2c2_read(MPU9250_I2C_ADDR, WHO_AM_I, data, 1);
    return data[0];
}

uint8_t mpu9250_gyro_lpf(void) {
    // FIFO_MODE: 0
    // EXT_SYNC_SET: 0
    // GYRO_FS_SEL: 0x00 (+250dps)
    uint8_t buf[2];
    uint8_t status;
    buf[0] = CONFIG;
    buf[1] = FCHOICE_B;
    status = i2c2_write(MPU9250_I2C_ADDR, buf, 2);
    if (status == 0) {
        buf[0] = GYRO_CONFIG;
        buf[1] = DLPF_CFG;
        status = i2c2_write(MPU9250_I2C_ADDR, buf, 2);
    }
    return status;
}

uint8_t mpu9250_accel_lpf(void) {
    uint8_t buf[2];
    buf[0] = ACCEL_CONFIG2;
    buf[1] = ACCEL_FCHOICE_B;
    buf[1] = A_DLPF_CFG + buf[1];
    return i2c2_write(MPU9250_I2C_ADDR, buf, 2);
}

uint8_t mpu9250_gyro_read(uint8_t *pbuf, uint8_t len) {
    return i2c2_read(MPU9250_I2C_ADDR, ACCEL_XOUT_H, pbuf, len);
}

uint8_t mpu9250_accel_read(uint8_t *pbuf, uint8_t len) {
    return i2c2_read(MPU9250_I2C_ADDR, GYRO_XOUT_H, pbuf, len);
}
