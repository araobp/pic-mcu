/* 
 * File:   i2c_util.h
 *
 * Created on 2018/07/24, 6:53
 */

#ifndef I2C_UTIL_H
#define	I2C_UTIL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
    
uint8_t i2c1_write(uint16_t dev_addr, uint8_t *pbuf, uint8_t len);
uint8_t i2c1_read(uint16_t dev_addr, uint8_t *pbuf, uint8_t len);

uint8_t i2c2_write(uint16_t dev_addr, uint8_t *pbuf, uint8_t len);
uint8_t i2c2_read(uint16_t dev_addr, uint8_t reg_addr, uint8_t *pbuf, uint8_t len);
uint8_t i2c2_write_no_data(uint16_t dev_addr, uint8_t reg_addr);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_UTIL_H */

