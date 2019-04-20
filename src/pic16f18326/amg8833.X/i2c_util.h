/* 
 * File:   i2c_util.h
 *
 * Created on January 11, 2019, 8:46 PM
 */

#ifndef NEWFILE_H
#define	NEWFILE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>

uint8_t i2c_write(uint16_t dev_addr, uint8_t *pbuf, uint8_t len);
uint8_t i2c_read(uint16_t dev_addr, uint8_t reg_addr, uint8_t *pbuf, uint8_t len);

#ifdef	__cplusplus
}
#endif

#endif	/* NEWFILE_H */

