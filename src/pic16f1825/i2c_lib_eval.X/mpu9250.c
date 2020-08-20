#include "mpu9250.h"
#include "mcc_generated_files/examples/i2c_master_example.h"
#include "mcc_generated_files/eusart.h"

#define putch(v) EUSART_Write(v)

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

void mpu9250_output_cfg_to_uart() {
    uint8_t who_am_i = mpu9250_who_am_i;
    uint8_t user_ctrl = mpu9250_user_ctrl_read;
    uint8_t int_pin_cfg = mpu9250_int_pin_cfg_read;
    uint8_t wia = ak8963_wia;
    uint8_t cfg = mpu9250_cfg_read;
    uint8_t gyro_cfg = mpu9250_gyro_cfg_read;
    uint8_t accel_cfg = mpu9250_accel_cfg_read;
    uint8_t accel_cfg2 = mpu9250_accel_cfg2_read;
    printf("WHO_AM_I: 0x%02x\r\n", who_am_i);
    printf("USER_CTRL: 0x%02x\r\n", user_ctrl);
    printf("INT_PIN_CFG: 0x%02x\r\n", int_pin_cfg);
    printf("CONFIG: 0x%02x\r\n", cfg);
    printf("GYRO_CONFIG: 0x%02x\r\n", gyro_cfg);    
    printf("ACCEL_CONFIG: 0x%02x\r\n", accel_cfg);
    printf("ACCEL_CONFIG2: 0x%02x\r\n", accel_cfg2);
    printf("(AK8963)WIA: 0x%02x\r\n", wia);
    
}

// (P13) Bandwidth set to 41Hz
void mpu9250_gyro_lpf(void) {
    uint8_t value = I2C_Read1ByteRegister(MPU9250_I2C_ADDR, GYRO_CONFIG);
    value &= 0b11111100;  // Clear the field
    // FCHOICE_B set to 0U (i.e., FCHOICE set to 3U)
    I2C_Write1ByteRegister(MPU9250_I2C_ADDR, GYRO_CONFIG, value);
    
    value = I2C_Read1ByteRegister(MPU9250_I2C_ADDR, CONFIG);
    value &= 0b11111000;  // Clear the field
    value |= 0b00000011;  // DLPF_CONFG set to 3U
    I2C_Write1ByteRegister(MPU9250_I2C_ADDR, CONFIG, value);
}

// (P15) Bandwidth set to 41Hz
void mpu9250_accel_lpf(void) {
    uint8_t value = I2C_Read1ByteRegister(MPU9250_I2C_ADDR, ACCEL_CONFIG2);
    value &= 0b11110000;  // Clear the field
    value |= 0b00000011;  // ACCEL_FCHOICE_B set to 0U, and A_DLPF_CFG set to 3U;
    I2C_Write1ByteRegister(MPU9250_I2C_ADDR, ACCEL_CONFIG2, value);
}

void mpu9250_gyro_read(sensor_data *pdata) {
    I2C_ReadDataBlock(MPU9250_I2C_ADDR, GYRO_XOUT_H, pdata->gyro_data, 6);
}

void mpu9250_accel_read(sensor_data *pdata) {
    I2C_ReadDataBlock(MPU9250_I2C_ADDR, ACCEL_XOUT_H, pdata->accel_data, 6);
}

// (P14) ACCEL_FS_SEL
void mpu9250_accel_set_range(accel_range range) {
    uint8_t value = I2C_Read1ByteRegister(MPU9250_I2C_ADDR, ACCEL_CONFIG);
    value &= 0b11100111;  // Clear the field
    value |= (range << 3);
    I2C_Write1ByteRegister(MPU9250_I2C_ADDR, ACCEL_CONFIG, value);    
}

// (P14) GYRO_FS_SEL
void mpu9250_gyro_set_range(gyro_range range) {
    uint8_t value = I2C_Read1ByteRegister(MPU9250_I2C_ADDR, GYRO_CONFIG);
    value &= 0b11100111;  // Clear the field
    value |= (range << 3);
    I2C_Write1ByteRegister(MPU9250_I2C_ADDR, GYRO_CONFIG, value);        
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
        putch(TYPE_MPU9250);
        putch(cnt_mpu9250 >> 8);
        putch(cnt_mpu9250 & 0x00FF);
        cnt_mpu9250++;
        for (int i = 0; i < 6; i++) {
            putch(pdata->accel_data[i]);
        }
        for (int i = 0; i < 6; i++) {
            putch(pdata->gyro_data[i]);
        }
        putch(TYPE_AK8963);
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
