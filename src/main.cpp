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
// C++ libraries
#include "Arduino.h"
#include "DS3231.h"
#include "ESP8266WiFi.h"
#include "NTPClient.h"
#include "WiFiUdp.h"
#include "btnapi.h"
// Project header
#include "MeteoClock.hpp"
// Project headers
#include "LCDWrapper.hpp"
#include "enc_wrap.hpp"
#include "encoder.hpp"
#include "handler_base.hpp"
#include "rtc_handler.hpp"
#include "stng_handler.hpp"
#ifdef DEBUG
#include "debug.hpp"
#endif
// Objects
/**
 * @brief Number of thongs in pointer table
 * 0 -> current page ptr
 * 1 -> lcd ptr
 * 2 -> rtc ptr
 * 3 -> encoder ptr
 * 4 -> WifiUDP ptr
 * 5 -> NTPClient ptr
 * 6 -> enc change (bitfields):
 *        bit 0 - button click state
 *        bit 1 - encoder rotation state
 */
#define PTR_TABLE_SIZE 7
LCD	  lcd;
DS3231	  rtc;
encapi	  enc(PIN_S1, PIN_S2, PIN_KEY);
Encoder	  enc1(PIN_KEY, PIN_S1, PIN_S2);
WiFiUDP	  udp;
NTPClient ntp(udp, NTP_SERVER, TIMEZONE);
// Global variables
// Handlers data
handler_t handlers[] = {STNG_TABLE_ENTRY, RTC_TABLE_ENTRY};
void	**ptr_table;
// Helper
uint8_t page;
long	last_enc_pos = 0;
// Timers
unsigned long	    prev_gfx = 0;
const unsigned long int_gfx  = 500;
unsigned long	    prev_enc = 0;
const unsigned long int_enc  = 50;
// Setup
void setup() {
// For debugging
#if defined(DEBUG)
  Serial.begin(115200);
#endif
  // init sensors
  // dht.begin();                              // dht11 init
  // bmp.begin();                              // bmp180 init
  enc.setPosition(0); // Center encoder
  // WiFi and NTP
  WiFi.begin(SSID, PASSWORD);
#if defined(DEBUG) && defined(DBG_STATUS)
  Serial.print("Connecting to WiFi...");
#endif
  lcd.writeStr("Connecting to WiFi.", 0, 0);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
#if defined(DEBUG) && defined(DBG_STATUS)
    Serial.print(".");
#endif
    lcd.writeCont(".");
  }
  ntp.begin();
  // init text
  lcd.clear();
  lcd.writeStr("MeteoClock v2.0   WM", 0, 0);
  lcd.writeStr("Encoder: change page", 0, 1);
  lcd.writeStr("Button: change stng", 0, 2);
  lcd.writeStr("Button hold: on/off", 0, 3);
  // Init ptr table
  ptr_table = (void **)malloc(PTR_TABLE_SIZE * sizeof(void *));
  if (ptr_table == NULL) {
#if defined(DEBUG) && defined(DBG_STATUS)
    Serial.begin(115200);
    Serial.println(
	"Error, cannot malloc ptr_table, so, cannot call any handler!\n "
	"Panic!\n Controller wil be halted until reboot.");
#endif
    while (true) {
    }
  }
  *(ptr_table + 0) = (void *)&page;
  *(ptr_table + 1) = (void *)&lcd;
  *(ptr_table + 2) = (void *)&rtc;
  *(ptr_table + 3) = (void *)&enc;
  *(ptr_table + 4) = (void *)&udp;
  *(ptr_table + 5) = (void *)&ntp;
  *(ptr_table + 6) = (void *)NULL;
  // Finishing of init
#if defined(DEBUG) && defined(DBG_STATUS)
  Serial.println("Inited");
