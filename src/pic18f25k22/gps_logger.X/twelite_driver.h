/* 
 * File:   twelite_driver.h
 * Author: https://github.com/araobp
 *
 * Created on July 26, 2020, 12:15 PM
 */

#ifndef TWELITE_DRIVER_H
#define	TWELITE_DRIVER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define TWELITE_WAKEUP_TIME 1  // 1 msec

// TWELITE packet
#define DST_NODE 0x00
#define BYTE 0xA0
#define RESPONSE_MSG_DISABLED 0x07
#define ACK_ENABLED 0x01
#define RESEND 0x02
#define TERMINATOR 0xFF
#define EOT 0x04

// The number of retries
#define NUM_RETRY 3U

// Data position in a packet
#define SEQ_NUMBER_POS 6U
#define PAYLOAD_POS 18U
#define EOT_POS 20U
    
// Delay before reset in case of out-of-sync
#define RESET_DELAY 50U  // 50msec
    
void twelite_uart_tx(uint8_t *pbuf, uint8_t seq, uint8_t len);
bool twelite_uart_rx(uint8_t c, uint8_t *cmd, uint8_t *seq);

#ifdef	__cplusplus
}
#endif

#endif	/* TWELITE_DRIVER_H */


