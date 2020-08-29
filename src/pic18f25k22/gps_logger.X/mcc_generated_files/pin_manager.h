/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.4
        Device            :  PIC18F25K22
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.20 and above
        MPLAB 	          :  MPLAB X 5.40	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set FET_TW aliases
#define FET_TW_TRIS                 TRISBbits.TRISB4
#define FET_TW_LAT                  LATBbits.LATB4
#define FET_TW_PORT                 PORTBbits.RB4
#define FET_TW_WPU                  WPUBbits.WPUB4
#define FET_TW_ANS                  ANSELBbits.ANSB4
#define FET_TW_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define FET_TW_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define FET_TW_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define FET_TW_GetValue()           PORTBbits.RB4
#define FET_TW_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define FET_TW_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define FET_TW_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define FET_TW_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define FET_TW_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define FET_TW_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set FET_GPS aliases
#define FET_GPS_TRIS                 TRISBbits.TRISB5
#define FET_GPS_LAT                  LATBbits.LATB5
#define FET_GPS_PORT                 PORTBbits.RB5
#define FET_GPS_WPU                  WPUBbits.WPUB5
#define FET_GPS_ANS                  ANSELBbits.ANSB5
#define FET_GPS_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define FET_GPS_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define FET_GPS_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define FET_GPS_GetValue()           PORTBbits.RB5
#define FET_GPS_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define FET_GPS_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define FET_GPS_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define FET_GPS_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define FET_GPS_SetAnalogMode()      do { ANSELBbits.ANSB5 = 1; } while(0)
#define FET_GPS_SetDigitalMode()     do { ANSELBbits.ANSB5 = 0; } while(0)

// get/set RB6 procedures
#define RB6_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define RB6_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define RB6_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define RB6_GetValue()              PORTBbits.RB6
#define RB6_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define RB6_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define RB6_SetPullup()             do { WPUBbits.WPUB6 = 1; } while(0)
#define RB6_ResetPullup()           do { WPUBbits.WPUB6 = 0; } while(0)

// get/set RB7 procedures
#define RB7_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define RB7_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define RB7_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define RB7_GetValue()              PORTBbits.RB7
#define RB7_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define RB7_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define RB7_SetPullup()             do { WPUBbits.WPUB7 = 1; } while(0)
#define RB7_ResetPullup()           do { WPUBbits.WPUB7 = 0; } while(0)

// get/set LED aliases
#define LED_TRIS                 TRISCbits.TRISC0
#define LED_LAT                  LATCbits.LATC0
#define LED_PORT                 PORTCbits.RC0
#define LED_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define LED_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define LED_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define LED_GetValue()           PORTCbits.RC0
#define LED_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define LED_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)

// get/set RC3 procedures
#define RC3_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define RC3_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define RC3_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define RC3_GetValue()              PORTCbits.RC3
#define RC3_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define RC3_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define RC3_SetAnalogMode()         do { ANSELCbits.ANSC3 = 1; } while(0)
#define RC3_SetDigitalMode()        do { ANSELCbits.ANSC3 = 0; } while(0)

// get/set RC4 procedures
#define RC4_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define RC4_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define RC4_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define RC4_GetValue()              PORTCbits.RC4
#define RC4_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define RC4_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define RC4_SetAnalogMode()         do { ANSELCbits.ANSC4 = 1; } while(0)
#define RC4_SetDigitalMode()        do { ANSELCbits.ANSC4 = 0; } while(0)

// get/set TW_M3 aliases
#define TW_M3_TRIS                 TRISCbits.TRISC5
#define TW_M3_LAT                  LATCbits.LATC5
#define TW_M3_PORT                 PORTCbits.RC5
#define TW_M3_ANS                  ANSELCbits.ANSC5
#define TW_M3_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define TW_M3_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define TW_M3_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define TW_M3_GetValue()           PORTCbits.RC5
#define TW_M3_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define TW_M3_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define TW_M3_SetAnalogMode()      do { ANSELCbits.ANSC5 = 1; } while(0)
#define TW_M3_SetDigitalMode()     do { ANSELCbits.ANSC5 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()              PORTCbits.RC6
#define RC6_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define RC6_SetAnalogMode()         do { ANSELCbits.ANSC6 = 1; } while(0)
#define RC6_SetDigitalMode()        do { ANSELCbits.ANSC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()              PORTCbits.RC7
#define RC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define RC7_SetAnalogMode()         do { ANSELCbits.ANSC7 = 1; } while(0)
#define RC7_SetDigitalMode()        do { ANSELCbits.ANSC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/