#endif
  page = RTC_PAGE;
  for (int i = 0; i < MAX_PAGE; i++) {
    handlers[page].init(&handlers[page], ptr_table);
  }
  // delay(1000 * BOOT_SECONDS); // delay for stability
  lcd.clear();
}
// Loop
void loop() {
#if defined(DEBUG) && defined(DBG_NUM)
  static int m = 0;
  static int h = 0;
  m++;
  if (m >= 100) {
    m = 0;
    h++;
  }
  if (h >= 100) {
    h = 0;
  }
  lcd.writeBigNumber(h, m);
#endif
  // Call draw routine
  unsigned long curr = millis();
  if (curr - prev_gfx >= int_gfx) {
    prev_gfx = curr;
    handlers[page].draw(&handlers[page], ptr_table);
  }
  // enc.tick();
  enc1.tick();
  // long pos   = enc.getPosition();
  // bool click = enc.btnClick();
  // if ((pos != last_enc_pos) || click) {
  //   // State of encode
  //   uint8_t state = 0;
  //   state |= (enc.btnClick() ? 0x01 : 0x00);
  //   state |= ((pos != last_enc_pos) ? 0x02 : 0x00);
  //   *(ptr_table + 6) = (void *)&state;
  //   // Call to ctrl handler
  //   page_ch ret = handlers[page].ctrl(&handlers[page], ptr_table);
  //   // Page change
  //   if (ret == page_ch::NEXT_PAGE) {
  //     page++;
  //     if (page > MAX_PAGE) {
  //       page = 0;
  //     }
  //   } else if (ret == page_ch::PREV_PAGE) {
  //     if (page != 0) {
  //       page--;
  //     } else {
  //       page = MAX_PAGE;
  //     }
  //   } else if (ret == page_ch::DEFAULT_CTRL) {
  //     int rot = enc.getDir();
  //     if (rot == 1) {
  //       page++;
  //       if (page > MAX_PAGE) {
  //         page = 0;
  //       }
  //     } else if (rot == -1) {
  //       if (page != 0) {
  //         page--;
  //       } else {
  //         page = MAX_PAGE;
  //       }
  //     }
  //   }
  //   last_enc_pos = pos;
  // }
#if defined(DEBUG) && defined(DBG_INPUT)
  static unsigned long tmr_dbg_enc = 0;
  if (curr - tmr_dbg_enc >= 100) {
    tmr_dbg_enc = curr;
    enc1.latch();
    Serial.print("Input sampling: ");
    switch (enc1.state()) {
    case encstate::NONE:
      Serial.println("no changes");
      break;
    case encstate::CLICK:
      Serial.println("button click");
      break;
    case encstate::ROTATION: {
      Serial.print("rotation, direction: ");
      int rot = enc1.direction();
      if (rot == 0) {
	Serial.println("no rotation");
      } else if (rot == 1) {
	Serial.println("clockwise");
      } else if (rot == -1) {
	Serial.println("counterclockwise");
      }
    } break;
    case encstate::HOLD_ROTATION: {
      Serial.print("rotation with button holding, direction: ");
      int rot = enc1.direction();
      if (rot == 0) {
	Serial.println("no rotation");
      } else if (rot == 1) {
	Serial.println("clockwise");
      } else if (rot == -1) {
	Serial.println("counterclockwise");
      }
    } break;
    }
  }
#endif
  // btn.tick(); // tick for button
  // static byte screen = 0;
  // if (btn.isClick()) // if click -> screen++ or screen = 0
  // {
  //   Serial.println("Click");
  //   if (screen == MAX_SCREAN) { // if screen value is maximum -> screen
  //   = 0
  //     screen = 0;
  //   } else { // else -> flag = flag + 1
  //     screen++;
  //   }
  //   lcd.clear(); // clear lcd for new information
  // }
  // if (screen == 0)
  //   rtc_check(); // if flag = 0 -> display cool clock
  // if (screen == 1)
  //   dht_check(); // if flag = 1 -> display information from dht
  // if (screen == 2)
  //   bmp_check(); // if flag = 2 -> display information from bmt
  // if (screen == 3)
  //   rtc_check_old(); // if flag = 3 -> display clock data only
  // static bool holded = 0;
  // if (!holded && btn.isHold()) // if button hold -> on/off display
  // {
  //   static bool flagH = 1;
  //   flagH = !flagH;
  //   holded = 1;
  //   if (!flagH) // off display
  //   {
  //     lcd.noDisplay();
  //     lcd.noBacklight();
  //   }
  //   if (flagH) // on display
  //   {
  //     lcd.display();
  //     lcd.backlight();
  //   }
  // }
  // if (btn.state() == 0)
  //   holded = 0;
}
