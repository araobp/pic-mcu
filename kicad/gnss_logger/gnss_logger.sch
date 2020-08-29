EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "GPS & I2C Logger with TWELITE"
Date "2020-08-08"
Rev "rev 0.6"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L i2c_adaptor:Q_NJFET_DGS Q1
U 1 1 5F1561CD
P 6600 3450
F 0 "Q1" H 6791 3496 50  0000 L CNN
F 1 "Fairchild BS170" H 6791 3405 50  0000 L CNN
F 2 "pic:BS170" H 6800 3550 50  0001 C CNN
F 3 "" H 6600 3450 50  0001 C CNN
	1    6600 3450
	1    0    0    -1  
$EndComp
$Comp
L i2c_adaptor:Conn_01x03 J6
U 1 1 5F158C25
P 9550 1300
F 0 "J6" V 9514 1112 50  0000 R CNN
F 1 "Conn_01x03" V 9423 1112 50  0000 R CNN
F 2 "pic:Pin_Header_3P" H 9550 1300 50  0001 C CNN
F 3 "" H 9550 1300 50  0001 C CNN
	1    9550 1300
	0    -1   -1   0   
$EndComp
$Comp
L i2c_adaptor:Conn_01x14 J5
U 1 1 5F159539
P 8500 5050
F 0 "J5" H 8580 5042 50  0000 L CNN
F 1 "Conn_01x14" H 8580 4951 50  0000 L CNN
F 2 "pic:Pinsocket_14pin" H 8500 5050 50  0001 C CNN
F 3 "" H 8500 5050 50  0001 C CNN
	1    8500 5050
	1    0    0    -1  
$EndComp
$Comp
L i2c_adaptor:Conn_01x14 J7
U 1 1 5F15A15F
P 9450 5150
F 0 "J7" H 9368 4225 50  0000 C CNN
F 1 "Conn_01x14" H 9368 4316 50  0000 C CNN
F 2 "pic:Pinsocket_14pin" H 9450 5150 50  0001 C CNN
F 3 "" H 9450 5150 50  0001 C CNN
	1    9450 5150
	-1   0    0    1   
$EndComp
NoConn ~ 5250 2100
Wire Wire Line
	7550 6050 7550 4650
Wire Wire Line
	8300 5350 7450 5350
Wire Wire Line
	7450 5350 7450 5950
$Comp
L i2c_adaptor:GND #PWR03
U 1 1 5F17815B
P 6700 4000
F 0 "#PWR03" H 6700 3750 50  0001 C CNN
F 1 "GND" H 6705 3827 50  0000 C CNN
F 2 "" H 6700 4000 50  0001 C CNN
F 3 "" H 6700 4000 50  0001 C CNN
	1    6700 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 3650 6700 4000
Wire Wire Line
	7850 4450 8300 4450
$Comp
L i2c_adaptor:+3.3V #PWR08
U 1 1 5F17C0E5
P 10100 3750
F 0 "#PWR08" H 10100 3600 50  0001 C CNN
F 1 "+3.3V" H 10115 3923 50  0000 C CNN
F 2 "" H 10100 3750 50  0001 C CNN
F 3 "" H 10100 3750 50  0001 C CNN
	1    10100 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	10100 3750 10100 4450
Wire Wire Line
	10100 5050 9650 5050
Wire Wire Line
	9650 4950 10100 4950
Connection ~ 10100 4950
Wire Wire Line
	10100 4950 10100 5050
Wire Wire Line
	9650 4850 10100 4850
Connection ~ 10100 4850
Wire Wire Line
	10100 4850 10100 4950
Wire Wire Line
	9650 4750 10100 4750
Connection ~ 10100 4750
Wire Wire Line
	10100 4750 10100 4850
Wire Wire Line
	9650 4450 10100 4450
Connection ~ 10100 4450
Wire Wire Line
	10100 4450 10100 4750
