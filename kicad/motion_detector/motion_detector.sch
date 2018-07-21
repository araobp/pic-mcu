EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:pic16f1-mcu
LIBS:motion_detector-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Motion detector"
Date "2018-07-21"
Rev "v0.1"
Comp "https://github.com/araobp"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L pic16f18326 U3
U 1 1 5B531F88
P 5000 3050
F 0 "U3" H 4450 3500 60  0000 C CNN
F 1 "pic16f18326" H 5000 3050 60  0000 C CNN
F 2 "" H 4600 3100 60  0001 C CNN
F 3 "" H 4600 3100 60  0001 C CNN
	1    5000 3050
	1    0    0    -1  
$EndComp
$Comp
L AE-FT234X U2
U 1 1 5B531FAF
P 2500 6450
F 0 "U2" H 2100 6800 60  0000 C CNN
F 1 "AE-FT234X" H 2550 6500 60  0000 C CNN
F 2 "" H 2350 6450 60  0001 C CNN
F 3 "" H 2350 6450 60  0001 C CNN
	1    2500 6450
	1    0    0    -1  
$EndComp
$Comp
L tactile_sw U4
U 1 1 5B532037
P 5300 6200
F 0 "U4" H 5100 6400 60  0000 C CNN
F 1 "tactile_sw" H 5300 6200 60  0000 C CNN
F 2 "" H 5250 6150 60  0001 C CNN
F 3 "" H 5250 6150 60  0001 C CNN
	1    5300 6200
	1    0    0    -1  
$EndComp
$Comp
L EEPROM U5
U 1 1 5B532054
P 8550 1800
F 0 "U5" H 8250 2100 60  0000 C CNN
F 1 "EEPROM" H 8550 1800 60  0000 C CNN
F 2 "" H 8450 1800 60  0001 C CNN
F 3 "" H 8450 1800 60  0001 C CNN
	1    8550 1800
	1    0    0    -1  
$EndComp
$Comp
L MPU9250 U1
U 1 1 5B532081
P 1500 1900
F 0 "U1" H 1150 2500 60  0000 C CNN
F 1 "MPU9250" H 1300 2000 60  0000 C CNN
F 2 "" H 1600 2050 60  0001 C CNN
F 3 "" H 1600 2050 60  0001 C CNN
	1    1500 1900
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR6
U 1 1 5B5321A8
P 3650 1100
F 0 "#PWR6" H 3650 950 50  0001 C CNN
F 1 "+5V" H 3650 1240 50  0000 C CNN
F 2 "" H 3650 1100 50  0001 C CNN
F 3 "" H 3650 1100 50  0001 C CNN
	1    3650 1100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR8
U 1 1 5B532208
P 6550 7150
F 0 "#PWR8" H 6550 6900 50  0001 C CNN
F 1 "GND" H 6550 7000 50  0000 C CNN
F 2 "" H 6550 7150 50  0001 C CNN
F 3 "" H 6550 7150 50  0001 C CNN
	1    6550 7150
	1    0    0    -1  
$EndComp
$Comp
L SW_Push SW1
U 1 1 5B532359
P 3700 3800
F 0 "SW1" H 3750 3900 50  0000 L CNN
F 1 "SW_Push" H 3700 3740 50  0000 C CNN
F 2 "" H 3700 4000 50  0001 C CNN
F 3 "" H 3700 4000 50  0001 C CNN
	1    3700 3800
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 5B532460
P 3350 4200
F 0 "R1" V 3430 4200 50  0000 C CNN
F 1 "10k" V 3350 4200 50  0000 C CNN
F 2 "" V 3280 4200 50  0001 C CNN
F 3 "" H 3350 4200 50  0001 C CNN
	1    3350 4200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR5
U 1 1 5B5324D1
P 3350 5200
F 0 "#PWR5" H 3350 4950 50  0001 C CNN
F 1 "GND" H 3350 5050 50  0000 C CNN
F 2 "" H 3350 5200 50  0001 C CNN
F 3 "" H 3350 5200 50  0001 C CNN
	1    3350 5200
	1    0    0    -1  
$EndComp
$Comp
L LED D1
U 1 1 5B533E19
P 4150 3850
F 0 "D1" H 4150 3950 50  0000 C CNN
F 1 "LED" H 4150 3750 50  0000 C CNN
F 2 "" H 4150 3850 50  0001 C CNN
F 3 "" H 4150 3850 50  0001 C CNN
	1    4150 3850
	0    -1   -1   0   
