# Application components for Microchip PIC16F18326 MCU

<img src="./doc/pic16f18326.jpg" width="500">

## Background and motivation

My hobby is IoT/AI prototyping. When a prototype is expected to be small and simple, I use Microchip PIC16F18326 MCU rather than a NUCLEO board (STM32). I do not use Arduino or RasPi these days, because they are large and cosume electric power a lot. And I would use DSP rather than multi core MCUs or RasPi for AI.

=> [A companion project (STM32) of this project](https://github.com/araobp/stm32-mcu).

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
