# Wireless network of infrared array sensors with TWELITE

![](./doc/twelite-dip.jpg)

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

Note: the sensor also outputs temperature data from a thermistor on the chip. I transfer both MSB and LSB in this case.

## Feature extraction on PIC16F1

In spite of 8bit quantization, the load on TWELITE is still heavy. It may require further processing to calculate features on PIC16F1 before transmitting the data to the master node.

Features for example:
- Diff between the current value and the previous value for each pixel.
- Diff average along each row.

## Command sequence

This command sequence (polling) is optimized for decreasing the power consumption.

```
 PIC16F1825           
 (as server)          Client
     |                  |     Time taken for the operation (approx.)
     |<----- 'p' -------|        10msec
     |---- pixels ----->|        50msec
     |        :         |
     |<----- 't' -------|        10msec
     |--- thermistor -->|        15msec
     |        :         |
```

The bottle neck of data transfer is the following:
- I2C (250kHz = 250kbps)
- UART (115200bps)
- IEEE802.15.4 PHY (250kbps)
- And buffering at each interface

## Specification of the co-processor (PIC16F1825 or PIC16F18326)

|          | Value                    |
|----------|--------------------------|
|VDD       | 3.0V DC (AAA battery x 2)|
|Power consumption| a few mA          |
|CPU Clock | 16MHz HF                 |
|UART      | 115200bps                |
|I2C Clock | 250kHz                   |

Note: I tried lower CPU clock frequencies, but UART did not work with TWELITE-DIP.

## Code

- [Master(Python) with TWELITE-STICK](./python/twelite)
- [Slave(PIC16F18326) with TWELITE-DIP](./src/pic16f18326/amg8833.X)

Test:
```
--- SLAVE 1 ---
 length: 2, seq number: 34, LQI: 144
 room temperature: 19.125 degrees Celsius

 length: 64, seq number: 35, LQI: 147
 pixels: 16.25,16.5,16.75,16.5,16.25,16.25,16.25,16.5,16.25,16.5,16.25,16.75,16.75,15.75,16.0,16.25,16.25,16.0,16.5,16.5,16.0,15.75,16.0,16.5,16.0,16.25,16.0,16.5,16.0,15.5,15.75,17.0,16.0,16.75,16.0,15.75,16.25,15.25,15.5,16.25,16.75,16.75,16.5,16.0,16.5,16.0,16.25,16.0,15.75,16.75,16.0,15.5,15.75,16.25,16.25,15.5,16.0,16.0,15.0,15.25,15.5,15.5,15.25,15.25 degrees Celsius

--- SLAVE 1 ---
 length: 2, seq number: 36, LQI: 147
 room temperature: 19.1875 degrees Celsius

 length: 64, seq number: 37, LQI: 147
 pixels: 16.25,16.5,17.0,16.75,16.25,16.25,16.25,16.25,16.0,16.25,16.25,16.5,16.5,16.25,15.75,16.25,16.25,16.25,16.75,16.75,16.25,15.75,16.0,16.75,16.25,16.25,16.0,16.25,16.0,15.0,15.5,16.5,16.0,16.75,16.25,16.0,16.5,15.5,15.75,16.5,16.5,16.75,16.5,16.0,16.25,16.25,16.25,16.25,15.5,16.75,15.75,15.5,16.25,16.0,15.25,15.75,15.5,15.5,14.75,14.75,15.75,15.5,15.75,15.5 degrees Celsius
 
                            :
```

## Power saving

PIC16F1 controls FET for power control.

```
 PIC16F1825
 (as server)          Client
     |                  |
     |---- 'h' -------->|  'h(ello)'
     |<-----------------|  Start polling to read data or respond with 'k(eep on)' to disable the periodic process.
     |        :         |
     |                  |  Stop polling
     |                  |  (after 3 minutes)
 Stop power supply to TWELITE-DIP and AMG8833
     |                  |  (every 10 minutes)
 Start power supply to TWELITE-DIP and AMG8833
     |                  |  (after 2 second)
     |---- 'h' -------->|
     |                  |  (after 3 second)
 Stop power supply TWELITE-DIP and AMG8833
     |                  |     
```

| Message | Description                            | Request | Response        |
|---------|----------------------------------------|---------|-----------------|
| h       | hello                                  | S -> C  | (start polling) |
| t       | thermistor                             | S <- C  | data            |
| p       | 64 pixels                              | S <- C  | data            |
| d       | 64 pixels diff                         | S <- C  | data            |
| D       | avarages of diff of each rows          | S <- C  | data            |
| k       | keep on (disable power saving process) | S <- C  | (none)          |

## Reference

- https://mono-wireless.com/jp/products/TWE-APPS/App_Uart/mode_format.html