NoConn ~ 8300 4550
NoConn ~ 8300 4750
NoConn ~ 8300 4850
NoConn ~ 8300 4950
NoConn ~ 8300 5050
NoConn ~ 8300 5150
NoConn ~ 8300 5250
NoConn ~ 8300 5450
NoConn ~ 8300 5550
NoConn ~ 8300 5650
NoConn ~ 8300 5750
NoConn ~ 9650 4650
NoConn ~ 9650 5150
NoConn ~ 9650 5250
NoConn ~ 9650 5350
NoConn ~ 9650 5450
NoConn ~ 9650 5550
NoConn ~ 9650 5650
NoConn ~ 9650 5750
$Comp
L i2c_adaptor:Conn_01x05 J3
U 1 1 5F156995
P 5050 1900
F 0 "J3" H 4968 2317 50  0000 C CNN
F 1 "Conn_01x05" H 4968 2226 50  0000 C CNN
F 2 "pic:Pin Socket 5P" H 5050 1900 50  0001 C CNN
F 3 "" H 5050 1900 50  0001 C CNN
	1    5050 1900
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5250 2000 5850 2000
Wire Wire Line
	5250 1900 5950 1900
$Comp
L i2c_adaptor:GND #PWR06
U 1 1 5F1613B1
P 9650 1750
F 0 "#PWR06" H 9650 1500 50  0001 C CNN
F 1 "GND" H 9655 1577 50  0000 C CNN
F 2 "" H 9650 1750 50  0001 C CNN
F 3 "" H 9650 1750 50  0001 C CNN
	1    9650 1750
	1    0    0    -1  
$EndComp
$Comp
L i2c_adaptor:+3.3V #PWR07
U 1 1 5F163029
P 10550 850
F 0 "#PWR07" H 10550 700 50  0001 C CNN
F 1 "+3.3V" H 10565 1023 50  0000 C CNN
F 2 "" H 10550 850 50  0001 C CNN
F 3 "" H 10550 850 50  0001 C CNN
	1    10550 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	9550 1500 9550 1600
Wire Wire Line
	9550 1600 10550 1600
Wire Wire Line
	10550 1600 10550 1400
$Comp
L i2c_adaptor:SW_SPDT SW1
U 1 1 5F169EA5
P 8050 2550
F 0 "SW1" V 8096 2362 50  0000 R CNN
F 1 "SW_SPDT" V 8005 2362 50  0000 R CNN
F 2 "pic:slide_switch" H 8050 2550 50  0001 C CNN
F 3 "" H 8050 2550 50  0001 C CNN
	1    8050 2550
	0    -1   -1   0   
$EndComp
$Comp
L i2c_adaptor:Conn_01x02 J4
U 1 1 5F165806
P 7950 1300
F 0 "J4" V 7914 1112 50  0000 R CNN
F 1 "Conn_01x02" V 7823 1112 50  0000 R CNN
F 2 "pic:Pin Header 2P" H 7950 1300 50  0001 C CNN
F 3 "" H 7950 1300 50  0001 C CNN
	1    7950 1300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9650 1500 9650 1750
Wire Wire Line
	9450 1500 9450 3050
NoConn ~ 8150 2350
$Comp
L i2c_adaptor:GND #PWR05
U 1 1 5F19FB80
P 8050 1650
F 0 "#PWR05" H 8050 1400 50  0001 C CNN
F 1 "GND" H 8055 1477 50  0000 C CNN
F 2 "" H 8050 1650 50  0001 C CNN
F 3 "" H 8050 1650 50  0001 C CNN
	1    8050 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 1650 8050 1500
Text Notes 7650 850  0    50   ~ 0
4.5V buttery case\n(AAA x 3)
Text Notes 8750 4300 0    50   ~ 0
TWELITE DIP
Wire Notes Line
	8150 4250 8150 5950
Wire Notes Line
	8150 5950 9800 5950
Wire Notes Line
	9800 5950 9800 4250
Wire Notes Line
	9800 4250 9250 4250
Wire Notes Line
	8150 4250 8700 4250
