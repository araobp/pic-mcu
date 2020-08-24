# Application components for Microchip PIC MCUs

<img src="./doc/pic16f18326.jpg" width="500">

## Background and motivation

In the past years, I have used a lot of the following MCUs in my successful IoT projects at work:
- PIC1825
- PIC1829
- PIC18326
- PIC18F25K22

## Application components

### Oscilloscope for nine-axis sensor (PIC16F1825 with MPU9250 and Android)

<img src="./doc/oscilloscope_for_6_axis_sensor.png" width="500">

=> **[Project page](I2C_LIB_EVAL.md)**

### Motion logger for sensing human body movement (PIC16F18326)

<img src="./doc/motion_measurement_system.jpg" width="500">

=> **[Project page](MOTION_LOGGER.md)**

### Wireless network of infrared array sensors with TWELITE (PIC16F18326)

<img src="./doc/amg8833_android_gui.png" width="250">

=> **[Project page](TWELITE.md)**

### Door controller (PIC16F1825)

<img src="./doc/door_controller.jpg" width="400">

=> **[Project page](DOOR_CONTROLLER.md)**

## Tips

#### Microchip XC8 C compiler

C standard "C99" does not allow some of MCC-generated code. Change the setting to "C90".

![](./doc/C90_standard.jpg)

#### Atmel EEPROM

Data addresses for page write must be 64^n.
