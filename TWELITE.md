# Wireless network of infrared array sensors with TWELITE

## TWELITE

[TWELITE](https://mono-wireless.com/en/) is a low-power wireless communication module based on IEEE802.15.4. Its core is 32bit MCU ["JN5164"](https://www.nxp.com/jp/products/wireless/proprietary-ieee-802.15.4-based/zigbee-and-ieee802.15.4-wireless-microcontroller-with-160-kb-flash-32-kb-ram:JN5164) from NXP.

## Why I use PIC16F1 with TWELITE

I need to transfer 64 pixel images (128 bytes data per frame, 10 FPS) over TWELITE.

TWELITE provides SDK for developing an application, but a single core CPU cannot toralate a relatively longer blocking time of I/O processing.

The SDK supports event-driven APIs to cope with such a problem, but I guess the CPU still cannot satisfy the requirement.

I use 8bit MCU as a co-processor of TWELITE just for receiving data from an infrared array sensor on I2C bus and transfer the data to TWELITE via UART.