Text Notes 8600 4700 0    50   ~ 0
RX
Text Notes 8600 5400 0    50   ~ 0
TX
Text Notes 9200 4500 0    50   ~ 0
VCC
Text Notes 8600 4500 0    50   ~ 0
GND
Wire Notes Line
	5050 2150 5050 2350
Wire Notes Line
	5050 1450 5050 1650
Text Notes 3850 1900 0    50   ~ 0
GPS receiver module\nAE-GYSFDMAXB
Wire Notes Line
	5050 1450 3800 1450
Wire Notes Line
	3800 1450 3800 2350
Wire Notes Line
	3800 2350 5050 2350
Text Notes 4850 1750 0    50   ~ 0
5V
Text Notes 4800 1850 0    50   ~ 0
GND
Text Notes 4800 1950 0    50   ~ 0
RXD
Text Notes 4800 2050 0    50   ~ 0
TXD
Text Notes 4750 2150 0    50   ~ 0
1PPS
Wire Wire Line
	7550 3050 7550 1700
Wire Wire Line
	7550 1700 5250 1700
Text Notes 6350 1650 0    50   ~ 0
3.8V - 12V
Text Notes 7900 1200 0    50   ~ 0
+
Text Notes 8000 1200 0    50   ~ 0
-
Text Notes 9500 1200 1    50   ~ 0
VIN
Text Notes 9600 1200 1    50   ~ 0
OUT
Text Notes 9700 1200 1    50   ~ 0
GND
Text Notes 9300 950  0    50   ~ 0
LDO module\nAMS1117
Wire Notes Line
	9400 1300 9250 1300
Wire Notes Line
	9250 1300 9250 700 
Wire Notes Line
	9250 700  9850 700 
Wire Notes Line
	9850 700  9850 1300
Wire Notes Line
	9850 1300 9700 1300
Wire Notes Line
	7900 1300 7550 1300
Wire Notes Line
	7550 1300 7550 600 
Wire Notes Line
	8100 1300 8450 1300
Wire Notes Line
	8450 1300 8450 600 
Wire Notes Line
	8450 600  7550 600 
Text Notes 9250 5000 0    50   ~ 0
AI2
Text Notes 9250 4900 0    50   ~ 0
AI3
Text Notes 9250 4800 0    50   ~ 0
AI4
Text Notes 9250 5100 0    50   ~ 0
AI1
$Comp
L i2c_adaptor:PWR_FLAG #FLG0102
U 1 1 5F2FA7B0
P 10750 1200
F 0 "#FLG0102" H 10750 1275 50  0001 C CNN
F 1 "PWR_FLAG" H 10750 1373 50  0000 C CNN
F 2 "" H 10750 1200 50  0001 C CNN
F 3 "" H 10750 1200 50  0001 C CNN
	1    10750 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	10750 1200 10750 1400
Wire Wire Line
	10750 1400 10550 1400
Connection ~ 10550 1400
Wire Wire Line
	10550 1400 10550 850 
$Comp
L i2c_adaptor:+3.3V #PWR0102
U 1 1 5F1AB036
P 6900 4850
F 0 "#PWR0102" H 6900 4700 50  0001 C CNN
F 1 "+3.3V" H 6915 5023 50  0000 C CNN
F 2 "" H 6900 4850 50  0001 C CNN
F 3 "" H 6900 4850 50  0001 C CNN
	1    6900 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 3450 6400 3450
Wire Wire Line
	7550 3050 8050 3050
Wire Wire Line
	7950 2350 7950 2200
Wire Wire Line
	8050 2750 8050 3050
Connection ~ 8050 3050
Wire Wire Line
	8050 3050 9450 3050
Wire Wire Line
	7950 1900 7950 1500
$Comp
L i2c_adaptor:Polyfuse F1
U 1 1 5F169822
P 7950 2050
F 0 "F1" H 7862 2004 50  0000 R CNN
F 1 "Polyfuse(100mA)" H 7862 2095 50  0000 R CNN
F 2 "pic:Pin Header 2P" H 8000 1850 50  0001 L CNN
F 3 "" H 7950 2050 50  0001 C CNN
	1    7950 2050
	-1   0    0    1   
