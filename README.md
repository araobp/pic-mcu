# pic16f1-mcu

I developed my original PIC16F1 evaluation board in 2017, and I have used this board for a number of IoT demonstrations so far.

![](https://docs.google.com/drawings/d/e/2PACX-1vTHoT0TZIyVhAgkDVHyuWkc1-_6oFHT2mF53g2q36bgH_qxplkvvRIkJ3PqJBNuTZauhhMmSiemMoZO/pub?w=680&h=400)

For example, below is a hamster wheel to which a hall sensor is attached to count the number of pulses: how long a hamster runs at night.

![](./doc/hamster_wheel.jpg)

I usually use the board with RasPi or Android, but I use it as a stand-alone system for physics experiments this time.

## PIC16F18326

I develop a small 9-axis sensor unit with [PIC16F18326](http://ww1.microchip.com/downloads/en/DeviceDoc/40001839B.pdf) and [MPU9250](https://www.invensense.com/products/motion-tracking/9-axis/mpu-9250/). I will use this for experiments of mechanics (physics).

```
                            5V system
                          +-------------+
  [MPU9250]--- I2C --+----|             |--- UART ---> to PC or Android
  [EEPROM]---- I2C --+----| PIC16F18326 |
                     |    +------+------+
  [Character LCD] ---+           |
                            [Tactile SW]
```
