#include "mcc_generated_files/mcc.h" 
#include "twelite.h"
#include <stdint.h>
#include <stdio.h>

void twelite_uart_tx(char *pbuf, uint8_t len) {
    static uint8_t seq = 0;
    static uint8_t cs;
    
    // Checksum
    cs = 0x00 ^ 0xA0 ^ seq ^ 0x01 ^ 0xff;
    
    //--- Binary transfer mode header
    putchar(0xA5); // Binary transfer mode header
    putchar(0x5A); // Binary transfer mode header
    putchar(0x80); // Data length MSB
    putchar((char)(len+4));  // Data length LSB
    //--- Packet header
    putchar(DST_NODE);  // Destination is "parent node"
    putchar(BYTE);  // Byte (fixed)
    putchar((char)seq);  // Sequence number
    //--- Option
    //putchar(ACK_ENABLED);  // ACK enabled
    putchar(TERMINATOR);  // Terminator
    //--- Payload
    for(int i=0;i<len;i++) {  // Payload
        putchar(pbuf[i]);
        cs = cs ^ pbuf[i];
    }
    //--- Checksum
    putchar((char)cs);  // Checksum
    
    if (++seq > 0xFF) seq = 0;
}

/**
 * Note: this API is just for receiving an one-byte payload.
 * @param c
 * @return true if the input character is payload.
 */
bool twelite_uart_rx(char c) {
    static uint8_t pos = 0;
    bool is_payload = false;
    switch (pos) {
        case PAYLOAD_POS:
            is_payload = true;
            break;
        case EOT_POS:
            if (c == EOT) {
                pos = 0;
            } else {  // Out-of-sync
                // Software reset
                __delay_ms(1000);
                __asm__ volatile("reset");
            } 
            is_payload = false;
            break;
        default:
            is_payload=false;
            break;
    }
    pos++;
    return is_payload;
}
