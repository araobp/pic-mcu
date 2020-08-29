/* 
 * File:   twelite_driver.c
 * Author: https://github.com/araobp
 *
 * Created on July 26, 2020, 12:15 PM
 */

#include "mcc_generated_files/mcc.h" 
#include "twelite_driver.h"
#include <stdint.h>

// EUSART1's Txd to TWELITE-DIP's rxd
#define putchar(c) EUSART1_Write(c)

void twelite_uart_tx(uint8_t *pbuf, uint8_t seq, uint8_t len) {
    static uint8_t cs;

    //--- Binary transfer mode header
    putchar(0xA5); // Binary transfer mode header
    putchar(0x5A); // Binary transfer mode header
    putchar(0x80); // Data length MSB
    putchar(len + 8); // Data length LSB
    //putchar(len+5);  // Data length LSB
    //--- Packet header
    putchar(DST_NODE); // Destination is "parent node"
    putchar(BYTE); // Byte (fixed)
    putchar(seq); // Sequence number
    //--- Options
    putchar(RESPONSE_MSG_DISABLED);
    putchar(ACK_ENABLED); // ACK enabled
    putchar(RESEND); // Resend
    putchar(NUM_RETRY); // The number of retries
    putchar(TERMINATOR); // Terminator
    //--- Payload
    cs = DST_NODE ^ BYTE ^ seq ^ RESPONSE_MSG_DISABLED ^ ACK_ENABLED ^ RESEND ^ NUM_RETRY ^ TERMINATOR;
    //cs = DST_NODE ^ BYTE ^ seq ^ RESPONSE_MSG_DISABLED ^ TERMINATOR;
    for (int i = 0; i < len; i++) { // Payload
        putchar(pbuf[i]);
        cs = cs ^ pbuf[i];
    }
    //--- Checksum
    putchar(cs); // Checksum    
}

/**
 * Note: this API is just for receiving an one-byte payload.
 * @param c
 * @return true if the input character is payload.
 */
bool twelite_uart_rx(uint8_t c, uint8_t *cmd, uint8_t *seq) {
    static uint8_t pos = 0;
    static uint8_t seq_ = 0;
    static uint8_t cmd_ = 0;
    bool eot_reached;

    eot_reached = false;
    switch (pos++) {
        case SEQ_NUMBER_POS:
            seq_ = c;
            break;
        case PAYLOAD_POS:
            cmd_ = c;
            break;
        case EOT_POS:
            if (c == EOT) {
                pos = 0;
                eot_reached = true;
            } else { // Out-of-sync
                // Software reset
                __delay_ms(RESET_DELAY);
                __asm__ volatile("reset");
            }
            break;
        default:
            break;
    }
    if (eot_reached) {
        *seq = seq_;
        *cmd = cmd_;
        return true;
    } else {
        return false;
    }
}