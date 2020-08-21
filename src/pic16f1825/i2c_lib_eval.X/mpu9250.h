/* 
 * File:   mpu9250.h
 * Author: shiny
 *
 * Created on August 11, 2020, 9:12 AM
 */

#ifndef MPU9250_H
#define	MPU9250_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

// I2C slave addresses
#define MPU9250_I2C_ADDR 0b1101000U
#define AK8963_I2C_ADDR 0x0C

// Register addresses
#define WHO_AM_I 0x75  // Its value: MPU9250 0x71, MPU9255 0x73
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define ACCEL_CONFIG2 0x1D
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H 0x43
#define USER_CTRL 0x6A
#define INT_PIN_CFG 0x37

// AK8963
#define WIA 0x00
#define HXL 0x03
#define CNTL1 0x0A
#define ST2 0x09

// Type definition of data output    
#define TYPE_MPU9250 0xA0
#define TYPE_AK8963 0xA1

// Macros to read registers
#define mpu9250_who_am_i I2C_Read1ByteRegister(MPU9250_I2C_ADDR, WHO_AM_I)
#define ak8963_wia I2C_Read1ByteRegister(AK8963_I2C_ADDR, WIA)
#define mpu9250_user_ctrl_read I2C_Read1ByteRegister(MPU9250_I2C_ADDR, USER_CTRL)
#define mpu9250_int_pin_cfg_read I2C_Read1ByteRegister(MPU9250_I2C_ADDR, INT_PIN_CFG)
#define mpu9250_cfg_read I2C_Read1ByteRegister(MPU9250_I2C_ADDR, CONFIG)
#define mpu9250_gyro_cfg_read I2C_Read1ByteRegister(MPU9250_I2C_ADDR, GYRO_CONFIG)
#define mpu9250_accel_cfg_read I2C_Read1ByteRegister(MPU9250_I2C_ADDR, ACCEL_CONFIG)
#define mpu9250_accel_cfg2_read I2C_Read1ByteRegister(MPU9250_I2C_ADDR, ACCEL_CONFIG2)

typedef struct {
    uint8_t gyro_data[6];
    uint8_t accel_data[6];
    uint8_t magneto_data[6];
} sensor_data;
    
typedef enum {
    G_2, G_4, G8, G_16
} accel_range;

typedef enum {
    DPS_250, DPS_500, DPS_1000, DPS_2000
} gyro_range;


void mpu9250_i2c_master_disable(void);

void mpu9250_pass_through_enable(void);

void mpu9250_output_cfg_to_uart(void);

void mpu9250_accel_lpf(void);

void mpu9250_gyro_lpf(void);

void mpu9250_accel_read(sensor_data *pdata);

void mpu9250_gyro_read(sensor_data *pdata);

void mpu9250_accel_set_range(accel_range range);

void mpu9250_gyro_set_range(gyro_range range);

void ak8963_continous_measurement_mode_1_enable(void);

bool ak8963_magneto_read(sensor_data *pdata);

void mpu9250_ascii_output_to_uart(sensor_data *pdata);

void mpu9250_binary_output_to_uart(sensor_data *pdata);

void ak8963_binary_output_to_uart(sensor_data *pdata);

#ifdef	__cplusplus
}
#endif

#endif	/* MPU9250_H */

