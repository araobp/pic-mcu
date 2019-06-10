# Blinkers

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
