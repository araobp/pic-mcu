#include "mpu9250.h"
#include "mcc_generated_files/examples/i2c_master_example.h"

uint8_t mpu9250_who_am_i() {
    return I2C_Read1ByteRegister(MPU9250_I2C_ADDR, WHO_AM_I);
}

void mpu9250_gyro_lpf(void) {
    I2C_Write1ByteRegister(MPU9250_I2C_ADDR, CONFIG, FCHOICE_B);
    I2C_Write1ByteRegister(MPU9250_I2C_ADDR, GYRO_CONFIG, DLPF_CFG);    
}

void mpu9250_accel_lpf(void) {
    I2C_Write1ByteRegister(MPU9250_I2C_ADDR, ACCEL_CONFIG2, ACCEL_FCHOICE_B + A_DLPF_CFG);    
}

void mpu9250_gyro_read(sensor_data *pdata) {
    I2C_ReadDataBlock(MPU9250_I2C_ADDR, GYRO_XOUT_H, pdata->gyro_data, 6);
}

void mpu9250_accel_read(sensor_data *pdata) {
    I2C_ReadDataBlock(MPU9250_I2C_ADDR, ACCEL_XOUT_H, pdata->accel_data, 6);
}

void mpu9250_output_to_uart(sensor_data *pdata) {
    static uint32_t cnt = 0U;
    int16_t gx = (int16_t)((pdata->gyro_data[0] << 8) | pdata->gyro_data[1]);
    int16_t gy = (int16_t)((pdata->gyro_data[2] << 8) | pdata->gyro_data[3]);
    int16_t gz = (int16_t)((pdata->gyro_data[4] << 8) | pdata->gyro_data[5]);
    int16_t ax = (int16_t)((pdata->accel_data[0] << 8) | pdata->accel_data[1]);
    int16_t ay = (int16_t)((pdata->accel_data[2] << 8) | pdata->accel_data[3]);
    int16_t az = (int16_t)((pdata->accel_data[4] << 8) | pdata->accel_data[5]);
    printf("%ld,%d,%d,%d,%d,%d,%d\n", cnt++, gx, gy, gz, ax, ay, az);            
}