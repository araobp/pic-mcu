# Application components for Microchip PIC MCUs

<img src="./doc/pic16f18326.jpg" width="500">

## Background and motivation

In the past years, I have used a lot of the following MCUs in my successful IoT projects at work:
- PIC1825
- PIC1829
- PIC18326
- PIC18F25K22

## Application components

### Wireless network of infrared array sensors with TWELITE (PIC16F18326)

<img src="./doc/twelite-dip.jpg" width="500">

=> **[Project page](TWELITE.md)**

### Motion logger for sensing human body movement (PIC16F18326)

<img src="./doc/motion_measurement_system.jpg" width="500">

=> **[Project page](MOTION_LOGGER.md)**

### Door controller (PIC1825)

<img src="./doc/door_controller.jpg" width="400">

=> **[Project page](DOOR_CONTROLLER.md)**

### Evaluation of new Microchip I2C library (PIC16F1825)

(Work in progress)

```
 [InvenSense MPU-9255]--I2C--[PIC16F1825]--USB serial--[TeraTerm on Win10 PC]

```

## Tips

#### Microchip XC8 C compiler

C standard "C99" does not allow some of MCC-generated code. Change the setting to "C90".

![](./doc/C90_standard.jpg)

#### Atmel EEPROM

Data addresses for page write must be 64^n.
