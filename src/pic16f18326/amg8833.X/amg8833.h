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

#include <stdbool.h>
#include <stdint.h>
#include "i2c_util.h"

// Infrared array sensor "Panasonic AMG8833"
#define AMG8833_DEV_ADDR 0x68
#define AMG8833_T01L_ADDR 0x80
#define AMG8833_TTHL_ADDR 0x0E
#define AMG8833_1F_ADDR 0x1F
#define AMG8833_AVE_ADDR 0x07
#define AMG8833_THERMISTOR_LENGTH 2U    
#define AMG8833_PIXELS_LENGTH 128U    
#define AMG8833_PIXELS_LENGTH_HALF 64U
#define AMG8833_PIXELS_TWIN_LENGTH_HALF 128U

// Absolute value of diff larger than this value is regarded as moving.
#define PEAK_COUNT_THRESHOLD 4

// Resolution to identify an object
#define OBJECT_RESOLUTION 2
    
// Motion count on a specific row: this value is an index of the row. 
#define SCAN_ROW 4
    
// AMG8833 instance
typedef struct {
    i2c_handle i2c_h;
    // The members below are updated by update_* methods defined in this file.
    uint8_t thermistor[2];
    uint8_t pixels[AMG8833_PIXELS_LENGTH];
    int8_t diff[AMG8833_PIXELS_LENGTH_HALF];
    int8_t motion[AMG8833_PIXELS_LENGTH_HALF];
    // The member below are buffers for internal use.
    uint8_t pixels_prev[AMG8833_PIXELS_LENGTH_HALF];
} amg8833_instance;

typedef struct {
    int8_t line[16];
    int8_t motion[AMG8833_PIXELS_TWIN_LENGTH_HALF];
    // The members below are buffers for internal use.
    int8_t prev_line[8][16];    
} line_instance;

void init_amg8833_instance(amg8833_instance *A, i2c_handle i2c_h);

void init_line_instance(line_instance *L);

void set_moving_average(amg8833_instance *A, bool enable);

void update_thermistor(amg8833_instance *A);

void update_pixels(amg8833_instance *A);

bool update_diff(amg8833_instance *A, bool flag);

void update_motion(amg8833_instance *A);

bool update_line(amg8833_instance *A1, amg8833_instance *A2, line_instance *L);

void merge_pixels(amg8833_instance *A1, amg8833_instance *A2, uint8_t *buf);

void merge_diff(amg8833_instance *A1, amg8833_instance *A2, uint8_t *buf);

void merge_motion(amg8833_instance *A1, amg8833_instance *A2, int8_t *buf);

void calibrate_threshold(int v);

#ifdef	__cplusplus
}
#endif

#endif	/* AMG8833_H */