$EndComp
$Comp
L R R2
U 1 1 5B533F63
P 4150 4250
F 0 "R2" V 4230 4250 50  0000 C CNN
F 1 "1k" V 4150 4250 50  0000 C CNN
F 2 "" V 4080 4250 50  0001 C CNN
F 3 "" H 4150 4250 50  0001 C CNN
	1    4150 4250
	1    0    0    -1  
$EndComp
NoConn ~ 2100 1850
NoConn ~ 2100 1950
NoConn ~ 2100 2050
NoConn ~ 2100 2150
NoConn ~ 2100 2250
NoConn ~ 2100 2350
$Comp
L C_Small 0.1pF1
U 1 1 5B534BA1
P 5000 2450
F 0 "0.1pF1" H 5010 2520 50  0000 L CNN
F 1 "C_Small" H 5010 2370 50  0000 L CNN
F 2 "" H 5000 2450 50  0001 C CNN
F 3 "" H 5000 2450 50  0001 C CNN
	1    5000 2450
	0    1    1    0   
$EndComp
$Comp
L +5V #PWR12
U 1 1 5B534D44
P 9300 1100
F 0 "#PWR12" H 9300 950 50  0001 C CNN
F 1 "+5V" H 9300 1240 50  0000 C CNN
F 2 "" H 9300 1100 50  0001 C CNN
F 3 "" H 9300 1100 50  0001 C CNN
	1    9300 1100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR10
U 1 1 5B534DDB
P 7750 2700
F 0 "#PWR10" H 7750 2450 50  0001 C CNN
F 1 "GND" H 7750 2550 50  0000 C CNN
F 2 "" H 7750 2700 50  0001 C CNN
F 3 "" H 7750 2700 50  0001 C CNN
	1    7750 2700
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 5B534E85
P 7600 2150
F 0 "R4" V 7680 2150 50  0000 C CNN
F 1 "10k" V 7600 2150 50  0000 C CNN
F 2 "" V 7530 2150 50  0001 C CNN
F 3 "" H 7600 2150 50  0001 C CNN
	1    7600 2150
	1    0    0    -1  
$EndComp
NoConn ~ 5800 3350
NoConn ~ 5800 2950
NoConn ~ 5800 2850
$Comp
L C_Small 0.1pF2
U 1 1 5B5364AE
P 5300 6550
F 0 "0.1pF2" H 5310 6620 50  0000 L CNN
F 1 "C_Small" H 5310 6470 50  0000 L CNN
F 2 "" H 5300 6550 50  0001 C CNN
F 3 "" H 5300 6550 50  0001 C CNN
	1    5300 6550
	0    1    1    0   
$EndComp
$Comp
L +5V #PWR7
U 1 1 5B53674E
P 6000 1100
F 0 "#PWR7" H 6000 950 50  0001 C CNN
F 1 "+5V" H 6000 1240 50  0000 C CNN
F 2 "" H 6000 1100 50  0001 C CNN
F 3 "" H 6000 1100 50  0001 C CNN
	1    6000 1100
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 5B539CE2
P 6200 6650
F 0 "R3" V 6280 6650 50  0000 C CNN
F 1 "10k" V 6200 6650 50  0000 C CNN
F 2 "" V 6130 6650 50  0001 C CNN
F 3 "" H 6200 6650 50  0001 C CNN
	1    6200 6650
	0    1    1    0   
$EndComp
$Comp
L GND #PWR13
U 1 1 5B53576D
P 9450 5300
F 0 "#PWR13" H 9450 5050 50  0001 C CNN
F 1 "GND" H 9450 5150 50  0000 C CNN
F 2 "" H 9450 5300 50  0001 C CNN
F 3 "" H 9450 5300 50  0001 C CNN
	1    9450 5300
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR11
U 1 1 5B53573D
P 8450 4050
F 0 "#PWR11" H 8450 3900 50  0001 C CNN
F 1 "+5V" H 8450 4190 50  0000 C CNN
F 2 "" H 8450 4050 50  0001 C CNN
F 3 "" H 8450 4050 50  0001 C CNN
	1    8450 4050
	1    0    0    -1  
$EndComp
$Comp
L character_lcd U6
U 1 1 5B531FDA
P 8750 5000
F 0 "U6" H 8350 4750 60  0000 C CNN
F 1 "character_lcd" H 8750 4900 60  0000 C CNN
F 2 "" H 8700 5000 60  0001 C CNN
F 3 "" H 8700 5000 60  0001 C CNN
	1    8750 5000
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR1
U 1 1 5B53CC71
P 1500 5600
F 0 "#PWR1" H 1500 5450 50  0001 C CNN
F 1 "+5V" H 1500 5740 50  0000 C CNN
F 2 "" H 1500 5600 50  0001 C CNN
F 3 "" H 1500 5600 50  0001 C CNN
	1    1500 5600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR2
