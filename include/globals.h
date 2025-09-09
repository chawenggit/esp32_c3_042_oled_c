
#pragma once
#include <stdint.h>
#include <stdbool.h>

/*
            BOARD ESP32-C3 0.42 OLED
                    (ESP32-C3)
                LEFT   TOP VIEW     RIGHT
                |1	+5V 			|1	GPIO10          | NA
                |2	GND				|2	GPIO9	I2C_SCL | SW-RH 
    	        |3	+3V3			|3	GPIO8	I2C_SDA | nBLUE LED
    NA|    RXD0 |4	GPIO20			|4	GPIO7	        | NA
    NA|    TXD0 |5	GPIO21			|5	GPIO6	MOSI    |OLED_SCL  
    NA|     A2  |6	GPIO2       	|6	GPIO5   MISO,A5 |OLED_SDA 
    NA|     A1  |7	GPIO1			|7	GPIO4        A4 | NA
    NA|     A0  |8	GPIO0			|8	GPIO3        A3 | NA
                    (USP-C on BOTTOM)
                    
//Pin interfaces: 1xI2C, 1xSPI, 2xUART, 11xGPIO (PWM), 4xADC

SW
    RIGHT SW pullup  GPO9 pin
    LEFT SW pullup  RST pin
LED
    LED blue light: GPO8 pin
USB
    USBP = GPIO19
    USBN = GPIO18
UART:2
    UART 0 :  
    UART 1 : 
RMT:1 - 2Tx, 2Rx
I2C:1
SPI:3    
    ONLY SPI2 is general
*/



#define TX0_P GPIO_NUM_21
#define RX0_P GPIO_NUM_20

#define LED_P GPIO_NUM_8

#define LCD_SCL_P GPIO_NUM_6
#define LCD_SDA_P GPIO_NUM_5

#define SW_P GPIO_NUM_9

#define MainLoopT 20 // 20x1mS *** subject to change
#define T20mS 1
#define T40mS 2 * T20mS
#define T60mS 3 * T20mS
#define T80mS 4 * T20mS
#define T100mS 5 * T20mS
#define T1S 10 * T100mS
#define T2S 2 * T1S
#define T3S 3 * T1S


#define TaskStack1K 1024
#define TaskStack2K (TaskStack1K * 2)
#define TaskStack3K (TaskStack1K * 3)
#define TaskStack4K (TaskStack1K * 4)
#define TaskStack8K (TaskStack1K * 8)
#define Priority1 1
#define Priority2 2
#define Priority3 3
#define Priority4 4
#define Priority5 5
#define Core0 0
#define Core1 1

typedef struct
{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    bool synced_PC;
} EMU_RTC_T;

typedef struct
{
    uint16_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} UPTIME_T;

#define millis_() (unsigned long)(esp_timer_get_time() / 1000ULL)
