#include "mpu9250.h"
#include "mcc_generated_files/examples/i2c_master_example.h"
#include "mcc_generated_files/eusart.h"

#define putch(v) EUSART_Write(v)

uint8_t mpu9250_who_am_i() {
    return I2C_Read1ByteRegister(MPU9250_I2C_ADDR, WHO_AM_I);
}

uint8_t ak8963_wia(void) {
    return I2C_Read1ByteRegister(AK8963_I2C_ADDR, WIA);
}

uint8_t mpu9250_user_ctrl_read(void) {
    return I2C_Read1ByteRegister(MPU9250_I2C_ADDR, USER_CTRL);
}

uint8_t mpu9250_int_pin_cfg_read(void) {
    return I2C_Read1ByteRegister(MPU9250_I2C_ADDR, INT_PIN_CFG);
}

void mpu9250_i2c_master_disable(void) {
    uint8_t user_ctrl = I2C_Read1ByteRegister(MPU9250_I2C_ADDR, USER_CTRL);
    user_ctrl &= 0b11011111; // Clear bit 5 "I2C_MST_EN"
    I2C_Write1ByteRegister(MPU9250_I2C_ADDR, USER_CTRL, user_ctrl);
}

void mpu9250_pass_through_enable(void) {
    uint8_t int_pin_cfg = I2C_Read1ByteRegister(MPU9250_I2C_ADDR, INT_PIN_CFG);
    int_pin_cfg |= 0b00000010; // Set bit 1 "INT_PIN_CFG"
    I2C_Write1ByteRegister(MPU9250_I2C_ADDR, INT_PIN_CFG, int_pin_cfg);
}

void mpu9250_output_cfg_to_uart(void) {
    uint8_t who_am_i = mpu9250_who_am_i();
    uint8_t user_ctrl = mpu9250_user_ctrl_read();
    uint8_t int_pin_cfg = mpu9250_int_pin_cfg_read();
    uint8_t wia = ak8963_wia();
    printf("WHO_AM_I: %02x\r\n", who_am_i);
    printf("USER_CTRL: %02x\r\n", user_ctrl);
    printf("INT_PIN_CFG: %02x\r\n", int_pin_cfg);
    printf("(AK8963)WIA: %02x\r\n", wia);
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

void ak8963_continous_measurement_mode_1_enable(void) {
    I2C_Write1ByteRegister(AK8963_I2C_ADDR, CNTL1, 0b00010010); // 16bit output
}

bool ak8963_magneto_read(sensor_data *pdata) {
    I2C_ReadDataBlock(AK8963_I2C_ADDR, HXL, pdata->magneto_data, 6);
    bool overflow = false;
    if ((I2C_Read1ByteRegister(AK8963_I2C_ADDR, ST2) & 0b00001000) > 0) overflow = true;
    return overflow;
}

void mpu9250_output_to_uart(sensor_data *pdata, bool ascii) {
    static uint16_t cnt = 0U;
    static uint16_t cnt_mpu9250 = 0U;
    static uint16_t cnt_ak8963 = 0U;

    if (ascii) {
        int16_t gx = (int16_t) ((pdata->gyro_data[0] << 8) | pdata->gyro_data[1]);
        int16_t gy = (int16_t) ((pdata->gyro_data[2] << 8) | pdata->gyro_data[3]);
        int16_t gz = (int16_t) ((pdata->gyro_data[4] << 8) | pdata->gyro_data[5]);
        int16_t ax = (int16_t) ((pdata->accel_data[0] << 8) | pdata->accel_data[1]);
        int16_t ay = (int16_t) ((pdata->accel_data[2] << 8) | pdata->accel_data[3]);
        int16_t az = (int16_t) ((pdata->accel_data[4] << 8) | pdata->accel_data[5]);
        int16_t mx = (int16_t) ((pdata->magneto_data[1] << 8) | pdata->magneto_data[0]);
        int16_t my = (int16_t) ((pdata->magneto_data[3] << 8) | pdata->magneto_data[2]);
        int16_t mz = (int16_t) ((pdata->magneto_data[5] << 8) | pdata->magneto_data[4]);
        printf("%ld,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n", cnt++, gx, gy, gz, ax, ay, az, mx, my, mz);
    } else {
        putch(HEADER_A0);
        putch(cnt_mpu9250 >> 8);
        putch(cnt_mpu9250 & 0x00FF);
        cnt_mpu9250++;
        for (int i = 0; i < 6; i++) {
            putch(pdata->accel_data[i]);
        }
        for (int i = 0; i < 6; i++) {
            putch(pdata->gyro_data[i]);
        }
        putch(HEADER_A1);
        putch(cnt_ak8963 >> 8);
        putch(cnt_ak8963 & 0x00FF);
        cnt_ak8963++;
        putch(pdata->magneto_data[1]);
        putch(pdata->magneto_data[0]);
        putch(pdata->magneto_data[3]);
        putch(pdata->magneto_data[2]);
        putch(pdata->magneto_data[5]);
        putch(pdata->magneto_data[4]);
    }
}
