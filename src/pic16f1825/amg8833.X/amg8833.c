
#include <stdbool.h>
#include <stdio.h>
#include "i2c_util.h"
#include "amg8833.h"
#include "twelite.h"

//#define DEBUG

uint8_t buf[AMG8833_PIXEL_DATA_LENGTH];

/**
 * @brief Enable/disable moving average
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
void uart_transmit(char *pbuf, int len) {
    for (int i=0; i<len; i++) {
        printf("%d,", pbuf[i]);
    }
    printf("%d\n", 0xff);
}

/**
 * @brief Read the thermistor register on AMG8833
 */
void read_thermistor_temp(void) {
    uint8_t err;
    float temp;
    err = i2c_read(AMG8833_DEV_ADDR, AMG8833_TTHL_ADDR, buf, AMG8833_THERMISTOR_DATA_LENGTH);
#ifdef DEBUG
    buf[0] = (int)((float)(buf[1] * 256 + buf[0]) * AMG8833_THERMISTOR_RESOLUTION);
    uart_transmit((char *)buf, AMG8833_THERMISTOR_DATA_LENGTH);
#else
    twelite_uart_tx(buf, AMG8833_THERMISTOR_DATA_LENGTH);
#endif
}

/**
 * @brief Read the pixel registers on AMG8833
 */
void read_64pixels_temp(void) {
    uint8_t err;
    float temp;
    err = i2c_read(AMG8833_DEV_ADDR, AMG8833_T01L_ADDR, buf, AMG8833_PIXEL_DATA_LENGTH);
    for (int i=0; i<AMG8833_PIXEL_DATA_LENGTH/2; i++) {
        buf[i] = buf[i*2];  // Ignore MSB of a pair of [LSB, MSB]
#ifdef DEBUG
        buf[i] = (int)((float)buf[i] * AMG8833_PIXEL_RESOLUTION);
    }
    uart_transmit((char *)buf, AMG8833_PIXEL_DATA_LENGTH/2);
#else
    }
    twelite_uart_tx(buf, AMG8833_PIXEL_DATA_LENGTH/2);    
#endif
}
