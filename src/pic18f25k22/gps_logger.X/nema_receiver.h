/* 
 * File:   nema_receiver.h
 * Author: https://github.com/araobp
 *
 * Created on July 26, 2020, 6:39 PM
 */

#ifndef NEMA_RECEIVER_H
#define	NEMA_RECEIVER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
    
void set_receiver_buf(uint8_t *pbuf, uint8_t len);
int parse_nema_gpgga(uint8_t c);

#ifdef	__cplusplus
}
#endif

#endif	/* NEMA_RECEIVER_H */

