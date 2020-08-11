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

#define MPU9250_I2C_ADDR 0b1101000U
#define AK8963_I2C_ADDR 0x0C

#define WHO_AM_I 0x75  // Its value: MPU9250 0x71, MPU9255 0x73
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
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

/* Low-Pass Filter setting
 * datasheet: https://www.invensense.com/wp-content/uploads/2015/02/RM-MPU-9250A-00-v1.6.pdf
 */
// (P13) Gyroscope: bandwidth 41Hz, delay 5.9 msec
#define FCHOICE_B 0U
#define DLPF_CFG 3U
// (P15) Accelerometer: bandwidth 44.8Hz, delay 4.88 msec
#define ACCEL_FCHOICE_B 0U
#define A_DLPF_CFG 3U

typedef struct {
    uint8_t gyro_data[6];
    uint8_t accel_data[6];
    uint8_t magneto_data[6];
} sensor_data;
    
uint8_t mpu9250_who_am_i(void);

uint8_t ak8963_wia(void);

uint8_t mpu9250_user_ctrl_read(void);

uint8_t mpu9250_int_pin_cfg_read(void);

void mpu9250_i2c_master_disable(void);

void mpu9250_pass_through_enable(void);

void mpu9250_output_cfg_to_uart(void);

void mpu9250_gyro_lpf(void);

void mpu9250_accel_lpf(void);

void mpu9250_gyro_read(sensor_data *pdata);

void mpu9250_accel_read(sensor_data *pdata);

void ak8963_continous_measurement_mode_1_enable(void);

bool ak8963_magneto_read(sensor_data *pdata);

void mpu9250_output_to_uart(sensor_data *pdata);

#ifdef	__cplusplus
}
#endif

#endif	/* MPU9250_H */

