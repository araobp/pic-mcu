# Application components for Microchip PIC16F18326 MCU

<img src="./doc/pic16f18326.jpg" width="500">

## Motivation

=> [A companion project (STM32) of this project](https://github.com/araobp/stm32-mcu).

## Why PIC16F1?

I developed my original PIC16F1 evaluation board in 2017, and I have used this board for a number of IoT demonstrations so far. When a demo is expected to be small and simple, the board is more useful than other commercial boards such as STM32 NUCLEO.

![](https://docs.google.com/drawings/d/e/2PACX-1vTHoT0TZIyVhAgkDVHyuWkc1-_6oFHT2mF53g2q36bgH_qxplkvvRIkJ3PqJBNuTZauhhMmSiemMoZO/pub?w=680&h=400)

## Application components

### Wireless network of infrared array sensors wireless with TWELITE 

<img src="./doc/twelite-dip.jpg" width="500">

=> **[Code](TWELITE.md)**

### Motion logger for sensing human body movement

<img src="./doc/motion_measurement_system.jpg" width="500">

=> **[Code](MOTION_LOGGER.md)**

## Tips

#### Microchip XC8 C compiler

C standard "C99" does not allow some of MCC-generated code. Change the setting to "C90".

![](./doc/C90_standard.jpg)

#### Atmel EEPROM

Data addresses for page write must be 64^n.
