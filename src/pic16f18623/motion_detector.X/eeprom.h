/* 
 * File:   eeprom.h
 *
 * Created on July 23, 2018, 5:22 PM
 */

#ifndef EEPROM_H
#define	EEPROM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
    
void eeprom_byte_write(uint16_t data_address, uint8_t *pbuf, uint8_t len);
void eeprom_sequential_read(uint16_t data_address, uint8_t *pbuf, uint8_t len);

#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

