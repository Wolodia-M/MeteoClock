/**
 * @file DS3231.cpp
 * @author WolodiaM (w_melnyk@outlook.com)
 * @brief Library implementation
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
#include "Wire.h"
// Library headers
#include "DS3231.h"
/* DS3231.h */
// Init
void DS3231::init() {
  Wire.begin(SDA, SCL);
  uint8_t ctrl = this->read(0x0E);
  ctrl &= ~(0b00011111);
  this->write(0x0E, ctrl);
}
// Get
uint8_t DS3231::getSec() {
  uint8_t sec	       = this->read(0x00);
  uint8_t decimalValue = ((sec & 0b01110000) >> 4) * 10 + (sec & 0b00001111);
  return decimalValue;
}
uint8_t DS3231::getMin() {
  uint8_t minute = this->read(0x01);
  uint8_t decimalValue =
      ((minute & 0b01110000) >> 4) * 10 + (minute & 0b00001111);
  return decimalValue;
}
uint8_t DS3231::getHour() {
  uint8_t hour	       = this->read(0x02);
  uint8_t decimalValue = ((hour & 0b00110000) >> 4) * 10 + (hour & 0b00001111);
  return decimalValue;
}
uint8_t DS3231::getDate() {
  uint8_t date	       = this->read(0x04);
  uint8_t decimalValue = ((date & 0b00110000) >> 4) * 10 + (date & 0b00001111);
  return decimalValue;
}
uint8_t DS3231::getMonth() {
  uint8_t month = this->read(0x05);
  uint8_t decimalValue =
      ((month & 0b00010000) >> 4) * 10 + (month & 0b00001111);
  return decimalValue;
}
uint16_t DS3231::getYear() {
  uint8_t  year		= this->read(0x06);
  uint16_t decimalValue = ((year & 0b11110000) >> 4) * 10 + (year & 0b00001111);
  return decimalValue + 2000;
}
float DS3231::getTemp() {
  uint16_t raw = read(0x11);
  raw |= read(0x12) << 8;
  int16_t temp = raw;
  if (temp & 0x0800) {
    temp |= 0xF000;
  }
  return temp * 0.25;
}
uint8_t DS3231::getDay() {
  // Reference date: January 1, 2000 (Saturday)
  uint16_t referenceYear      = 2000;
  uint8_t  referenceMonth     = 1;
  uint8_t  referenceDay	      = 1;
  uint8_t  referenceDayOfWeek = 6; // Saturday (0-based)
  uint8_t  day		      = getDate();
  uint8_t  month	      = getMonth();
  uint16_t year		      = getYear();
  uint64_t days		      = 0;
  for (uint16_t y = referenceYear; y < year; y++) {
    days += this->isLeapYear(y) ? 366 : 365;
  }
  for (uint8_t m = 1; m < month; m++) {
    days += this->daysInMonth(m, year);
  }
  days += day - 1;
  uint8_t dayOfWeek = (referenceDayOfWeek + days) % 7;
  return dayOfWeek;
}
DS3231_HOUR DS3231::getHourType() { return (DS3231_HOUR)(this->read(0x0D)); }
// Set
void DS3231::setSec(uint8_t val) {
  uint8_t bcdValue = ((val / 10) << 4) + (val % 10);
  this->write(0x00, bcdValue);
}
void DS3231::setMin(uint8_t val) {
  uint8_t bcdValue = ((val / 10) << 4) + (val % 10);
  this->write(0x01, bcdValue);
}
void DS3231::setHour(uint8_t val) {
  uint8_t bcdValue = ((val / 10) << 4) + (val % 10);
  this->write(0x02, bcdValue);
}
void DS3231::setDate(uint8_t val) {
  uint8_t bcdValue = ((val / 10) << 4) + (val % 10);
  this->write(0x04, bcdValue);
}
void DS3231::setMonth(uint8_t val) {
  uint8_t bcdValue = ((val / 10) << 4) + (val % 10);
  this->write(0x05, bcdValue);
}
void DS3231::setYear(uint16_t val) {
  uint8_t lastTwoDigits = val % 100;
  uint8_t bcdValue	= ((lastTwoDigits / 10) << 4) + (lastTwoDigits % 10);
  this->write(0x06, bcdValue);
}
void DS3231::setHourType(DS3231_HOUR type) { this->write(0x0D, type); }
// Helper
uint8_t DS3231::daysInMonth(uint8_t mon, uint16_t year) {
  const uint8_t dim[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  // Account for the leap year if it's February
  if (mon == 2 && this->isLeapYear(year)) {
    return 29;
  }

  return dim[mon - 1];
}
bool DS3231::isLeapYear(uint16_t year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
// Internal
void DS3231::write(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(0x68);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}
uint8_t DS3231::read(uint8_t reg) {
  Wire.beginTransmission(0x68);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 1);
  if (Wire.available()) {
    uint8_t val = Wire.read();
    return val;
  }
  return 0;
}
