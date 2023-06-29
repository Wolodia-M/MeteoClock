/**
 * @file handlers.cpp
 * @author WolodiaM (w_melnyk@outlook.com)
 * @brief Implement different handlers
 * @version 1.0
 * @date 2023-06-13
 *
 * @copyright (C) 2023 WolodiaM
 * @license GPL v3.0 or later
 *
 * Copyright (C) 2022  WolodiaM
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
// C++ libraries
#include "DS3231.h"
// Project includes
#include "LCDWrapper.hpp"
#include "MeteoClock.hpp"
#include "NTPClient.h"
#include "enc_wrap.hpp"
#include "rtc_handler.hpp"
#pragma region RTC
/* rtc_handler.hpp data */
namespace rtc_handler {
bool charset_loaded = false;
int  getMonthNumber(String monthStr) {
  if (monthStr == "Jan")
    return 1;
  else if (monthStr == "Feb")
    return 2;
  else if (monthStr == "Mar")
    return 3;
  else if (monthStr == "Apr")
    return 4;
  else if (monthStr == "May")
    return 5;
  else if (monthStr == "Jun")
    return 6;
  else if (monthStr == "Jul")
    return 7;
  else if (monthStr == "Aug")
    return 8;
  else if (monthStr == "Sep")
    return 9;
  else if (monthStr == "Oct")
    return 10;
  else if (monthStr == "Nov")
    return 11;
  else if (monthStr == "Dec")
    return 12;
  else
    return 0;
}
char	daysOfTheWeekShort[7][4] = {"Mon", "Tue", "Wed", "Thu",
				    "Fri", "Sat", "Sun"};
bool	dot			 = true;
uint8_t prevMin			 = -1;
} // namespace rtc_handler
/* rtc_handler.hpp */
void rtc_init(handler_t *handler, void **args) {
  GET_RTC_OBJ(rtc);
  rtc->init();
  const char *t = __TIME__;
  int	      h, mn, s;
  sscanf(t, "%d:%d:%d", &h, &mn, &s);
  String ds = __DATE__;
  int	 d  = ds.substring(4, 6).toInt();
  String ms = ds.substring(0, 3);
  int	 mo = rtc_handler::getMonthNumber(ms);
  int	 y  = ds.substring(7).toInt();
  rtc->setSec(s);
  rtc->setMin(mn);
  rtc->setHour(h);
  rtc->setDate(d);
  rtc->setMonth(mo);
  rtc->setYear(y);
}
void rtc_draw(handler_t *handler, void **args) {
  GET_RTC_OBJ(rtc);
  GET_LCD_OBJ(lcd);
  if (!rtc_handler::charset_loaded) {
    lcd->loadCharset(charset::NUMBERS);
  }
  // static char daysOfTheWeek[7][12] = {"Sunday",    "Monday", "Tuesday",
  //                                     "Wednesday", "Thursday",
  //                                     "Friday", "Saturday"};
  lcd->disableCursor();
  uint8_t currMin = rtc->getMin();
  // Print time and date
  if ((currMin != rtc_handler::prevMin) || !rtc_handler::charset_loaded) {
    lcd->clear();
    lcd->writeBigNumber(rtc->getHour(), currMin);
    lcd->writeStr(String(rtc_handler::daysOfTheWeekShort[rtc->getDay()]), 16,
		  0);
    lcd->writeNum(rtc->getMonth(), 15, 1);
    lcd->writeCont(".");
    lcd->writeCont(rtc->getDate());
    lcd->writeNum(rtc->getYear(), 15, 2);
    rtc_handler::prevMin	= currMin;
    rtc_handler::charset_loaded = true;
  }
  // Print dots
  lcd->writeBigDots(rtc_handler::dot);
  rtc_handler::dot = !rtc_handler::dot;
}
page_ch rtc_ctrl(handler_t *handler, void **args) {
  GET_ENC_OBJ(enc);
  int rot = enc->getDir();
  if (rot != 0) {
    rtc_handler::charset_loaded = false;
  }
  if (rot == 1) {
    return page_ch::NEXT_PAGE;
  } else if (rot == -1) {
    return page_ch::PREV_PAGE;
  }
  return page_ch::NO_CHANGE;
}
#pragma endregion RTC
#pragma region	  STNG
namespace stng_handler {
bool charset_loaded = false;
bool redraw_req	    = true;
enum page { NTP = 0, CLK_MANUAL = 1, DATE_MANUAL = 2 };
stng_handler::page curr_page = NTP;
int		   cursor    = 0;
void		   set_ntp_time(DS3231 *rtc, NTPClient *ntp) {
	ntp->update();
	time_t	   rawtime = ntp->getEpochTime();
	struct tm *ti;
	ti = localtime(&rawtime);
	rtc->setSec(ti->tm_sec);
	rtc->setMin(ti->tm_min);
	rtc->setHour(ti->tm_hour);
	rtc->setDate(ti->tm_mday);
	rtc->setMonth(ti->tm_mon);
	rtc->setYear(ti->tm_year);
	Serial.println("Time");
}
} // namespace stng_handler
void stng_init(handler_t *handler, void **args) {}
void stng_draw(handler_t *handler, void **args) {
  GET_LCD_OBJ(lcd);
  if (!stng_handler::charset_loaded) {
    lcd->loadCharset(charset::STNG_ICONS);
    stng_handler::redraw_req	 = true;
    stng_handler::charset_loaded = true;
  }
  if (stng_handler::redraw_req) {
    lcd->clear();
    lcd->disableCursor();
    lcd->writeStr("MeteoClock " + VER_STR, 2, 0);
    lcd->writeSym(STNG::chars::COG, 18, 0);
    switch (stng_handler::curr_page) {
    case stng_handler::page::NTP:
      lcd->writeStr("Set NTP time:", 0, 1);
      lcd->writeSym(STNG::WIFI, 1, 2);
      lcd->writeStr("apply", 3, 2);
      if (stng_handler::cursor == 1) {
	lcd->setCursor(18, 0);
      } else if (stng_handler::cursor == 2) {
	lcd->setCursor(1, 2);
      }
      break;
    case stng_handler::page::CLK_MANUAL:
    case stng_handler::page::DATE_MANUAL:
    default:
      break;
    }
    stng_handler::redraw_req = false;
  }
}
page_ch stng_ctrl(handler_t *handler, void **args) {
  GET_ENC_OBJ(enc);
  GET_STATE_VAR(state);
  int rot		   = enc->getDir();
  stng_handler::redraw_req = true;
  // Some other UI
  if (((state & 0x02) > 0) && digitalRead(PIN_KEY) == LOW) {
    // Hold rotation
    switch (stng_handler::curr_page) {
    case stng_handler::page::NTP:
      if (enc->getDir() == 1) {
	stng_handler::cursor++;
	if (stng_handler::cursor > 2) {
	  stng_handler::cursor = 0;
	}
      } else if (enc->getDir() == -1) {
	if (stng_handler::cursor == 1) {
	  stng_handler::cursor = 2;
	} else {
	  stng_handler::cursor--;
	}
      }
      break;
    case stng_handler::page::CLK_MANUAL:
    case stng_handler::page::DATE_MANUAL:
    default:
      break;
    }
  } else if ((state & 0x01) > 0) {
    // Button pressed
    switch (stng_handler::curr_page) {
    case stng_handler::page::NTP:
      if (stng_handler::cursor == 2) {
	GET_RTC_OBJ(rtc);
	GET_NTP_OBJ(ntp);
	stng_handler::set_ntp_time(rtc, ntp);
      }
      break;
    case stng_handler::page::CLK_MANUAL:
    case stng_handler::page::DATE_MANUAL:
    default:
      break;
    }
  } else {
    if (rot != 0) {
      stng_handler::charset_loaded = false;
      stng_handler::cursor	   = 0;
      stng_handler::curr_page	   = stng_handler::page::NTP;
    }
    if (rot == 1) {
      return page_ch::NEXT_PAGE;
    } else if (rot == -1) {
      return page_ch::PREV_PAGE;
    }
  }
  return page_ch::NO_CHANGE;
}
#pragma endregion STNG
