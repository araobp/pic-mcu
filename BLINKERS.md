# Blinkers

## Background

In some project, I need a simple device like this: one of LEDs in an array blinks as an indicator of some current action ongoing on another device. 

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
              [PIC16F18326]
                    |
     +--------------+---------------+
     |                              |   
   [LED 0]                        [LED 1]
  [QR code]                      [QR code]
  [touch sensor]                 [touch sensor]
  
```

Note 1: QR codes may be replaced with dynamic NFC tags such as ST25DV-I2C.
Note 2: ST25DV-I2C can detect that a smart phone is in its proximity, and report the event to a host MCU via GPO.

Although dynamic NFC tags is interesting, I would use QR codes rather than dynamic NFC tags for some reasons in this use case.

