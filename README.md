# pic16f1-mcu

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
