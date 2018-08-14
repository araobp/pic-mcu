/* 
 * File:   mpu9250.h
 *
 * Created on July 26, 2018, 8:16 PM
 */

#ifndef MPU9250_H
#define	MPU9250_H

#ifdef	__cplusplus
extern "C" {
#endif

#define WHO_AM_I_RESPONSE 0x71
    
uint8_t mpu9250_who_am_i(void);

uint8_t mpu9250_gyro_lpf(void);
uint8_t mpu9250_accel_lpf(void);

uint8_t mpu9250_gyro_read(uint8_t *pbuf, uint8_t len);
uint8_t mpu9250_accel_read(uint8_t *pbuf, uint8_t len);

#ifdef	__cplusplus
}
#endif

#endif	/* MPU9250_H */

