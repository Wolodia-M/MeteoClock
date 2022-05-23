// Defines
#define INIT_DEL 10000 // delay for init text visible
#define MAX_SCREAN 3   // screan numbers
#define DHTTYPE DHT11  // dht tuypes
#define NODEBUG        // debug options NODEBUG/CHARTEST/NUMBERTEST
#define TEMPBMP        // NODATA/TEMPDHT/TEMPBMP/HUMDHT/PRESSBMP/SECOND, display in right down corner on main screan
#define MILLIS         // HARDVARETIMER/MILLIS,  hardvare not work
// pins
#define DHTPIN 2 // pin for dht
#define BTNPIN 3 // pin for button
// pins for ds1302
#define RST_C 9 // pin for rst/ce/cs pin from clock
#define CLK_C 7 // pin for clk/sck/scl from clock
#define DAT_C 8 // pin for dat/io/sda from clock
// Libs
#include <SPI.h>               // need for Adafruit bus IO lib
#include <Wire.h>              // i2c bus lib
#include <Adafruit_Sensor.h>   // need for dht lib
#include <LiquidCrystal_I2C.h> // lib for lcd
#include <RTClib.h>            // lib for rtcq
#include <btnapi.h>            // lib for buttons
#include <DHT.h>               // lib for dht (temperature + humidity)
#include <Adafruit_BMP085.h>   // lib for bmp085/180 (temperature + pressure)
#ifdef HARDVARETIMER
#include <TimerOne.h> // Harvare timer1 lib
#endif
// Objects
LiquidCrystal_I2C lcd(0x27, 20, 4);      // lcd object
DHT dht(DHTPIN, DHTTYPE);                // dht object
DS1302 rtc;                              // rtc object
Adafruit_BMP085 bmp;                     // bmp object
btnapi btn(BTNPIN, LOW_PULL, NORM_OPEN); // btn object
DateTime now;                            // time object
// Vars
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; // days names
char daysOfTheWeekShort[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};                         // short version of day names
volatile int counter = 0;
volatile bool dotCounter = 0;
// Files
#include "charset.h"
#include "numbers.h"
#include "lcd.h"
#include "rtc_func.h"
#include "dht_func.h"
#include "bmp_func.h"
#ifndef NODEBUG
#include "debug.h"
#endif

// platformIO.ini
/*
    libs
    wolodia-m/btnapi@^1.0.0
    adafruit/RTClib@^2.0.3
    adafruit/DHT sensor library@^1.4.3
    adafruit/Adafruit BMP085 Library@^1.2.1
    marcoschwartz/LiquidCrystal_I2C@^1.1.4
*/