$EndComp
Text GLabel 10300 4550 2    50   Input ~ 0
SLEEP
Wire Wire Line
	9650 4550 10300 4550
$Comp
L i2c_adaptor:GND #PWR04
U 1 1 5F309F92
P 7850 4950
F 0 "#PWR04" H 7850 4700 50  0001 C CNN
F 1 "GND" H 7855 4777 50  0000 C CNN
F 2 "" H 7850 4950 50  0001 C CNN
F 3 "" H 7850 4950 50  0001 C CNN
	1    7850 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 4450 7850 4950
Wire Wire Line
	4800 6150 4450 6150
Text GLabel 4800 6150 2    50   Input ~ 0
SLEEP
Text Notes 1000 6550 0    50   ~ 0
I2C extension
Wire Notes Line
	1650 6600 1650 7400
Wire Notes Line
	900  6600 1650 6600
Wire Notes Line
	900  7400 900  6600
Wire Notes Line
	1650 7400 900  7400
Text Notes 1050 6950 0    50   ~ 0
SDA
Text Notes 1050 7050 0    50   ~ 0
SCL
Text Notes 1050 7150 0    50   ~ 0
GND
Text Notes 1050 7250 0    50   ~ 0
3.3V
Wire Wire Line
	4700 6250 4700 6900
Wire Wire Line
	3050 6250 3050 7000
Wire Wire Line
	2950 7100 2950 7400
Wire Wire Line
	1500 7100 2950 7100
Wire Wire Line
	1500 7200 2250 7200
$Comp
L power:GND #PWR09
U 1 1 5F32B74E
P 2950 7400
F 0 "#PWR09" H 2950 7150 50  0001 C CNN
F 1 "GND" H 2955 7227 50  0000 C CNN
F 2 "" H 2950 7400 50  0001 C CNN
F 3 "" H 2950 7400 50  0001 C CNN
	1    2950 7400
	1    0    0    -1  
$EndComp
$Comp
L i2c_adaptor:+3.3V #PWR02
U 1 1 5F3220EB
P 2250 6300
F 0 "#PWR02" H 2250 6150 50  0001 C CNN
F 1 "+3.3V" H 2265 6473 50  0000 C CNN
F 2 "" H 2250 6300 50  0001 C CNN
F 3 "" H 2250 6300 50  0001 C CNN
	1    2250 6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 6250 4700 6250
Wire Wire Line
	3250 6250 3050 6250
$Comp
L Connector_Generic:Conn_01x04 J1
U 1 1 5F2C7437
P 1300 7100
F 0 "J1" H 1218 6675 50  0000 C CNN
F 1 "Conn_01x04" H 1218 6766 50  0000 C CNN
F 2 "pic:pin-socket-4p" H 1300 7100 50  0001 C CNN
F 3 "~" H 1300 7100 50  0001 C CNN
	1    1300 7100
	-1   0    0    1   
$EndComp
Text Notes 3950 6300 0    50   ~ 0
SDA1
Text Notes 3550 6300 0    50   ~ 0
SCL1
Wire Wire Line
	5250 6750 5250 7300
Connection ~ 5250 6750
Wire Wire Line
	5500 6750 5250 6750
Wire Wire Line
	5500 6550 5500 6750
$Comp
L i2c_adaptor:PWR_FLAG #FLG0101
U 1 1 5F254B46
P 5500 6550
F 0 "#FLG0101" H 5500 6625 50  0001 C CNN
F 1 "PWR_FLAG" H 5500 6723 50  0000 C CNN
F 2 "" H 5500 6550 50  0001 C CNN
F 3 "" H 5500 6550 50  0001 C CNN
	1    5500 6550
	1    0    0    -1  
