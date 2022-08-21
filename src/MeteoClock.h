// Meteo Clock - cool clock with meteostation on Arduino
// Copyright (C) 2022 WolodiaM

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// Defines
#define INIT_DEL 10000 // delay for init text visible
#define MAX_SCREAN 3   // screan numbers
#define DHTTYPE DHT11  // dht tuypes
#define NODEBUG        // debug options NODEBUG/CHARTEST/NUMBERTEST
#define TEMPBMP        // NODATA/TEMPDHT/TEMPBMP/HUMDHT/PRESSBMP/SECOND, display in right down corner on main screan
// pins
#define DHTPIN 23 // pin for dht
#define BTNPIN 35 // pin for button
// Libs
#include <SPI.h>               // need for Adafruit bus IO lib (dependency)
#include <Wire.h>              // i2c bus lib (hardvare)
#include <Adafruit_Sensor.h>   // need for dht lib (dependency)
#include <LiquidCrystal_I2C.h> // lib for lcd (output)
#include <RTClib.h>            // lib for rtc (time)
#include <btnapi.h>            // lib for buttons (input)
#include <DHT.h>               // lib for dht (temperature + humidity)
#include <Adafruit_BMP085.h>   // lib for bmp085/180 (temperature + pressure)
// Objects
LiquidCrystal_I2C lcd(0x27, 20, 4);       // lcd object
DHT dht(DHTPIN, DHTTYPE);                 // dht object
RTC_DS3231 rtc;                           // rtc object
Adafruit_BMP085 bmp;                      // bmp object
btnapi btn(BTNPIN, LOW_PULL, NORM_CLOSE); // btn object
DateTime now;                             // time object
// Vars
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; // days names
char daysOfTheWeekShort[7][3] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};                          // short version of day names
volatile int counter = 0;
volatile bool dotCounter = 0;
// Files
#include "charset.h"
#include "numbers.h"
numbers nmb;
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