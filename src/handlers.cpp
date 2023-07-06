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
#include "DHTesp.h"
#include "DS3231.h"
#include "NTPClient.h"
#include "SFE_BMP180.h"
// Project includes
#include "LCDWrapper.hpp"
#include "MeteoClock.hpp"
#include "bmp_handler.hpp"
#include "dht_handler.hpp"
#include "encoder.hpp"
#include "handler_base.hpp"
#include "rtc_handler.hpp"
// Defines
#define PAGE_CH_ROUTINE(page)                                                  \
  stng_handler::curr_page	= page;                                        \
  stng_handler::cursor		= 0;                                           \
  stng_handler::custom_rotation = false;
#define DATA_CLR_ROUTINE                                                       \
  stng_handler::data[0] = 0;                                                   \
  stng_handler::data[1] = 0;                                                   \
  stng_handler::data[2] = 0;                                                   \
  stng_handler::data[3] = 0;                                                   \
  stng_handler::data[4] = 0;                                                   \
  stng_handler::data[5] = 0;                                                   \
  stng_handler::data[6] = 0;                                                   \
  stng_handler::data[7] = 0;
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
char	daysOfTheWeekShort[7][4] = {"Sun", "Mon", "Tue", "Wed",
				    "Thu", "Fri", "Sat"};
