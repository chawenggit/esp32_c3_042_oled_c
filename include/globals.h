
#pragma once
#include <stdint.h>
#include <stdbool.h>
// #include "driver/gpio.h"
// GPIO
//*** CAN NOT USE GPIO6-11 since it connected to Flash

// const gpio_num_t BUG33_P = GPIO_NUM_25;
// const gpio_num_t BUG18_P = GPIO_NUM_18;
// const gpio_num_t BUG19_P = GPIO_NUM_19;

// const gpio_num_t TX1_P = GPIO_NUM_4;
// const gpio_num_t RX1_P = GPIO_NUM_15;
// const gpio_num_t I2C_SDA_P = GPIO_NUM_32;
// const gpio_num_t I2C_SCL_P = GPIO_NUM_33;

// const gpio_num_t LED_P = GPIO_NUM_2; // Espino=16, TTGO=12,IOX=5
// const byte ADC_EN = 14;
// const byte BUTTON_1 = 35;
// const byte BUTTON_2 = 0;
// const byte ADC_PIN = 34;
// const byte LED_PIN = 2;
// extern uint8_t groupcnt; // bugweng
// extern uint groupcnt0; // bugweng
// extern Adafruit_SSD1306 display;

// extern const char* ssid;
// extern const char* password;
/*
BOARD WT32_SC01_Plus
    ---- EXT CONN ---
    |1|  +5V
    |2|  GND
    |3|  GPIO10 = MISO
    |4|  GPIO11 = MOSI
    |5|  GPIO12 = CS
    |6|  GPIO13 = SCK
    |7|  GPIO14 = INT
    |8|  GPIO21 = RST
    ----- DEBUG PORT -----
    |1|  +5V
    |2|  +3V3
    |3|  TX0 = IO43
    |4|  RX0 = IO44
    |5|  EN
    |6|  GPIO0
    |7|  GND
    ----- U4 NS4168 CLASS D AMPLIFIER ----- = HC12
    P1 = GND
    P2 GPIO35 (TX2) = RXD (HC12)
    P3 GPIO36 (RX2) = TXD (HC12)
    P3 GPIO37       = SET (HC12)

    ----- SD CARD -----
    SD_CS   IO41
    SD_MOSI IO40
    SD_CLK  IO39
    SD_MISO IO38

*/
// ESP32    Debug  Wired to LCD   EXT CONN   SDCARD W5500 HC12
// -------- ------  -------------- ---------- ------ ----- ----
// GPIO10           -              P3 EXT_IO1 -      MISO
// GPIO11           -              P4 EXT_IO2 -      MOSI
// GPIO12           -              P5 EXT_IO3 -      SCS
// GPIO13           -              P6 EXT_IO4 -      SCLK
// GPIO14           -              P7 EXT_IO5 -      INT
// GPIO21           -              P8 EXT_IO6 -      RST 

// GPIO0    P6 BOOT -                                       CAN NOT USE SINCE LCD USE IT AS RS   
// GPI43    P3 TX1  -                                       RXD
// GPI44    P4 RX1  -                                       TXD
//---------------------------------------------------------------
// ---------------------BELOW UNKNOWN ----------
// GPIO35           -               -         -      -      
// GPIO36           -               -         -      -      
// GPIO37           -               -         -      -      
// GPIO17           -               -                     -       RX of NEXTION (if in use)

// GPIO1    TXD0    -               -                     -       Reserved serial output
// GPI03    RXD0    -               -                     -       Reserved serial input
// GPIO39   INP     -               -                     -       Rotary encoder SW
// GPIO36   INP     -               -                     -       Rotary encoder SW
// GPIO15           pin 2 CS        -                     -       -
// GPIO14            -               pin 4 DREQ            -       -
// GPIO12   -       -               -                     -       Rotary encoder DT
// GPIO35   INP     -               -                     -       Rotary encoder SW

// GPIO4            pin 3 D/C or A0 -                     -       -
// GPIO2            pin 3 D/C or A0 -                     -       -
// -------  ------  --------------- -------------------  ------   ----------------
// GND      -       pin 8 GND       pin 8 GND                     Power supply GND
// VCC 3 V  -       pin 7 BL        -                             Power supply
// VCC 5 V  -       pin 6 VCC       pin 9 5V                      Power supply
// EN       -       pin 1 RST       pin 3 XRST                    -

// Serial(1+3)  TX(D-)  RX(D+)
//   0          GPIO19  GPIO20    USB
//   1          GPIO43  GPIO44    U0  DEBUG CON HC12
//   2          


// W5500 pins

// #define ETH_SPI_MISO 10
// #define ETH_SPI_MOSI 11
// #define ETH_PHY_CS   12
// #define ETH_SPI_SCK  13
// #define ETH_PHY_IRQ  14
// #define ETH_PHY_RST  21

#define TX0_P GPIO_NUM_21
#define RX0_P GPIO_NUM_20
#define LED_P GPIO_NUM_8
#define LCD_SCL_P GPIO_NUM_6
#define LCD_SDA_P GPIO_NUM_5
#define SW_P GPIO_NUM_9




#define TaskStack1K 1024
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
