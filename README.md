# Software components of IoT/AI on Microchip PIC16F1 

![](./doc/motion_measurement_system.jpg)

## Background

Use [PIC16F1 DIP (14 pins, such as PIC16F18326 or PIC16F1825)](http://microchipdeveloper.com/mcu1102:start) to develop a measuring instrument for IoT/AI. It is very small and very cheap, but it has a lot of capabilities comparable to Arduino.

#### My original PIC16F1 evaluation board

I developed my original PIC16F1 evaluation board in 2017, and I have used this board for a number of IoT demonstrations so far. When a demo is expected to be small and simple, the board is more useful than other commercial boards such as Arduino or STM32 Nucleo. The total cost of the board (~ $10) is much cheaper than Arduino Uno.

![](https://docs.google.com/drawings/d/e/2PACX-1vTHoT0TZIyVhAgkDVHyuWkc1-_6oFHT2mF53g2q36bgH_qxplkvvRIkJ3PqJBNuTZauhhMmSiemMoZO/pub?w=680&h=400)

I usually use the board with RasPi or Android for IoT demo, but I use it as a stand-alone system for physics experiments this time.

## Components in development

- [Motion logger](MOTION_LOGGER.md)
- [I2C adaptor for TWELITE](TWELITE.md]

## Tips

#### Microchip XC8 C compiler

C standard "C99" does not allow some of MCC-generated code. Change the setting to "C90".

![](./doc/C90_standard.jpg)

#### Atmel EEPROM

Data addresses for page write must be 64^n.
