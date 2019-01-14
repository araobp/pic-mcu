#include <stdbool.h>
#include <stdio.h>
#include "i2c_util.h"
#include "amg8833.h"
#include "twelite.h"

//#define DEBUG

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
    for (int i=0; i<AMG8833_PIXELS_LENGTH/2; i++) {
        pbuf[i] = pbuf[i*2];  // Ignore MSB of a pair of [LSB, MSB]
#ifdef DEBUG
        buf[i] = (int)((float)buf[i] * AMG8833_PIXELS_RESOLUTION);
    }
    uart_transmit((char *)buf, AMG8833_PIXELS_LENGTH/2);
#else
    }
#endif
}
