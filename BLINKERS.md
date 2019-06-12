# Blinkers

This is a remake of [another project that have used ST25DV04K with STM32](https://github.com/araobp/stm32-mcu/tree/master/NUCLEO-F401RE/NFC).

## Background

In some project, I need a simple device like this: one of LEDs in an array blinks as an indicator of some current action ongoing on another device. 

I have worked on this [Dynamic NFC tag project on STM32](https://github.com/araobp/stm32-mcu/tree/master/NUCLEO-F401RE/NFC) for a month, but its IDE from STMicro is unstable, and the code size tends to become large because of its multi-layer abstraction layers.

In my project, such abstraction is not so important, and the physical size matters. That's where PIC16F1 comes into play. In my experience, it does not matter if it is 8-bit or 32-bit MCU, when MCU is expected to work as a hub among hardware modules. PIC16F1 is the best choice for that purpose.

MPLAB-X IDE from Microchip has been very stable and reliable in my IoT projects at work so far.

## Architecture

```
             [Home appliance]
                    |
                   USB
                    |
        [FTDI USB/UART converter]
                    |
                   UART
                    |
              [PIC16F1829] @ 5V system
                    |
     +--------------+---------------+---------------+---------------+
     |              |               |               |               |
   [LED 0]        [LED 1]         [LED2]          [LED2]       [ST25DV-I2C]
[touch sensor] [touch sensor]  [touch sensor]  [touch sensor]
```

## Test code

(Since my PC is broken and going to be repaired, I will not be able to use MPLAB-X for a few weeks.)

For now, I have made a test code on Arduino to work with ST25DV04K. This is an IoT prototyping project, and the test code proves that a simple code like this is easier to maintain than those of STMicro's Cube IDE that is for product development.

[Test code](./arduino/)

When I hold my smart phone over the tag, it opens the URL with Chrome browser automatically!

```
ST25 system config: 88 3 1 0 C 7 0 E C F 0 0 0 0 7 0 0 0 0 0 7F 0 3 24 AF F 62 2 0 24 2 E0 
ST25 password: 0 0 0 0 0 0 0 0 
ST25 dynamic config: 88 FF 8 0 1 0 0 0 
NDEF CC file: E1 40 40 0 
NDEF message type: 3 
NDEF message length: 3A 
NDEF header: D1 1 36 55 
NDEF payload length: 55 
NDEF Identifier code: 4 
NDEF URI field:
github.com/araobp/pic16f1-mcu/bl
ob/master/BLINKERS.md
```

### NDEF format for URI(HTTPS)

```
+---------------+
|     0xE1      | CC File 4 bytes length
+---------------+ 
|     0x40      | CC File
+---------------+
|     0x40      | CC File
+---------------+
|     0x00      | CC File
+---------------+
|     0x03      | Meaning that this tag contains NDEF records.
+---------------+
|Payload len + 4| NDEF Head 4 bytes (short record, no ID)
+---------------+
|1|1|0|1|0|0|0|1| MB(1), ME(1), CF(0), SR(1), IL(0), TNF(001)
+---------------+
|     0x01      | Type length
+---------------+
|Payload length |
+---------------+
|    0x55 ('U') | URI
+---------------+
|     0x04      | HTTPS
+---------------+
|     URL       | Payload
|      :        |
|      :        |
+---------------+
```
