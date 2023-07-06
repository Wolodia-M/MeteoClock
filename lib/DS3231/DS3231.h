/**
 * @file DS3231.h
 * @author WolodiaM (w_melnyk@outlook.com)
 * @brief Library for DS3231 RTC
 * @version 1.0
 * @date 2023-06-12
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
#include "Arduino.h"
// Code
#ifndef __DS3231_H__
#define __DS3231_H__
enum DS3231_HOUR { AM = 0, PM = 1 };
class DS3231 {
public:
  // Init
  void init();
  // Get
  uint8_t     getSec();
  uint8_t     getMin();
  uint8_t     getHour();
  uint8_t     getDate();
  uint8_t     getMonth();
  uint16_t    getYear();
  uint8_t     getDay();
  DS3231_HOUR getHourType();
  float	      getTemp();
  // Set
  void setSec(uint8_t val);
  void setMin(uint8_t val);
  void setHour(uint8_t val);
  void setDate(uint8_t val);
  void setMonth(uint8_t val);
  void setYear(uint16_t val);
  void setHourType(DS3231_HOUR type);
  // Helper
  uint8_t daysInMonth(uint8_t mon, uint16_t year);
  bool	  isLeapYear(uint16_t year);

private:
  // Internal
  void	  write(uint8_t reg, uint8_t val);
  uint8_t read(uint8_t reg);
};
#endif // __DS3231_H__
