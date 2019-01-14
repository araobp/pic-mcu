/* 
 * File:   twelite.h
 *
 * Created on 2019/01/14, 4:27
 */

#ifndef TWELITE_H
#define	TWELITE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

// TWELITE packet
#define DST_NODE 0x00
#define BYTE 0xA0
#define ACK_ENABLED 0x01
#define RESPONSE_MSG_DISABLED 0x07
#define TERMINATOR 0xFF
#define EOT 0x04

// Data position in a packet
#define PAYLOAD_POS 18U
#define EOT_POS 20U
    
void twelite_uart_tx(uint8_t *pbuf, uint8_t len);
bool twelite_uart_rx(uint8_t c);

#ifdef	__cplusplus
}
#endif

#endif	/* TWELITE_H */

