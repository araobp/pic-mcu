/* 
 * File:   amg8833.h
 * Author: shiny
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
#define AMG8833_DEV_ADDR 0x68 << 1
#define AMG8833_T01L_ADDR 0x80
#define AMG8833_PIXEL_DATA_LENGTH 128U
#define AMG8833_PIXEL_DATA_LENGTH_16bit 64U
#define AMG8833_TTHL_ADDR 0x0E
#define AMG8833_1F_ADDR 0x1F
#define AMG8833_AVE_ADDR 0x07
#define AMG8833_RESOLUTION 0.25

void set_moving_average(bool enable);

void read_thermistor_temp(float *temp);

void read_64pixels_temp(float *pbuf);

#ifdef	__cplusplus
}
#endif

#endif	/* AMG8833_H */

