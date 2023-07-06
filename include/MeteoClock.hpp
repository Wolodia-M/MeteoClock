/**
 * @file MeteoClock.hpp
 * @author WolodiaM (w_melnyk@outlook.com)
 * @brief Project header
 * @version 1.0
 * @date 2023-06-11
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
// Libs
#include "Arduino.h"
#include "EEPROM.h"
// Project headers
#ifndef __MEOCLOCK_HEADER_HPP__
#define __MEOCLOCK_HEADER_HPP__
// Defines
// Pins
#define PIN_S1 12
#define PIN_S2 14
#define PIN_KEY 13
#define PIN_BTN 15
#define PIN_DHT 16
// Debugs
#define DEBUG	     // Enable / disable debug - DEBUG/NODEBUG
#define NO_DBG_NUM   // Debug numbers DBG_NUM/NO_DBG_NUM
#define DBG_STATUS   // Debug some status info DBG_STATUS/NO_DBG_STATUS
#define NO_DBG_INPUT // Some debug related to input handlers
		     // DBG_INPUT/NO_DBG_INPUT
// Utility
#define BOOT_SECONDS 10 // Time to show boot text
#define VER_STR String("v2.0")
#define DHT_VER DHTesp::DHT11 // DHTesp::DHT11/DHTesp::DHT22
// Pages
#define MAX_PAGE 3
#define DHT_PAGE 3
#define BMP_PAGE 2
#define RTC_PAGE 1
#define STNG_PAGE 0
// Wifi
#include "wifi.hpp"
#define NTP_SERVER "pool.ntp.org"
// Settings
#define NO_AMERICAN_DATE_FORMAT // date.month or month.date,
				// NO_AMERICAN_DATE_FORMAT/AMERICAN_DATE_FORMAT
#define DHT_HUM_SIDE // What will be displayed on right-bottom of clock
		     // screen,
		     // BMP_PRESS_SIDE/BMP_TEMP_SIDE/DHT_TEMP_SIDE/DHT_HUM_SIDE/DHT_HI_SIDE/RTC_SEC_SIDE/NONE
#define TIME_OFFSET 2 // UTC offset
/**
 * @class settings_t
 * @brief Settings of MeteoClock
 * Save values:
 *  - [0]: 0xA5 if saved settings is correct
 *  - [1]: CRC
 *  - [2]: date_format
 *  - [3]: date_divisor
 *  - [4]: utc_offset, as signed char
 *  - [5]: utc_offset_min
 *  - [6]: display_type
 */
