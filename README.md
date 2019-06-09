# Application components for Microchip PIC16F18326 MCU

<img src="./doc/pic16f18326.jpg" width="500">

## Background and motivation

In the past three years, PIC16F1 has been the most useful MCU in my IoT projects at work.

## Application components

### Wireless network of infrared array sensors with TWELITE 

<img src="./doc/twelite-dip.jpg" width="500">

=> **[Project page](TWELITE.md)**

### Motion logger for sensing human body movement

<img src="./doc/motion_measurement_system.jpg" width="500">

=> **[Project page](MOTION_LOGGER.md)**

### Door controller

<img src="./doc/door_controller.jpg" width="400">

=> **[Project page](DOOR_CONTROLLER.md)**

### Blikers

=> **[Project page](BLINKERS.md)**

## Tips

#### Microchip XC8 C compiler

C standard "C99" does not allow some of MCC-generated code. Change the setting to "C90".

![](./doc/C90_standard.jpg)

#### Atmel EEPROM

Data addresses for page write must be 64^n.