$EndComp
$Comp
L i2c_adaptor:+3.3V #PWR0103
U 1 1 5F251CC7
P 6350 4550
F 0 "#PWR0103" H 6350 4400 50  0001 C CNN
F 1 "+3.3V" H 6365 4723 50  0000 C CNN
F 2 "" H 6350 4550 50  0001 C CNN
F 3 "" H 6350 4550 50  0001 C CNN
	1    6350 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 4850 6350 4900
Wire Wire Line
	2450 4950 2650 4950
Wire Wire Line
	2450 3850 2450 4950
$Comp
L i2c_adaptor:R R2
U 1 1 5F15E2F6
P 2800 4950
F 0 "R2" V 2593 4950 50  0000 C CNN
F 1 "51k" V 2684 4950 50  0000 C CNN
F 2 "pic:Register" V 2730 4950 50  0001 C CNN
F 3 "" H 2800 4950 50  0001 C CNN
	1    2800 4950
	0    1    1    0   
$EndComp
Wire Wire Line
	2950 4950 3100 4950
Wire Wire Line
	6150 5150 6150 3450
Wire Wire Line
	4450 5150 6150 5150
Connection ~ 4800 5450
Wire Wire Line
	4600 5750 4450 5750
Wire Wire Line
	4600 5450 4600 5750
Wire Wire Line
	4800 5450 4600 5450
Wire Wire Line
	3400 4350 4950 4350
Wire Wire Line
	4950 4350 4950 5450
Wire Wire Line
	4800 5850 4800 5750
Connection ~ 4800 5850
Wire Wire Line
	5250 5850 4800 5850
Wire Wire Line
	4450 5850 4800 5850
Connection ~ 5250 5850
Wire Wire Line
	5250 4250 5250 5850
Wire Wire Line
	5250 5850 5250 6750
$Comp
L i2c_adaptor:GND #PWR0101
U 1 1 5F1A733D
P 5250 7300
F 0 "#PWR0101" H 5250 7050 50  0001 C CNN
F 1 "GND" H 5255 7127 50  0000 C CNN
F 2 "" H 5250 7300 50  0001 C CNN
F 3 "" H 5250 7300 50  0001 C CNN
	1    5250 7300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 4950 4450 4950
Connection ~ 5500 4950
Wire Wire Line
	5500 4150 5500 4950
Wire Wire Line
	3600 4150 5500 4150
Wire Wire Line
	3600 3900 3600 4150
NoConn ~ 3250 6150
NoConn ~ 3250 6050
NoConn ~ 3250 5850
NoConn ~ 3250 5750
NoConn ~ 3250 5650
NoConn ~ 3250 5550
NoConn ~ 3250 5450
NoConn ~ 3250 5350
NoConn ~ 3250 5250
NoConn ~ 3250 5150
NoConn ~ 3250 5050
NoConn ~ 4450 5650
NoConn ~ 4450 5550
NoConn ~ 4450 5450
NoConn ~ 4450 5350
Text Notes 3350 3250 0    50   ~ 0
PICkit3
Wire Notes Line
	2950 3300 2950 4000
Wire Notes Line
	4050 3300 2950 3300
Wire Notes Line
	4050 4000 4050 3300
Wire Notes Line
	2950 4000 4050 4000
Text Notes 3350 3600 1    50   ~ 0
MCLR
Text Notes 3450 3600 1    50   ~ 0
VDD
Text Notes 3550 3600 1    50   ~ 0
VSS
Text Notes 3650 3600 1    50   ~ 0
PGD
Text Notes 3750 3600 1    50   ~ 0
PGC
Text Notes 4150 6100 2    50   ~ 0
TX1
Text Notes 4150 6000 2    50   ~ 0
RX1
Text Notes 4150 5100 2    50   ~ 0
TX2
Text Notes 4150 5000 2    50   ~ 0
RX2
Wire Wire Line
	4800 5450 4950 5450
Wire Wire Line
	4800 5550 4800 5450
Wire Wire Line
	5850 2000 5850 4950
Wire Wire Line
	3100 4950 3250 4950
Connection ~ 3100 4950
Wire Wire Line
	3100 3900 3100 4950