struct settings_t {
  /**
   * @brief How date is displayed
   */
  enum date_format_t {
    /**
     * @brief One mode
     * month.date
     * year
     */
    MDY = 0,
    /**
     * @brief Other mode
     * day.month
     * year
     */
    DMY = 1,
    /**
     * @brief Third mode
     * year
     * month.day
     */
    YMD = 2
  } date_format;
  /**
   * @brief What will be betwen date elements
   */
  enum date_divisor_t { DOT = 0, SLASH = 1, DASH = 2 } date_divisor;
  /**
   * @brief UTC time offset for NTP, hours
   */
  signed char utc_offset;
  /**
   * @brief UTC time offset for NTP, minutes
   */
  signed char utc_offset_min;
  /**
   * @brief What will be displayed in lower-right cornen of clock page
   */
  enum side_display_t {
    DHT_TEMP  = 0,
    DHT_HUM   = 1,
    DHT_HI    = 2,
    BMP_TEMP  = 3,
    BMP_PRESS = 4,
    RTC_SEC   = 5,
    NONE
  } side_display;
  /**
   * @brief Size of settings save area
   */
  const int save_size = 256;
  /**
   * @brief Setup save EEPROM area as EEPROM emulation
   */
  settings_t() { EEPROM.begin(this->save_size); }
  /**
   * @brief Stop EEPROM emulation
   */
  ~settings_t() { EEPROM.end(); }
  /**
   * @brief Serialize enum
   *
   * @tparam Enum -> Enum type
   * @param e => Enum -> Enum value
   * @return uint8_t -> Serialized value
   */
  template <typename Enum> uint8_t serializeEnum(Enum e) {
    return static_cast<uint8_t>(e);
  }
  /**
   * @brief Deserialize enum
   *
   * @tparam Enum -> Enum type
   * @param val => uint8_t -> Serialized value
   * @return Enum -> Enum value
   */
  template <typename Enum> Enum deserializeEnum(uint8_t val) {
    return static_cast<Enum>(val);
  }
  uint8_t crc(uint8_t arr[7], bool check = false) {
    if (check == false) {
      uint8_t checksum = 0;
      checksum ^= arr[0];
      checksum ^= arr[2];
      checksum ^= arr[3];
      checksum ^= arr[4];
      checksum ^= arr[5];
      checksum ^= arr[6];
      return checksum;
    } else {
      uint8_t checksum = 0;
      checksum ^= arr[0];
      checksum ^= arr[2];
      checksum ^= arr[3];
      checksum ^= arr[4];
      checksum ^= arr[5];
      checksum ^= arr[6];
      if (arr[1] == checksum) {
	return 1;
      } else {
	return 0;
      }
    }
  }
  /**
   * @brief Save settings
   * @return bool -> Correct save or not
   */
  bool save() {
    // Save and calculate CRC
    uint8_t arr[7];
    arr[1] = 0;
    EEPROM.write(0, 0xA5);
    arr[0] = 0xA5;
    EEPROM.write(
	2, this->serializeEnum<settings_t::date_format_t>(this->date_format));
    arr[2] = this->serializeEnum<settings_t::date_format_t>(this->date_format);
    EEPROM.write(
	3, this->serializeEnum<settings_t::date_divisor_t>(this->date_divisor));
    arr[3] =
	this->serializeEnum<settings_t::date_divisor_t>(this->date_divisor);
    uint8_t *uoff = reinterpret_cast<uint8_t *>(&(this->utc_offset));
    EEPROM.write(4, *uoff);
    arr[4] = *uoff;
    EEPROM.write(5, this->utc_offset_min);
    arr[5] = this->utc_offset_min;
    EEPROM.write(
	6, this->serializeEnum<settings_t::side_display_t>(this->side_display));
    arr[6] =
	this->serializeEnum<settings_t::side_display_t>(this->side_display);
    EEPROM.write(1, this->crc(arr));
    // Apply saved data
    EEPROM.commit();
    // Clear arr
    for (int i = 0; i < 7; i++) {
      arr[i] = 0;
    }
    // Check saved values
    arr[0] = EEPROM.read(0);
    arr[1] = EEPROM.read(1);
    arr[2] = EEPROM.read(2);
    arr[3] = EEPROM.read(3);
    arr[4] = EEPROM.read(4);
    arr[5] = EEPROM.read(5);
    arr[6] = EEPROM.read(6);
    // Check CRC
    if (this->crc(arr, true) == 1) {
      return true;
    } else {
      return false;
    }
    return true;
  }
  /**
   * @brief Load saved settings
   */
  bool load() {
    // Load values
    uint8_t arr[7];
    arr[0] = EEPROM.read(0);
    arr[1] = EEPROM.read(1);
    arr[2] = EEPROM.read(2);
    arr[3] = EEPROM.read(3);
    arr[4] = EEPROM.read(4);
    arr[5] = EEPROM.read(5);
    arr[6] = EEPROM.read(6);
    // Saved data is definetely wrong
    if (arr[0] != 0xA5) {
      return false;
    }
    // // Check CRC
    // if (this->crc(arr, true) == 1) {
    //   return true;
    // } else {
    //   return false;
    // }
    // Apply values
    this->date_format =
	this->deserializeEnum<settings_t::date_format_t>(arr[2]);
    this->date_divisor =
	this->deserializeEnum<settings_t::date_divisor_t>(arr[3]);
    signed char *uoff	 = reinterpret_cast<signed char *>(&(arr[4]));
    this->utc_offset	 = *uoff;
    this->utc_offset_min = arr[5];
    this->side_display =
	this->deserializeEnum<settings_t::side_display_t>(arr[6]);
    return true;
  }
};
#endif // __MEOCLOCK_HEADER_HPP__