U 1 1 5B53CCA5
P 1500 7400
F 0 "#PWR2" H 1500 7150 50  0001 C CNN
F 1 "GND" H 1500 7250 50  0000 C CNN
F 2 "" H 1500 7400 50  0001 C CNN
F 3 "" H 1500 7400 50  0001 C CNN
	1    1500 7400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR9
U 1 1 5B53CE48
P 6600 3750
F 0 "#PWR9" H 6600 3500 50  0001 C CNN
F 1 "GND" H 6600 3600 50  0000 C CNN
F 2 "" H 6600 3750 50  0001 C CNN
F 3 "" H 6600 3750 50  0001 C CNN
	1    6600 3750
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG1
U 1 1 5B53D53D
P 1800 5750
F 0 "#FLG1" H 1800 5825 50  0001 C CNN
F 1 "PWR_FLAG" H 1800 5900 50  0000 C CNN
F 2 "" H 1800 5750 50  0001 C CNN
F 3 "" H 1800 5750 50  0001 C CNN
	1    1800 5750
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG2
U 1 1 5B53D571
P 1800 6900
F 0 "#FLG2" H 1800 6975 50  0001 C CNN
F 1 "PWR_FLAG" H 1800 7050 50  0000 C CNN
F 2 "" H 1800 6900 50  0001 C CNN
F 3 "" H 1800 6900 50  0001 C CNN
	1    1800 6900
	1    0    0    -1  
$EndComp
Text Label 2800 1600 0    60   ~ 0
3.3V
Text Label 2800 1900 0    60   ~ 0
3.3V
$Comp
L GND #PWR4
U 1 1 5B53DA88
P 2450 2650
F 0 "#PWR4" H 2450 2400 50  0001 C CNN
F 1 "GND" H 2450 2500 50  0000 C CNN
F 2 "" H 2450 2650 50  0001 C CNN
F 3 "" H 2450 2650 50  0001 C CNN
	1    2450 2650
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR3
U 1 1 5B53DE56
P 2450 1100
F 0 "#PWR3" H 2450 950 50  0001 C CNN
F 1 "+5V" H 2450 1240 50  0000 C CNN
F 2 "" H 2450 1100 50  0001 C CNN
F 3 "" H 2450 1100 50  0001 C CNN
	1    2450 1100
	1    0    0    -1  
$EndComp
Text Notes 8050 5500 0    60   ~ 0
I2C pull up inside LCD
Text Notes 750  1150 0    60   ~ 0
I2C pull up inside the module
Text Notes 4100 1900 0    60   ~ 0
My original PIC evaluation board
NoConn ~ 9150 1750
NoConn ~ 2450 6450
NoConn ~ 5700 6100
$Comp
L R R5
U 1 1 5B53AB68
P 4000 2350
F 0 "R5" V 4080 2350 50  0000 C CNN
F 1 "51k" V 4000 2350 50  0000 C CNN
F 2 "" V 3930 2350 50  0001 C CNN
F 3 "" H 4000 2350 50  0001 C CNN
	1    4000 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 6650 6550 7150
Wire Wire Line
	4000 3800 3900 3800
Wire Wire Line
	4000 2500 4000 3050
Wire Wire Line
	4000 3050 4000 3800
Wire Wire Line
	4000 3050 4200 3050
Wire Wire Line
	3350 4350 3350 4550
Wire Wire Line
	3350 4550 3350 5200
Wire Wire Line
	3650 1100 3650 2050
Wire Wire Line
	3650 2050 3650 2750
Wire Wire Line
	3650 2750 4150 2750
Wire Wire Line
	4150 2750 4200 2750
Wire Wire Line
	5800 2750 5850 2750
Wire Wire Line
	5850 2750 6600 2750
Wire Wire Line
	2100 1550 2450 1550
Wire Wire Line
	4200 3350 4150 3350
Wire Wire Line
	4150 3350 4150 3700
Wire Wire Line
	4150 4000 4150 4100
Wire Wire Line
	4150 4400 4150 4550
Wire Wire Line
	4150 4550 3350 4550
Connection ~ 3350 4550
Wire Wire Line
	5800 3150 8700 3150
Wire Wire Line
	8700 3150 9500 3150
Wire Wire Line
	5800 3250 8800 3250
Wire Wire Line
	8800 3250 9400 3250
Wire Wire Line
	2100 1650 3500 1650
Wire Wire Line
	3500 1650 3500 2850
Wire Wire Line
	3500 2850 4200 2850
Wire Wire Line
	2100 1750 3350 1750
