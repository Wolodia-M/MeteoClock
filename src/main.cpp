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
#include "DHTesp.h"
#include "DS3231.h"
#include "ESP8266WiFi.h"
#include "NTPClient.h"
#include "SFE_BMP180.h"
#include "WiFiUdp.h"
#include "btnapi.h"
// Project header
#include "MeteoClock.hpp"
// Project headers
#include "LCDWrapper.hpp"
#include "bmp_handler.hpp"
#include "dht_handler.hpp"
#include "encoder.hpp"
#include "handler_base.hpp"
#include "rtc_handler.hpp"
#include "stng_handler.hpp"
#ifdef DEBUG
#include "debug.hpp"
#endif
// Global variables
// Objects
/**
 * @brief Number of thongs in pointer table
 * 0 -> current page ptr
 * 1 -> lcd ptr
 * 2 -> rtc ptr
 * 3 -> encoder ptr
 * 4 -> WifiUDP ptr
 * 5 -> NTPClient ptr
 * 6 -> bmp ptr
 * 7 -> btn ptr
 * 8 -> dht ptr
 * 9 -> settings struct ptr
 */
#define PTR_TABLE_SIZE 10
LCD	   lcd;
DS3231	   rtc;
Encoder	   enc(PIN_KEY, PIN_S1, PIN_S2);
WiFiUDP	   udp;
NTPClient  ntp(udp, NTP_SERVER);
SFE_BMP180 bmp;
btnapi	   btn(PIN_BTN, LOW_PULL, NORM_OPEN);
DHTesp	   dht;
// Settings
settings_t settings;
// Handlers data
handler_t handlers[] = {STNG_TABLE_ENTRY, RTC_TABLE_ENTRY, BMP_TABLE_ENTRY,
			DHT_TABLE_ENTRY};
void	**ptr_table;
// Helper
uint8_t page;
long	last_enc_pos   = 0;
bool	display_enable = true;
// Timers
unsigned long	    prev_gfx   = 0;
const unsigned long int_gfx    = 500;
unsigned long	    prev_cntrl = 0;
const unsigned long int_cntrl  = 50;
// Setup
void setup() {
// For debugging
#if defined(DEBUG)
  Serial.begin(115200);
#endif
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
  ntp.setTimeOffset(TIME_OFFSET * 3600);
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
  *(ptr_table + 6) = (void *)&bmp;
  *(ptr_table + 7) = (void *)&btn;
  *(ptr_table + 8) = (void *)&dht;
  *(ptr_table + 9) = (void *)&settings;
  // Finishing of init
#if defined(DEBUG) && defined(DBG_STATUS)
  Serial.println("Inited");
#endif
  page = RTC_PAGE;
  for (int i = 0; i <= MAX_PAGE; i++) {
    handlers[i].init(&handlers[i], ptr_table);
  }
  // Setup settings
#if defined(NO_AMERICAN_DATE_FORMAT)
  settings.date_format = settings_t::date_format_t::DMY;
#elif defined(AMERICAN_DATE_FORMAT)
  settings.date_format	= settings_t::date_format_t::MDY;
#else
  settings.date_format	= settings_t::date_format_t::DMY;
#endif
  settings.date_divisor = settings_t::date_divisor_t::DOT;
#if defined(TIME_OFFSET)
  settings.utc_offset	  = TIME_OFFSET;
  settings.utc_offset_min = 0;
#endif
#if defined(BMP_PRESS_SIDE)
  settings.side_display = settings_t::side_display_t::BMP_PRESS;
#elif defined(BMP_TEMP_SIDE)
  settings.side_display = settings_t::side_display_t::BMP_TEMP;
#elif defined(DHT_TEMP_SIDE)
  settings.side_display = settings_t::side_display_t::DHT_TEMP;
#elif defined(DHT_HUM_SIDE)
  settings.side_display = settings_t::side_display_t::DHT_HUM;
#elif defined(DHT_HI_SIDE)
  settings.side_display = settings_t::side_display_t::DHT_HI;
#elif defined(RTC_SEC_SIDE)
  settings.side_display = settings_t::side_display_t::RTC_SEC;
#elif defined(NONE)
  settings.side_display = settings_t::side_display_t::NONE;
#else
  settings.side_display = settings_t::side_display_t::RTC_SEC;
#endif
  // Delay
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
    if (display_enable) {
      handlers[page].draw(&handlers[page], ptr_table);
    }
  }
  // Update encoder
  enc.tick();
  // Procces button
  btn.tick();
  if (btn.isClick()) {
    display_enable = !display_enable;
    lcd.toogle();
  }
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
  if (curr - prev_cntrl >= int_cntrl) {
    prev_cntrl = curr;
    enc.latch();
    page_ch pch = handlers[page].ctrl(&handlers[page], ptr_table);
    switch (pch) {
    case page_ch::NEXT_PAGE:
      page++;
      if (page > MAX_PAGE) {
	page = 0;
      }
      break;
    case page_ch::PREV_PAGE:
      if (page == 0) {
	page = MAX_PAGE;
      } else {
	page--;
      }
      break;
    case page_ch::DEFAULT_CTRL:
      if (enc.state() == encstate::ROTATION) {
	if (enc.direction() == 1) {
	  page++;
	  if (page > MAX_PAGE) {
	    page = 0;
	  }
	} else if (enc.direction() == -1) {
	  if (page == 0) {
	    page = MAX_PAGE;
	  } else {
	    page--;
	  }
	}
      }
      break;
    case page_ch::NO_CHANGE:
      [[fallthrough]];
    default:
      break;
    }
#if defined(DEBUG) && defined(DBG_INPUT)
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
#endif
  }
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
