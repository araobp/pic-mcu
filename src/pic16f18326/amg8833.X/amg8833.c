#include <stdbool.h>
#include <stdio.h>
#include "i2c_util.h"
#include "amg8833.h"
#include "twelite.h"

//#define DEBUG

/* Macro for 2D matrix elemenet positions
 *     -- i -->
 *    +-----------
 *  | |
 *  j | 2D matrix
 *  | |
 *  V |
 */
#define IDX(i, j) (j * AMG8833_PIXELS_LENGTH_HALF + i)
#define SCAN_ROW_IDX(i) (SCAN_ROW * AMG8833_PIXELS_LENGTH_HALF + i)

/**
 * Enable/disable moving average
 * @param enable
 */
void set_moving_average(bool enable) {
    uint8_t reg_addr_sequence[5] = {AMG8833_1F_ADDR, AMG8833_1F_ADDR, AMG8833_1F_ADDR,
        AMG8833_AVE_ADDR, AMG8833_1F_ADDR};

    uint8_t enable_sequence[5][2] = {
        {AMG8833_1F_ADDR, 0x50},
        {AMG8833_1F_ADDR, 0x45},
        {AMG8833_1F_ADDR, 0x57},
        {AMG8833_AVE_ADDR, 0x20},
        {AMG8833_1F_ADDR, 0x00}};

    uint8_t disable_sequence[5][2] = {
        {AMG8833_1F_ADDR, 0x50},
        {AMG8833_1F_ADDR, 0x45},
        {AMG8833_1F_ADDR, 0x57},
        {AMG8833_AVE_ADDR, 0x00},
        {AMG8833_1F_ADDR, 0x00}};

    if (enable) {
        for (int i = 0; i < 5; i++) {
            i2c_write(AMG8833_DEV_ADDR, enable_sequence[i], 2);
        }
    } else {
        for (int i = 0; i < 5; i++) {
            i2c_write(AMG8833_DEV_ADDR, disable_sequence[i], 2);
        }        
    }
}

// Transmit data to UART TX
void uart_transmit(uint8_t *pbuf, uint8_t len) {
    for (int i=0; i<len; i++) {
        printf("%d,", pbuf[i]);
    }
    printf("%d\n", 0xff);
}

/**
 * @brief Read the thermistor register on AMG8833
 */
void read_thermistor(uint8_t *pbuf) {
    uint8_t err;
    float temp;
    err = i2c_read(AMG8833_DEV_ADDR, AMG8833_TTHL_ADDR, pbuf, AMG8833_THERMISTOR_LENGTH);
#ifdef DEBUG
    buf[0] = (int)((float)(buf[1] * 256 + buf[0]) * AMG8833_THERMISTOR_RESOLUTION);
    uart_transmit((char *)buf, AMG8833_THERMISTOR_LENGTH);
#else
#endif
}

/**
 * @brief Read the pixel registers on AMG8833
 */
void read_pixels(uint8_t *pbuf) {
    uint8_t err;
    float temp;
    err = i2c_read(AMG8833_DEV_ADDR, AMG8833_T01L_ADDR, pbuf, AMG8833_PIXELS_LENGTH);
    for (int i=0; i<AMG8833_PIXELS_LENGTH_HALF; i++) {
        pbuf[i] = pbuf[i*2];  // Ignore MSB of a pair of [LSB, MSB]
#ifdef DEBUG
        buf[i] = (int)((float)buf[i] * AMG8833_PIXELS_RESOLUTION);
    }
    uart_transmit((char *)buf, AMG8833_PIXELS_LENGTH/2);
#else
    }
#endif
}

void read_pixels_diff(uint8_t *pbuf, uint8_t *pbuf_prev, int8_t *pdiff) {
    uint8_t err;
    float temp;
    err = i2c_read(AMG8833_DEV_ADDR, AMG8833_T01L_ADDR, pbuf, AMG8833_PIXELS_LENGTH);
    for (int i=0; i<AMG8833_PIXELS_LENGTH_HALF; i++) {
        pbuf[i] = pbuf[i*2];  // Ignore MSB of a pair of [LSB, MSB]
        pdiff[i] = (int8_t)pbuf[i] - (int8_t)pbuf_prev[i]; 
        pbuf_prev[i] = pbuf[i];
    }
}

/**
 * Colmun-wise motion detection filter
 * @param j
 * @param pdiff
 * @param pcolumn
 * @param downward
 */
void filter(int i, int8_t *pdiff, int8_t *pcolumn, bool downward) {
    bool filter_on, filter_detecting;
    int idx, start, end;
    int j;
    filter_on = false;
    filter_detecting = false;
    j = (downward) ? 0: 7;
    while (true) {
        idx = IDX(i, j);
        // Column-wise scan downward
        if (!filter_on && pdiff[idx] > PEAK_COUNT_THRESHOLD) {
            filter_on = true;
        } else if (filter_on && pdiff[idx] < -PEAK_COUNT_THRESHOLD) {
            filter_detecting = true;
            pcolumn[j] = 1;
        } else if (filter_on && filter_detecting && pdiff[idx] >= -PEAK_COUNT_THRESHOLD) {
            filter_on = false;
            filter_detecting = false;
        }
        if (downward) {
            if (j++ >= 7) break;
        } else {
            if (j-- <= 0) break;
        }
    }
}

/**
 * Column-wise motion detection
 * @param pbuf
 * @param pbuf_prev
 * @param pdiff
 */
void read_pixels_motion(uint8_t *pbuf, uint8_t *pbuf_prev, int8_t *pdiff) {
    int8_t column[8] = { 0 }; 
    bool filter_on, filter_flip, filter_detecting;
    int idx;
    
    read_pixels_diff(pbuf, pbuf_prev, pdiff);
    for (int i=0; i<8; i++) {
        filter(i, pdiff, column, true);  // column-wise scan downward
        filter(i, pdiff, column, false); // column-wise scan upward
        for (int j=0; j<8; j++) {  // Copy motion data
            pdiff[IDX(i,j)] = column[i];
            column[i] = 0;
        }
    }
}

/**
 * Motion count on a specific row after column-wise motion detection
 * @param pbuf
 * @param pbuf_prev
 * @param pdiff
 * @param row
 */
void read_motion(uint8_t *pbuf, uint8_t *pbuf_prev, int8_t *pdiff, int8_t *row) {
    static int8_t prev_row[8] = { 0 };
    int idx;
    int peak_on, peak;

    read_pixels_motion(pbuf, pbuf_prev, pdiff);

    // Find peaks
    for (int i=0; i<8; i++) {
        idx = SCAN_ROW_IDX(i);
        peak_on = 0;
        if (peak_on == 0 && pdiff[idx] != 0) {
            peak_on = i;
        } else if (peak_on != 0 && (pdiff[idx] == 0 || i==7)) {
            peak = (peak_on + i)/2;
            row[peak] = pdiff[idx];
            peak_on = 0;
        }
    }
    
    // Check if the peaks were already counted in the previous read.
    for (int i=0; i<8; i++) {
        switch (i) {
            case 0:
                if (prev_row[i] != 0 && prev_row[i+1] != 0) {
                    row[i] = 0;
                }
                break;
            case 7:
                if (prev_row[i-1] != 0 && prev_row[i] != 0) {
                    row[i] = 0;
                }
                break;
            default:
                if (prev_row[i-1] != 0 && prev_row[i] != 0 && prev_row[i+1] != 0) {
                    row[i] = 0;
                }
                break;
        }
    }
    
    // Save the current row
    for (int i=0; i<8; i++) {
        prev_row[i] = pdiff[SCAN_ROW_IDX(i)];
    }
}