Wire Wire Line
	3350 1750 3350 2950
Wire Wire Line
	3350 2950 4200 2950
Wire Wire Line
	4900 2450 4150 2450
Wire Wire Line
	4150 2450 4150 2750
Connection ~ 4150 2750
Wire Wire Line
	5100 2450 5850 2450
Wire Wire Line
	5850 2450 5850 2750
Connection ~ 5850 2750
Wire Wire Line
	9150 1650 9300 1650
Wire Wire Line
	9300 1650 9300 1100
Wire Wire Line
	7950 1950 7750 1950
Wire Wire Line
	7750 1950 7750 2500
Wire Wire Line
	7750 2500 7750 2700
Wire Wire Line
	7600 1850 7950 1850
Wire Wire Line
	7600 1650 7600 1750
Wire Wire Line
	7600 1750 7600 1850
Wire Wire Line
	7600 1850 7600 2000
Wire Wire Line
	7950 1750 7600 1750
Connection ~ 7600 1850
Wire Wire Line
	7950 1650 7600 1650
Connection ~ 7600 1750
Wire Wire Line
	7600 2300 7600 2500
Wire Wire Line
	7600 2500 7750 2500
Connection ~ 7750 2500
Wire Wire Line
	6000 3050 5800 3050
Wire Wire Line
	6000 1100 6000 3050
Wire Wire Line
	6000 3050 6000 5450
Wire Wire Line
	5850 6650 6050 6650
Wire Wire Line
	6350 6650 6550 6650
Wire Wire Line
	6000 5450 4850 5450
Wire Wire Line
	4850 5450 4850 6100
Wire Wire Line
	4850 6100 4900 6100
Connection ~ 6000 3050
Wire Wire Line
	4900 6300 4850 6300
Wire Wire Line
	4850 6300 4850 6550
Wire Wire Line
	4850 6550 5200 6550
Wire Wire Line
	5400 6550 5850 6550
Connection ~ 5850 6550
Connection ~ 8700 3150
Wire Wire Line
	8900 4450 8900 4650
Wire Wire Line
	9450 4450 8900 4450
Wire Wire Line
	9450 5300 9450 4450
Wire Wire Line
	8600 4450 8600 4650
Wire Wire Line
	8450 4450 8600 4450
Wire Wire Line
	8450 4050 8450 4450
Wire Wire Line
	8800 3250 8800 4650
Wire Wire Line
	8700 4650 8700 3150
Connection ~ 8800 3250
Wire Wire Line
	9150 1850 9500 1850
Wire Wire Line
	9500 1850 9500 3150
Wire Wire Line
	9150 1950 9400 1950
Wire Wire Line
	9400 1950 9400 3250
Wire Wire Line
	3350 4050 3350 3800
Wire Wire Line
	3350 3800 3500 3800
Wire Wire Line
	4200 3150 1150 3150
Wire Wire Line
	1150 3150 1150 6450
Wire Wire Line
	1150 6450 1800 6450
Wire Wire Line
	4200 3250 1050 3250
Wire Wire Line
	1050 3250 1050 6550
Wire Wire Line
	1050 6550 1800 6550
Wire Wire Line
	1500 6250 1800 6250
Wire Wire Line
	1500 5600 1500 6000
Wire Wire Line
	1500 6000 1500 6250
Wire Wire Line
	1800 6350 1500 6350
Wire Wire Line
	1500 6350 1500 7100
Wire Wire Line
	1500 7100 1500 7400
Wire Wire Line
	6600 2750 6600 3750
Wire Wire Line
	1800 5750 1800 6000
Wire Wire Line
	1800 6000 1500 6000
Connection ~ 1500 6000
Wire Wire Line
	1800 6900 1800 7100
Wire Wire Line
	1800 7100 1500 7100
Connection ~ 1500 7100
Wire Wire Line
	2450 1550 2450 2650
Wire Wire Line
	2100 1450 2450 1450
Wire Wire Line
	2450 1450 2450 1100
Wire Notes Line
	3150 1950 3150 4650
Wire Notes Line
	3150 4650 6250 4650
Wire Notes Line
	6250 4650 6250 1950
Wire Notes Line
	6250 1950 3150 1950
Wire Wire Line
	5700 6300 5850 6300
Connection ~ 5850 6300
Wire Wire Line
	5850 6300 5850 6550
Wire Wire Line
	5850 6550 5850 6650
Connection ~ 4000 3050
Wire Wire Line
	3650 2050 4000 2050
Wire Wire Line
	4000 2050 4000 2200
Connection ~ 3650 2050
$EndSCHEMATC
