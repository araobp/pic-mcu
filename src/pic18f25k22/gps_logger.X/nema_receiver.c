/* 
 * File:   nema_receiver.h
 * Author: https://github.com/araobp
 *
 * Created on July 26, 2020, 6:39 PM
 */

#include "nema_receiver.h"
#include <stdint.h>
#include <stdbool.h>

#define THREE_D_FIX '1'

uint8_t buflen = 0;
uint8_t *buf;
bool prefix_check = false;
bool this_is_gprmc = false;

int idx = 0;
const uint8_t GPGGA[] = "$GPGGA";
const int PREFIX_LEN = 6;

int comma_cnt = 0;
bool fixed = false;

void set_receiver_buf(uint8_t *pbuf, uint8_t len) {
    buf = pbuf;
    buflen = len;
}

int parse_nema_gpgga(uint8_t c) {
    int record_len = 0;

    if (c == '\n') {  // End of record
        if (this_is_gprmc) {  // "$GPGGA ... \n"
            if (fixed) {
                record_len = idx - 1;  // w/o "\r\n"
            } else {
                record_len = -1;
            }
        }
        prefix_check = true;
        this_is_gprmc = false;
        idx = 0;
        comma_cnt = 0;
        fixed = false;
    } else if (prefix_check) {  // Header sync
          if (GPGGA[idx] == c) {
            buf[idx++] = c;
            if (idx == PREFIX_LEN) {
                this_is_gprmc = true;
                prefix_check = false;
            }
        } else {
            prefix_check = false;
        }
    } else if (this_is_gprmc) {  // GPGGA record
        buf[idx++] = c;
        if (comma_cnt == 6 && c == THREE_D_FIX) {
            fixed = true;
        }
        if (c == ',') {
            comma_cnt++;
        }
    }

    return record_len;
}