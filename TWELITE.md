# Wireless network of infrared array sensors with TWELITE

## Hardware components

### TWELITE

[TWELITE](https://mono-wireless.com/en/) is a low-power wireless communication module based on IEEE802.15.4. Its core is 32bit MCU ["JN5164"](https://www.nxp.com/jp/products/wireless/proprietary-ieee-802.15.4-based/zigbee-and-ieee802.15.4-wireless-microcontroller-with-160-kb-flash-32-kb-ram:JN5164) from NXP.

### Infrared array sensor (Panasonic AMG8833)

I use [AMG8833](https://cdn-learn.adafruit.com/assets/assets/000/043/261/original/Grid-EYE_SPECIFICATIONS%28Reference%29.pdf?1498680225
) from Panasonic.

## Why I use PIC16F1 with TWELITE

I need to transfer 64 pixel images (128 bytes data per frame, 10 FPS) over TWELITE.

TWELITE provides SDK for developing an application, but a single core CPU cannot toralate a relatively longer blocking time of I/O processing.

The SDK supports event-driven APIs to cope with such a problem, but I guess the CPU still cannot satisfy the requirement.

I use 8bit MCU as a co-processor of TWELITE just for receiving data from an infrared array sensor on I2C bus and transfer the data to TWELITE via UART.

## 8bit quantization

- Payload size of TWELITE's packet is 80, so I need to compress the image data.
- The infrared array sensor outputs temperature data of each pixel in a range of 0 to 80 degreees Celsius.
- Room temperature is usually in a range of 10 to 40.

So I just ignore MSB bytes from the sensor, and transfer LSB bytes to TWELITE. I use 0xFF as a delimiter of data.

Temperature range: 0 - 63.5 degrees Celsius (63.5/0.25 = 0xfe)
