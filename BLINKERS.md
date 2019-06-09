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
  [QR code or NFC tag]           [QR code or NFC tag]
  [touch sensor]                 [touch sensor]
  
```

Note 1: QR codes may be replaced with dynamic NFC tags such as ST25DV-I2C.

Note 2: ST25DV-I2C can detect that a smart phone is in its proximity, and report the event to a host MCU via GPO.

Although dynamic NFC tags are interesting, I would rather use QR codes or static NFC tags than dynamic NFC tags for some reasons in this project.