Wire Wire Line
	3300 3900 3100 3900
Wire Wire Line
	3400 3900 3400 4350
Wire Wire Line
	3500 4250 5250 4250
Wire Wire Line
	3500 3900 3500 4250
Wire Wire Line
	5600 5050 4450 5050
Connection ~ 5600 5050
Wire Wire Line
	5600 4050 5600 5050
Wire Wire Line
	3700 4050 5600 4050
Wire Wire Line
	3700 3900 3700 4050
$Comp
L i2c_adaptor:Conn_01x05 J2
U 1 1 5F199D56
P 3500 3700
F 0 "J2" V 3464 3412 50  0000 R CNN
F 1 "Conn_01x05" V 3373 3412 50  0000 R CNN
F 2 "pic:Pin Socket 5P" H 3500 3700 50  0001 C CNN
F 3 "" H 3500 3700 50  0001 C CNN
	1    3500 3700
	0    -1   -1   0   
$EndComp
$Comp
L i2c_adaptor:R R1
U 1 1 5F190923
P 6350 5050
F 0 "R1" H 6420 5096 50  0000 L CNN
F 1 "2k" H 6420 5005 50  0000 L CNN
F 2 "pic:Register" V 6280 5050 50  0001 C CNN
F 3 "" H 6350 5050 50  0001 C CNN
	1    6350 5050
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D1
U 1 1 5F18F0B9
P 6350 4700
F 0 "D1" V 6389 4582 50  0000 R CNN
F 1 "LED red" V 6298 4582 50  0000 R CNN
F 2 "pic:LED" H 6350 4700 50  0001 C CNN
F 3 "~" H 6350 4700 50  0001 C CNN
	1    6350 4700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7450 5950 4450 5950
Wire Wire Line
	4450 6050 7550 6050
Wire Wire Line
	5950 5050 5600 5050
Wire Wire Line
	5950 1900 5950 5050
Wire Wire Line
	5850 4950 5500 4950
$Comp
L gnss_logger:pic18f25k22 U1
U 1 1 5F1642CB
P 3850 5650
F 0 "U1" H 3850 6715 50  0000 C CNN
F 1 "PIC18f25k22" H 3850 6624 50  0000 C CNN
F 2 "pic:PIC18F25K22" H 3850 5150 50  0001 C CNN
F 3 "" H 3850 5150 50  0001 C CNN
	1    3850 5650
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR01
U 1 1 5F15B7C0
P 2450 3850
F 0 "#PWR01" H 2450 3700 50  0001 C CNN
F 1 "+3V3" H 2465 4023 50  0000 C CNN
F 2 "" H 2450 3850 50  0001 C CNN
F 3 "" H 2450 3850 50  0001 C CNN
	1    2450 3850
	1    0    0    -1  
$EndComp
$Comp
L i2c_adaptor:C_Small C1
U 1 1 5F155D21
P 4800 5650
F 0 "C1" H 4708 5604 50  0000 R CNN
F 1 "0.1uF" H 4708 5695 50  0000 R CNN
F 2 "pic:Capacitor" H 4800 5650 50  0001 C CNN
F 3 "" H 4800 5650 50  0001 C CNN
	1    4800 5650
	-1   0    0    1   
$EndComp
Wire Wire Line
	7550 4650 8300 4650
Wire Wire Line
	5250 1800 6700 1800
Wire Wire Line
	6700 1800 6700 3250
Wire Wire Line
	6900 5450 6900 4850
NoConn ~ 3250 5950
Connection ~ 4950 5450
Wire Wire Line
	6350 5200 6350 5250
Wire Wire Line
	6350 5250 4450 5250
Wire Wire Line
	6900 5450 4950 5450
Text Notes 900  7600 0    50   ~ 0
Note: pull up registers must be provided\nby the external I2C module.
Wire Wire Line
	1500 6900 4700 6900
Wire Wire Line
	1500 7000 3050 7000
Wire Wire Line
	2250 6300 2250 7200
$EndSCHEMATC