bool	dot			 = true;
uint8_t prevMin			 = -1;
#if defined(DHT_TEMP_SIDE) || defined(DHT_HUM_SIDE) || defined(DHT_HI_SIDE)
uint8_t max;
uint8_t tmr = 0;
float	disp;
#endif
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
    GET_STNG_VAR(stng);
    lcd->clear();
    lcd->writeBigNumber(rtc->getHour(), currMin);
    lcd->writeStr(String(rtc_handler::daysOfTheWeekShort[rtc->getDay()]), 16,
		  0);
    String divisor;
    if (stng->date_divisor == settings_t::date_divisor_t::DOT) {
      divisor.concat('.');
    } else if (stng->date_divisor == settings_t::date_divisor_t::DASH) {
      divisor.concat('-');
    } else if (stng->date_divisor == settings_t::date_divisor_t::SLASH) {
      divisor.concat('/');
    }
    if (stng->date_format == settings_t::date_format_t::MDY) {
      lcd->writeNum(rtc->getMonth(), 15, 1);
      lcd->writeCont(divisor);
      lcd->writeCont(rtc->getDate());
      lcd->writeStr(divisor, 15, 2);
      lcd->writeNum(rtc->getYear(), 16, 2);
    } else if (stng->date_format == settings_t::date_format_t::DMY) {
      lcd->writeNum(rtc->getDate(), 15, 1);
      lcd->writeCont(divisor);
      lcd->writeCont(rtc->getMonth());
      lcd->writeStr(divisor, 15, 2);
      lcd->writeNum(rtc->getYear(), 16, 2);
    } else if (stng->date_format == settings_t::date_format_t::YMD) {
      lcd->writeStr(divisor, 15, 1);
      lcd->writeNum(rtc->getYear(), 16, 1);
      lcd->writeNum(rtc->getMonth(), 15, 2);
      lcd->writeCont(divisor);
      lcd->writeCont(rtc->getDate());
    }
    rtc_handler::prevMin	= currMin;
    rtc_handler::charset_loaded = true;
  }
  // Down-right corner
  GET_STNG_VAR(stng);
  lcd->writeStr("     ", 15, 3);
  if ((stng->side_display == settings_t::side_display_t::BMP_PRESS) ||
      (stng->side_display == settings_t::side_display_t::BMP_TEMP)) {
    GET_BMP_OBJ(bmp);
    // Measurments vars
    char   status;
    double temp, press;
    // Reading temperature
    status = bmp->startTemperature();
    if (status == 0) {
#if defined(DEBUG) && defined(DBG_STATUS)
      Serial.print(
	  "Error while starting temperature measurments from bmp, errno: ");
      Serial.println(bmp->getError());
#endif
      return;
    }
    delay(status);
    status = bmp->getTemperature(temp);
    if (status == 0) {
#if defined(DEBUG) && defined(DBG_STATUS)
      Serial.print(
	  "Error while reading temperature measurments from bmp, errno: ");
      Serial.println(bmp->getError());
#endif
      return;
    }
    // Reading pressure
    status = bmp->startPressure(3);
    if (status == 0) {
#if defined(DEBUG) && defined(DBG_STATUS)
      Serial.print(
	  "Error while starting pressure measurments from bmp, errno: ");
      Serial.println(bmp->getError());
#endif
      return;
    }
    delay(status);
    status = bmp->getPressure(press, temp);
    if (status == 0) {
#if defined(DEBUG) && defined(DBG_STATUS)
      Serial.print(
	  "Error while reading pressure measurments from bmp, errno: ");
      Serial.println(bmp->getError());
#endif
      return;
    }
    // Conversion and output
    float fT, fP;
    fT = temp;
    fP = press * 0.750061683;
    if (stng->side_display == settings_t::side_display_t::BMP_PRESS) {
      lcd->writeNum(fP, 15, 3, (uint8_t)1);
    } else if (stng->side_display == settings_t::side_display_t::BMP_TEMP) {
      lcd->writeNum(fT, 15, 3, (uint8_t)1);
    }
  } else if (stng->side_display == settings_t::side_display_t::RTC_SEC) {
    lcd->writeNum(rtc->getSec(), 16, 3);
  } else if ((stng->side_display == settings_t::side_display_t::DHT_TEMP) ||
	     (stng->side_display == settings_t::side_display_t::DHT_HUM) ||
	     (stng->side_display == settings_t::side_display_t::DHT_HI)) {
    GET_DHT_OBJ(dht);
    if (rtc_handler::tmr > rtc_handler::max) {
      rtc_handler::tmr = 0;
      if (stng->side_display == settings_t::side_display_t::DHT_TEMP) {
	rtc_handler::disp = dht->getTemperature();
      } else if (stng->side_display == settings_t::side_display_t::DHT_HUM) {
	rtc_handler::disp = dht->getHumidity();
      } else if (stng->side_display == settings_t::side_display_t::DHT_HI) {
	rtc_handler::disp = dht->computeHeatIndex(dht->getTemperature(),
						  dht->getHumidity(), false);
      }
    }
    lcd->writeNum(rtc_handler::disp, 15, 3, (uint8_t)1);
    rtc_handler::tmr++;
  }
  // Print dots
  lcd->writeBigDots(rtc_handler::dot);
  rtc_handler::dot = !rtc_handler::dot;
}
page_ch rtc_ctrl(handler_t *handler, void **args) {
  GET_ENC_OBJ(enc);
  if (enc->state() != encstate::NONE) {
    rtc_handler::charset_loaded = false;
  }
  return page_ch::DEFAULT_CTRL;
}
#pragma endregion RTC
#pragma region	  STNG
/* stng_handler.hpp data */
namespace stng_handler {
bool	charset_loaded	= false;
bool	redraw_req	= true;
bool	custom_rotation = false;
uint8_t data[8]; // Generick data needed for some stng pages
enum page {
  /**
   * @brief NTP apply settings, data not used
   */
  NTP = 0,
  /**
   * @brief Manual time setup,
   *  - data[0]: Edit flag
   *  - data[1]: Hours
   *  - data[2]: Minutes
   *  - data[3]: Seconds
   */
  CLK_MANUAL = 1,
  /**
   * @brief Manual date setup,
   *  - data[0]: Edit flag
   *  - data[1]: Day
   *  - data[2]: Month
   *  - data[3]: Year
   */
  DATE_MANUAL = 2,
  /**
   * @brief Date formatting
   */
  DATE_FORMAT = 3,
  /**
   * @brief What will be displayed on lower-right corner of RTC page
   */
  RTC_PAGE_SIDE = 4,
  /**
   * @brief Setup UTC offset for NTP time loading
   *  - data[0]: Edit flag
   *  - data[1]: utc time offset hours, as `signed char`
   *  - data[2]: utc time offset minutes, as `signed char`
   */
  NTP_OFFSET = 5,
  /**
   * @brief Save/Load settings
   *  - data[0]: Fail flag for loading
   *  - data[1]: Fail flag for saving
   */
  SETTINGS_SAVE = 6
};
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
	rtc->setMonth(ti->tm_mon + 1);
	rtc->setYear(ti->tm_year);
}
} // namespace stng_handler
/* stng_handler.hpp */
void stng_init(handler_t *handler, void **args) {}
void stng_draw(handler_t *handler, void **args) {
  GET_LCD_OBJ(lcd);
  // Preload charset if it is not loaded, also, force redraw if we reload
  // charset
  if (!stng_handler::charset_loaded) {
    // Charset loading
    lcd->loadCharset(charset::STNG_ICONS);
    stng_handler::redraw_req	 = true;
    stng_handler::charset_loaded = true;
  }
  if (stng_handler::redraw_req) {
    // Start from the fresh lcd setup
    lcd->clear();
    lcd->disableCursor();
    // Basic setup
    lcd->writeStr("MeteoClock " + VER_STR, 2, 0);
    lcd->writeSym(STNG::chars::COG, 18, 0);
    switch (stng_handler::curr_page) {
      // NTP settings
    case stng_handler::page::NTP:
      lcd->writeStr("Set NTP time:", 0, 1);
      lcd->writeSym(STNG::WIFI, 1, 2);
      lcd->writeStr("apply", 3, 2);
      // Cog
      if (stng_handler::cursor == 1) {
	lcd->setCursor(18, 0);
	// NTP apply
      } else if (stng_handler::cursor == 2) {
	lcd->setCursor(1, 2);
      }
      break;
    case stng_handler::page::CLK_MANUAL:
      lcd->writeStr("Set time:", 0, 1);
      lcd->writeNum(stng_handler::data[1], 1, 2, 2); // 1, 2 to 2, 2
      lcd->writeCont(":");			     // 3, 2
      lcd->writeCont(stng_handler::data[2], 2);	     // 4, 2 to 5, 2
      lcd->writeCont(":");			     // 6, 2
      lcd->writeCont(stng_handler::data[3], 2);	     // 7, 2 to 8, 2
      lcd->writeSym(STNG::OK, 3, 3);
      lcd->writeCont(" apply");
      if (stng_handler::data[0] > 0) {
	lcd->writeStr("em", 18, 3);
      } else {
	lcd->writeStr("  ", 18, 3);
      }
      // Cog
      if (stng_handler::cursor == 1) {
	lcd->setCursor(18, 0);
	// Hours
      } else if (stng_handler::cursor == 2) {
	lcd->setCursor(1, 2);
	// Minutes
      } else if (stng_handler::cursor == 3) {
	lcd->setCursor(4, 2);
	// Seconds
      } else if (stng_handler::cursor == 4) {
	lcd->setCursor(7, 2);
	// Apply
      } else if (stng_handler::cursor == 5) {
	lcd->setCursor(3, 3);
      }
      break;
    case stng_handler::page::DATE_MANUAL:
      lcd->writeStr("Set date:", 0, 1);
      lcd->writeNum(stng_handler::data[1], 1, 2, 2);
      lcd->writeCont(".");
      lcd->writeCont(stng_handler::data[2], 2);
      lcd->writeCont(".");
      lcd->writeCont(stng_handler::data[3], 2);
      lcd->writeSym(STNG::OK, 3, 3);
      lcd->writeCont(" apply");
      if (stng_handler::data[0] > 0) {
	lcd->writeStr("em", 18, 3);
      } else {
	lcd->writeStr("  ", 18, 3);
      }
      // Cog
      if (stng_handler::cursor == 1) {
	lcd->setCursor(18, 0);
	// Day
      } else if (stng_handler::cursor == 2) {
	lcd->setCursor(1, 2);
	// Month
      } else if (stng_handler::cursor == 3) {
	lcd->setCursor(4, 2);
	// Year
      } else if (stng_handler::cursor == 4) {
	lcd->setCursor(7, 2);
	// Apply
      } else if (stng_handler::cursor == 5) {
	lcd->setCursor(3, 3);
      }
      break;
    case stng_handler::page::DATE_FORMAT:
      lcd->writeStr("Date formatting:", 0, 1);
      lcd->writeStr("Format:", 1, 2);
      lcd->writeStr("Delimeter: \'", 1, 3);
      lcd->writeStr("\'", 14, 3);
      {
	GET_STNG_VAR(stng);
	if (stng->date_format == settings_t::date_format_t::MDY) {
	  lcd->writeStr("MDY", 9, 2);
	} else if (stng->date_format == settings_t::date_format_t::DMY) {
	  lcd->writeStr("DMY", 9, 2);
	} else if (stng->date_format == settings_t::date_format_t::YMD) {
	  lcd->writeStr("YMD", 9, 2);
	}
	if (stng->date_divisor == settings_t::date_divisor_t::DOT) {
	  lcd->writeStr(".", 13, 3);
	} else if (stng->date_divisor == settings_t::date_divisor_t::DASH) {
	  lcd->writeStr("-", 13, 3);
	} else if (stng->date_divisor == settings_t::date_divisor_t::SLASH) {
	  lcd->writeStr("/", 13, 3);
	}
      }
      // Cog
      if (stng_handler::cursor == 1) {
	lcd->setCursor(18, 0);
	// Date formatting
      } else if (stng_handler::cursor == 2) {
	lcd->setCursor(9, 2);
	// Date delimeter
      } else if (stng_handler::cursor == 3) {
	lcd->setCursor(13, 3);
      }
      break;
    case stng_handler::page::RTC_PAGE_SIDE:
      lcd->writeStr("RTC side-display: ", 0, 1);
      lcd->writeStr("Mode: ", 1, 2);
      {
	GET_STNG_VAR(stng);
	if (stng->side_display == settings_t::side_display_t::RTC_SEC) {
	  lcd->writeStr("RTC seconds", 7, 2);
	} else if (stng->side_display ==
		   settings_t::side_display_t::BMP_PRESS) {
	  lcd->writeStr("BMP pressure", 7, 2);
	} else if (stng->side_display == settings_t::side_display_t::BMP_TEMP) {
	  lcd->writeStr("BMP temp", 7, 2);
	} else if (stng->side_display == settings_t::side_display_t::DHT_HUM) {
	  lcd->writeStr("DHT humidity", 7, 2);
	} else if (stng->side_display == settings_t::side_display_t::DHT_TEMP) {
	  lcd->writeStr("DHT temp", 7, 2);
	} else if (stng->side_display == settings_t::side_display_t::DHT_HI) {
	  lcd->writeStr("DHT heat idx", 7, 2);
	} else if (stng->side_display == settings_t::side_display_t::NONE) {
	  lcd->writeStr("None", 7, 2);
	}
      }
      // Cog
      if (stng_handler::cursor == 1) {
	lcd->setCursor(18, 0);
	// Mode
      } else if (stng_handler::cursor == 2) {
	lcd->setCursor(7, 2);
      }
      break;
    case stng_handler::page::NTP_OFFSET:
      lcd->writeStr("UTC time offset: ", 0, 1);
      lcd->writeStr("H:", 1, 2);
      lcd->writeStr("M:", 9, 2);
      lcd->writeSym(STNG::OK, 3, 3);
      lcd->writeCont(" apply");
      if (stng_handler::data[0] > 0) {
	lcd->writeStr("em", 18, 3);
      } else {
	lcd->writeStr("  ", 18, 3);
      }
      {
	signed char *h =
	    reinterpret_cast<signed char *>(stng_handler::data + 1);
	lcd->writeNum(*h, 4, 2);
	lcd->writeNum(stng_handler::data[2], 12, 2);
      }
      // Cog
      if (stng_handler::cursor == 1) {
	lcd->setCursor(18, 0);
	// UTC offset hours
      } else if (stng_handler::cursor == 2) {
	lcd->setCursor(4, 2);
	// UTC offset minutes
      } else if (stng_handler::cursor == 3) {
	lcd->setCursor(12, 2);
	// Apply
      } else if (stng_handler::cursor == 4) {
	lcd->setCursor(3, 3);
      }
      break;
    case stng_handler::page::SETTINGS_SAVE:
      lcd->writeStr("Settings backup:", 0, 1);
      lcd->writeSym(STNG::LOAD, 4, 2);
      lcd->writeSym(STNG::SAVE, 15, 2);
      lcd->writeStr("Factory reset", 1, 3);
      if (stng_handler::data[0] == 1) {
	lcd->writeStr("f", 3, 2);
      } else {
	lcd->writeStr(" ", 3, 2);
      }
      if (stng_handler::data[1] == 1) {
	lcd->writeStr("f", 16, 2);
      } else {
	lcd->writeStr(" ", 16, 2);
      }
      // Cog
      if (stng_handler::cursor == 1) {
	lcd->setCursor(18, 0);
	// Load button
      } else if (stng_handler::cursor == 2) {
	lcd->setCursor(4, 2);
	// Save button
      } else if (stng_handler::cursor == 3) {
	lcd->setCursor(15, 2);
	// Factory reset button
      } else if (stng_handler::cursor == 4) {
	lcd->setCursor(1, 3);
      }
      break;
    default:
      break;
    }
    stng_handler::redraw_req = false;
  }
}
page_ch stng_ctrl(handler_t *handler, void **args) {
  GET_ENC_OBJ(enc);
  if (enc->state() == encstate::NONE) {
    return page_ch::NO_CHANGE;
  }
  stng_handler::redraw_req = true;
  // Some other UI
  if (enc->state() == encstate::HOLD_ROTATION) {
    // Hold rotation, cursor change
    switch (stng_handler::curr_page) {
    case stng_handler::page::NTP:
      if (enc->direction() == 1) {
	stng_handler::cursor++;
	if (stng_handler::cursor > 2) {
	  stng_handler::cursor = 0;
	}
      } else if (enc->direction() == -1) {
	if (stng_handler::cursor == 1) {
	  stng_handler::cursor = 2;
	} else {
	  stng_handler::cursor--;
	}
      }
      break;
    case stng_handler::page::CLK_MANUAL:
      stng_handler::data[0]	    = 0;
      stng_handler::custom_rotation = false;
      if (enc->direction() == 1) {
	stng_handler::cursor++;
	if (stng_handler::cursor > 5) {
	  stng_handler::cursor = 0;
	}
      } else if (enc->direction() == -1) {
	if (stng_handler::cursor == 1) {
	  stng_handler::cursor = 5;
	} else {
	  stng_handler::cursor--;
	}
      }
      break;
    case stng_handler::page::DATE_MANUAL:
      stng_handler::data[0]	    = 0;
      stng_handler::custom_rotation = false;
      if (enc->direction() == 1) {
	stng_handler::cursor++;
	if (stng_handler::cursor > 5) {
	  stng_handler::cursor = 0;
	}
      } else if (enc->direction() == -1) {
	if (stng_handler::cursor == 1) {
	  stng_handler::cursor = 5;
	} else {
	  stng_handler::cursor--;
	}
      }
      break;
    case stng_handler::page::DATE_FORMAT:
      if (enc->direction() == 1) {
	stng_handler::cursor++;
	if (stng_handler::cursor > 3) {
	  stng_handler::cursor = 0;
	}
      } else if (enc->direction() == -1) {
	if (stng_handler::cursor == 1) {
	  stng_handler::cursor = 3;
	} else {
	  stng_handler::cursor--;
	}
      }
      break;
    case stng_handler::page::RTC_PAGE_SIDE:
      if (enc->direction() == 1) {
	stng_handler::cursor++;
	if (stng_handler::cursor > 2) {
	  stng_handler::cursor = 0;
	}
      } else if (enc->direction() == -1) {
	if (stng_handler::cursor == 1) {
	  stng_handler::cursor = 2;
	} else {
	  stng_handler::cursor--;
	}
      }
      break;
    case stng_handler::page::NTP_OFFSET:
      stng_handler::data[0]	    = 0;
      stng_handler::custom_rotation = false;
      if (enc->direction() == 1) {
	stng_handler::cursor++;
	if (stng_handler::cursor > 4) {
	  stng_handler::cursor = 0;
	}
      } else if (enc->direction() == -1) {
	if (stng_handler::cursor == 1) {
	  stng_handler::cursor = 4;
	} else {
	  stng_handler::cursor--;
	}
      }
      break;
    case stng_handler::page::SETTINGS_SAVE:
      if (enc->direction() == 1) {
	stng_handler::cursor++;
	if (stng_handler::cursor > 4) {
	  stng_handler::cursor = 0;
	}
      } else if (enc->direction() == -1) {
	if (stng_handler::cursor == 1) {
	  stng_handler::cursor = 4;
	} else {
	  stng_handler::cursor--;
	}
      }
      break;
    default:
      break;
    }
  } else if (enc->state() == encstate::CLICK) {
    // Button pressed
    switch (stng_handler::curr_page) {
    case stng_handler::page::NTP:
      // Page change button (cog)
      if (stng_handler::cursor == 1) {
	PAGE_CH_ROUTINE(stng_handler::page::CLK_MANUAL);
	// Data setup for next routine
	GET_RTC_OBJ(rtc);
	stng_handler::data[1] = rtc->getHour();
	stng_handler::data[2] = rtc->getMin();
	stng_handler::data[3] = rtc->getSec();
	// Apply NTP time button
      } else if (stng_handler::cursor == 2) {
	GET_RTC_OBJ(rtc);
	GET_NTP_OBJ(ntp);
	stng_handler::set_ntp_time(rtc, ntp);
      }
      break;
    case stng_handler::page::CLK_MANUAL:
      // Page change button (cog)
      if (stng_handler::data[0] == 0) {
	if (stng_handler::cursor == 1) {
	  PAGE_CH_ROUTINE(stng_handler::page::DATE_MANUAL);
	  DATA_CLR_ROUTINE;
	  // Data setup for next routine
	  GET_RTC_OBJ(rtc);
	  stng_handler::data[1] = rtc->getDay();
	  stng_handler::data[2] = rtc->getMonth();
	  uint16_t year		= rtc->getYear();
	  stng_handler::data[3] = year % 100;
	  // Time
	} else if (stng_handler::cursor == 2) {
	  stng_handler::custom_rotation = true;
	  stng_handler::data[0]		= 1;
	} else if (stng_handler::cursor == 3) {
	  stng_handler::custom_rotation = true;
	  stng_handler::data[0]		= 2;
	} else if (stng_handler::cursor == 4) {
	  stng_handler::custom_rotation = true;
	  stng_handler::data[0]		= 3;
	  // Apply
	} else if (stng_handler::cursor == 5) {
	  GET_RTC_OBJ(rtc);
	  rtc->setHour(stng_handler::data[1]);
	  rtc->setMin(stng_handler::data[2]);
	  rtc->setSec(stng_handler::data[3]);
	}
      } else {
	stng_handler::data[0]	      = 0;
	stng_handler::custom_rotation = false;
      }
      break;
    case stng_handler::page::DATE_MANUAL:
      if (stng_handler::data[0] == 0) {
	// Page change button (cog)
	if (stng_handler::cursor == 1) {
	  PAGE_CH_ROUTINE(stng_handler::page::DATE_FORMAT);
	  DATA_CLR_ROUTINE;
	  // Date
	} else if (stng_handler::cursor == 2) {
	  stng_handler::custom_rotation = true;
	  stng_handler::data[0]		= 1;
	} else if (stng_handler::cursor == 3) {
	  stng_handler::custom_rotation = true;
	  stng_handler::data[0]		= 2;
	} else if (stng_handler::cursor == 4) {
	  stng_handler::custom_rotation = true;
	  stng_handler::data[0]		= 3;
	  // Apply
	} else if (stng_handler::cursor == 5) {
	  GET_RTC_OBJ(rtc);
	  rtc->setDate(stng_handler::data[1]);
	  rtc->setMonth(stng_handler::data[2]);
	  rtc->setYear(stng_handler::data[3]);
	}
      } else {
	stng_handler::data[0]	      = 0;
	stng_handler::custom_rotation = false;
      }
      break;
    case stng_handler::page::DATE_FORMAT:
      // Page change button (cog)
      if (stng_handler::cursor == 1) {
	PAGE_CH_ROUTINE(stng_handler::page::RTC_PAGE_SIDE);
	DATA_CLR_ROUTINE;
	// Date format
      } else if (stng_handler::cursor == 2) {
	GET_STNG_VAR(stng);
	if (stng->date_format == settings_t::date_format_t::MDY) {
	  stng->date_format = settings_t::date_format_t::DMY;
	} else if (stng->date_format == settings_t::date_format_t::DMY) {
	  stng->date_format = settings_t::date_format_t::YMD;
	} else if (stng->date_format == settings_t::date_format_t::YMD) {
	  stng->date_format = settings_t::date_format_t::MDY;
	}
	// Date delimeter
      } else if (stng_handler::cursor == 3) {
	GET_STNG_VAR(stng);
	if (stng->date_divisor == settings_t::date_divisor_t::DOT) {
	  stng->date_divisor = settings_t::date_divisor_t::DASH;
	} else if (stng->date_divisor == settings_t::date_divisor_t::DASH) {
	  stng->date_divisor = settings_t::date_divisor_t::SLASH;
	} else if (stng->date_divisor == settings_t::date_divisor_t::SLASH) {
	  stng->date_divisor = settings_t::date_divisor_t::DOT;
	}
      }
      break;
    case stng_handler::page::RTC_PAGE_SIDE:
      // Page change button (cog)
      if (stng_handler::cursor == 1) {
	PAGE_CH_ROUTINE(stng_handler::page::NTP_OFFSET);
	// Data setup for next routine
	DATA_CLR_ROUTINE;
	GET_STNG_VAR(stng);
	stng_handler::data[1] = stng->utc_offset;
	stng_handler::data[2] = stng->utc_offset_min;
	// Date format
      } else if (stng_handler::cursor == 2) {
	GET_STNG_VAR(stng);
	if (stng->side_display == settings_t::side_display_t::RTC_SEC) {
	  stng->side_display = settings_t::side_display_t::BMP_PRESS;
	} else if (stng->side_display ==
		   settings_t::side_display_t::BMP_PRESS) {
	  stng->side_display = settings_t::side_display_t::BMP_TEMP;
	} else if (stng->side_display == settings_t::side_display_t::BMP_TEMP) {
	  stng->side_display = settings_t::side_display_t::DHT_HUM;
	} else if (stng->side_display == settings_t::side_display_t::DHT_HUM) {
	  stng->side_display = settings_t::side_display_t::DHT_TEMP;
	} else if (stng->side_display == settings_t::side_display_t::DHT_TEMP) {
	  stng->side_display = settings_t::side_display_t::DHT_HI;
	} else if (stng->side_display == settings_t::side_display_t::DHT_HI) {
	  stng->side_display = settings_t::side_display_t::NONE;
	} else if (stng->side_display == settings_t::side_display_t::NONE) {
	  stng->side_display = settings_t::side_display_t::RTC_SEC;
	}
      }
      break;
    case stng_handler::page::NTP_OFFSET:
      if (stng_handler::data[0] == 0) {
	// Page change button (cog)
	if (stng_handler::cursor == 1) {
	  PAGE_CH_ROUTINE(stng_handler::page::SETTINGS_SAVE);
	  DATA_CLR_ROUTINE;
	  // UTC time offset hours
	} else if (stng_handler::cursor == 2) {
	  stng_handler::data[0]		= 1;
	  stng_handler::custom_rotation = true;
	  // UTC time offset minutes
	} else if (stng_handler::cursor == 3) {
	  stng_handler::data[0]		= 2;
	  stng_handler::custom_rotation = true;
	  // Apply button
	} else if (stng_handler::cursor == 4) {
	  GET_STNG_VAR(stng);
	  GET_NTP_OBJ(ntp);
	  signed char *h =
	      reinterpret_cast<signed char *>(stng_handler::data + 1);
	  stng->utc_offset     = *h;
	  stng->utc_offset_min = stng_handler::data[2];
	  if (stng->utc_offset >= 0) {
	    ntp->setTimeOffset(((int)stng->utc_offset * 3600) +
			       ((int)stng->utc_offset_min * 60));
	  } else {
	    ntp->setTimeOffset(((int)stng->utc_offset * 3600) -
			       ((int)stng->utc_offset_min * 60));
	  }
	}
      } else {
	stng_handler::data[0]	      = 0;
	stng_handler::custom_rotation = false;
      }
      break;
    case stng_handler::page::SETTINGS_SAVE:
      // Page change button (cog)
      if (stng_handler::cursor == 1) {
	PAGE_CH_ROUTINE(stng_handler::page::NTP);
	DATA_CLR_ROUTINE;
	// Load button
      } else if (stng_handler::cursor == 2) {
	GET_STNG_VAR(stng);
	if (stng->load()) {
	  stng_handler::data[0] = 0;
	} else {
	  stng_handler::data[0] = 1;
	}
	// Save button
      } else if (stng_handler::cursor == 3) {
	GET_STNG_VAR(stng);
	if (stng->save()) {
	  stng_handler::data[1] = 0;
	} else {
	  stng_handler::data[1] = 1;
	}
	// Factory reset button
      } else if (stng_handler::cursor == 4) {
	ESP.restart();
      }
      break;
    default:
      break;
    }
    // Near-standard screen-change logic, also some simple reset of variables
  } else if (enc->state() == encstate::ROTATION) {
    if (stng_handler::custom_rotation == false) {
      if (enc->direction() != 0) {
	stng_handler::charset_loaded  = false;
	stng_handler::cursor	      = 0;
	stng_handler::curr_page	      = stng_handler::page::NTP;
	stng_handler::custom_rotation = false;
	DATA_CLR_ROUTINE;
      }
      if (enc->direction() == 1) {
	return page_ch::NEXT_PAGE;
      } else if (enc->direction() == -1) {
	return page_ch::PREV_PAGE;
      }
      // ... or something special
    } else {
      if (stng_handler::curr_page == stng_handler::page::CLK_MANUAL) {
	if (stng_handler::data[0] == 1) {
	  if (enc->direction() == 1) {
	    stng_handler::data[1]++;
	    if (stng_handler::data[1] > 33) {
	      stng_handler::data[1] = 0;
	    }
	  } else if (enc->direction() == -1) {
	    stng_handler::data[1]--;
	    if (stng_handler::data[1] > 23) {
	      stng_handler::data[1] = 23;
	    }
	  }
	} else if (stng_handler::data[0] == 2) {
	  if (enc->direction() == 1) {
	    stng_handler::data[2]++;
	    if (stng_handler::data[2] > 59) {
	      stng_handler::data[2] = 0;
	    }
	  } else if (enc->direction() == -1) {
	    stng_handler::data[2]--;
	    if (stng_handler::data[2] > 59) {
	      stng_handler::data[2] = 59;
	    }
	  }
	} else if (stng_handler::data[0] == 3) {
	  if (enc->direction() == 1) {
	    stng_handler::data[3]++;
	    if (stng_handler::data[3] > 59) {
	      stng_handler::data[3] = 0;
	    }
	  } else if (enc->direction() == -1) {
	    stng_handler::data[3]--;
	    if (stng_handler::data[3] > 59) {
	      stng_handler::data[3] = 59;
	    }
	  }
	}
      } else if (stng_handler::curr_page == stng_handler::page::DATE_MANUAL) {
	if (stng_handler::data[0] == 1) {
	  if (enc->direction() == 1) {
	    stng_handler::data[1]++;
	    if (stng_handler::data[1] > 31) {
	      stng_handler::data[1] = 0;
	    }
	  } else if (enc->direction() == -1) {
	    if (stng_handler::data[1] == 1) {
	      stng_handler::data[1] = 31;
	    } else {
	      stng_handler::data[1]--;
	    }
	  }
	} else if (stng_handler::data[0] == 2) {
	  if (enc->direction() == 1) {
	    stng_handler::data[2]++;
	    if (stng_handler::data[2] > 12) {
	      stng_handler::data[2] = 1;
	    }
	  } else if (enc->direction() == -1) {
	    if (stng_handler::data[2] == 1) {
	      stng_handler::data[2] = 12;
	    } else {
	      stng_handler::data[2]--;
	    }
	  }
	} else if (stng_handler::data[0] == 3) {
	  if (enc->direction() == 1) {
	    stng_handler::data[3]++;
	    if (stng_handler::data[3] > 99) {
	      stng_handler::data[3] = 0;
	    }
	  } else if (enc->direction() == -1) {
	    if (stng_handler::data[3] == 0) {
	      stng_handler::data[3] = 99;
	    } else {
	      stng_handler::data[3]--;
	    }
	  }
	}
      } else if (stng_handler::curr_page == stng_handler::page::NTP_OFFSET) {
	if (stng_handler::data[0] == 1) {
	  signed char *h =
	      reinterpret_cast<signed char *>(stng_handler::data + 1);
	  if (enc->direction() == 1) {
	    (*h)++;
	    if ((*h) > 12) {
	      (*h) = -12;
	    }
	  } else if (enc->direction() == -1) {
	    (*h)--;
	    if ((*h) < -12) {
	      (*h) = 12;
	    }
	  }
	} else if (stng_handler::data[0] == 2) {
	  if (enc->direction() == 1) {
	    stng_handler::data[2]++;
	    if (stng_handler::data[2] > 59) {
	      stng_handler::data[2] = 0;
	    }
	  } else if (enc->direction() == -1) {
	    if (stng_handler::data[2] == 0) {
	      stng_handler::data[2] = 59;
	    } else {
	      stng_handler::data[2]--;
	    }
	  }
	}
      }
    }
  }
  return page_ch::NO_CHANGE;
}
#pragma endregion STNG
#pragma region	  BMP
/* bmp_handler.hpp data */
namespace bmp_handler {
bool tmr = true;
}; // namespace bmp_handler
/* bmp_handler.hpp */
void bmp_init(handler_t *handler, void **args) {
  GET_BMP_OBJ(bmp);
  bmp->begin();
}
void bmp_draw(handler_t *handler, void **args) {
  if (bmp_handler::tmr == true) {
    GET_BMP_OBJ(bmp);
    GET_LCD_OBJ(lcd);
    // Measurments vars
    char   status;
    double temp, press;
    // Reading temperature
    status = bmp->startTemperature();
    if (status == 0) {
#if defined(DEBUG) && defined(DBG_STATUS)
      Serial.print(
	  "Error while starting temperature measurments from bmp, errno: ");
      Serial.println(bmp->getError());
#endif
      return;
    }
    delay(status);
    status = bmp->getTemperature(temp);
    if (status == 0) {
#if defined(DEBUG) && defined(DBG_STATUS)
      Serial.print(
	  "Error while reading temperature measurments from bmp, errno: ");
      Serial.println(bmp->getError());
#endif
      return;
    }
    // Reading pressure
    status = bmp->startPressure(3);
    if (status == 0) {
#if defined(DEBUG) && defined(DBG_STATUS)
      Serial.print(
	  "Error while starting pressure measurments from bmp, errno: ");
      Serial.println(bmp->getError());
#endif
      return;
    }
    delay(status);
    status = bmp->getPressure(press, temp);
    if (status == 0) {
#if defined(DEBUG) && defined(DBG_STATUS)
      Serial.print(
	  "Error while reading pressure measurments from bmp, errno: ");
      Serial.println(bmp->getError());
#endif
      return;
    }
    // Conversion
    float fT, fP;
    fT = temp;
    fP = press * 0.750061683;
    // LCD output
    lcd->clear();
    lcd->disableCursor();
    lcd->writeStr("      BMP 180       ", 0, 0);
    lcd->writeStr("              ", 6, 1);
    lcd->writeStr("             ", 7, 2);
    lcd->writeStr("Temp: ", 1, 1);
    lcd->writeCont(fT, (uint8_t)1);
    lcd->writeCont(String((char)223));
    lcd->writeCont("C");
    lcd->writeStr("Press: ", 1, 2);
    lcd->writeCont(fP, (uint8_t)1);
    lcd->writeCont(" mmHg");
    lcd->writeStr("MeteoClock   WolodiaM", 0, 3);
  }
  bmp_handler::tmr = !bmp_handler::tmr;
}
page_ch bmp_ctrl(handler_t *handler, void **args) {
  return page_ch::DEFAULT_CTRL;
}
#pragma endregion BMP
#pragma region	  DHT
/* dht_handler.hpp data */
namespace dht_handler {
uint8_t tmr = 0;
uint8_t max;
float	hum, temp, hi;
} // namespace dht_handler
/* dht_handler.hpp */
void dht_init(handler_t *handler, void **args) {
  GET_DHT_OBJ(dht);
  dht->setup(PIN_DHT, DHT_VER);
  int del	   = dht->getMinimumSamplingPeriod();
  dht_handler::max = ceil(del / 500);
  dht_handler::tmr = dht_handler::max;
#if defined(DHT_TEMP_SIDE) || defined(DHT_HUM_SIDE) || defined(DHT_HI_SIDE)
  rtc_handler::max = dht_handler::max;
  rtc_handler::tmr = dht_handler::tmr;
#endif
}
void dht_draw(handler_t *handler, void **args) {
  // Get pointers
  GET_LCD_OBJ(lcd);
  GET_DHT_OBJ(dht);
  // Check timer
  if (dht_handler::tmr > dht_handler::max) {
    dht_handler::tmr  = 0;
    dht_handler::hum  = dht->getHumidity();
    dht_handler::temp = dht->getTemperature();
    dht_handler::hi =
	dht->computeHeatIndex(dht_handler::temp, dht_handler::hum, false);
    lcd->clear();
  }
  // LCD output
  lcd->disableCursor();
  lcd->writeStr("     DHT 11/22      ", 0, 0);
  lcd->writeStr("Temp: ", 1, 1);
  lcd->writeCont(dht_handler::temp, (uint8_t)1);
  lcd->writeCont(String((char)223));
  lcd->writeCont("C");
  lcd->writeStr("Hum: ", 1, 2);
  lcd->writeCont(dht_handler::hum, (uint8_t)1);
  lcd->writeCont(" %");
  lcd->writeStr("Heat index: ", 1, 3);
  lcd->writeCont(dht_handler::hi, (uint8_t)1);
  lcd->writeCont(String((char)223));
  lcd->writeCont("C");
  // Increment timer
  dht_handler::tmr++;
}
page_ch dht_ctrl(handler_t *handler, void **args) {
  return page_ch::DEFAULT_CTRL;
}
#pragma endregion DHT
