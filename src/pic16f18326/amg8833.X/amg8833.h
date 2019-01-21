/* 
 * File:   amg8833.h
 *
 * Created on January 11, 2019, 9:44 PM
 */

#ifndef AMG8833_H
#define	AMG8833_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stdint.h"

// Infrared array sensor "Panasonic AMG8833"
#define AMG8833_DEV_ADDR 0x68
#define AMG8833_T01L_ADDR 0x80
#define AMG8833_TTHL_ADDR 0x0E
#define AMG8833_1F_ADDR 0x1F
#define AMG8833_AVE_ADDR 0x07
#define AMG8833_THERMISTOR_LENGTH 2U    
#define AMG8833_PIXELS_LENGTH 128U
#define AMG8833_THERMISTOR_RESOLUTION 0.0625
#define AMG8833_PIXELS_RESOLUTION 0.25

void set_moving_average(bool enable);

void read_thermistor(uint8_t *pbuf);

void read_pixels(uint8_t *pbuf);

void read_pixels_diff(uint8_t *pbuf, uint8_t *pbuf_prev, int8_t *pdiff);

#ifdef	__cplusplus
}
#endif

#endif	/* AMG8833_H */
