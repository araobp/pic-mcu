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
#define TERMINATOR 0xFF
#define EOT 0x04

// Data position in a packet
#define PAYLOAD_POS 14U
#define EOT_POS 16U
    
void twelite_uart_tx(char *pbuf, uint8_t len);
bool twelite_uart_rx(char c);

#ifdef	__cplusplus
}
#endif

#endif	/* TWELITE_H */

