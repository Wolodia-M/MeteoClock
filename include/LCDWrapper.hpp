/**
 * @file LCDWrapper.hpp
 * @author WolodiaM (w_melnyk@outlook.com)
 * @brief Wrapper for LCD library
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
// C++ libraries
#include "LiquidCrystal_I2C.h"
#ifndef __METEOCLOCK_LCD_HPP__
#define __METEOCLOCK_LCD_HPP__
// Charsets
/**
 * @brief Charset type
 */
enum charset { NUMBERS, STNG_ICONS };
/**
 * @brief Big numbers charset
 */
namespace DIGITS {
enum chars {
  CH_BLOCK = 0,
  CH_BLOCKUP = 1,
  CH_BLOCKDOWN = 2,
  CH_RIGHTUP = 3,
  CH_RIGHTDOWN = 4,
  CH_LEFTUP = 5,
  CH_LEFTDOWN = 6
};
} // namespace DIGITS
/**
 * @brief Custom symbols for settings
 */
namespace STNG {
enum chars {
  COG = 0,
  CLOCK = 1,
  WIFI = 2,
};
} // namespace STNG
/**
 * @brief Lcd wrapper
 */
class LCD {
private:
  // Vars
  LiquidCrystal_I2C *lcd;
  bool backlight = false;
  // Charset load functions
  void loadNumbers();
  // Number print functions
  void printdot(int x);
  void clrdot(int x);
  void print0(int x);
  void print1(int x);
  void print2(int x);
  void print3(int x);
  void print4(int x);
  void print5(int x);
  void print6(int x);
  void print7(int x);
  void print8(int x);
  void print9(int x);
  // Helper
  int getDigit(int num, int pos);
  void printDigit(int x, int dg);

public:
  /**
   * @brief Construct a new LCD wrapper object
   */
  LCD();
  /**
   * @brief Write string to LCD
   *
   * @param str => String -> Data to write
   * @param x => uint8_t -> Column
   * @param y => uint8_t -> Row
   */
  void writeStr(String str, uint8_t x, uint8_t y);
  /**
   * @brief Write number to LCD
   *
   * @param num => int -> Data to write
   * @param x => uint8_t -> Column
   * @param y => uint8_t -> Row
   */
  void writeNum(int num, uint8_t x, uint8_t y);
  /**
   * @brief Write float to LCD
   *
   * @param num => float -> Data to write
   * @param x => uint8_t -> Column
   * @param y => uint8_t -> Row
   * @param prescion => uint8_t -> How many digits left, default = 2
   */
  void writeNum(float num, uint8_t x, uint8_t y, uint8_t prescion = 2);
  /**
   * @brief Print custom symbol
   *
   * @param symbol => uint8_t -> ID
   * @param x => uint8_t -> X
   * @param y => uint8_t -> Y
   */
  void writeSym(uint8_t symbol, uint8_t x, uint8_t y);
  /**
   * @brief Toogle backlight
   */
  void toogle();
  /**
   * @brief Clear display
   */
  void clear();
  /**
   * @brief Pring big full-screan numbers
   *
   * @param h => int -> Hours
   * @param m => int -> Minutes
   */
  void writeBigNumber(int h, int m);
  /**
   * @brief Print without cursor
   *
   * @param str => String -> Data to write
   */
  void writeCont(String str);
  /**
   * @brief Print without cursor
   *
   * @param num => int -> Data to write
   */
  void writeCont(int num);
  /**
   * @brief Print without cursor
   *
   * @param num => float -> Data to write
   * @param prescion => uint8_t -> How many digits left, default = 2
   */
  void writeCont(float num, uint8_t prescion);
  /**
   * @brief Print big dot separetor for big numbers
   *
   * @param state => bool -> Draw or clear
   */
  void writeBigDots(bool state);
  /**
   * @brief Get internal LCD object
   *
   * @return LiquidCrystal_I2C* -> Pointer to lib lcd object
   */
  LiquidCrystal_I2C *getLCD();
  /**
   * @brief Load custom charset
   *
   * @param ch => charset -> Charset type
   */
  void loadCharset(charset ch);
  /**
   * @brief Set the visible cursor on display
   *
   * @param x => uint8_t -> x
   * @param y => uint8_t -> y
   */
  void setCursor(uint8_t x, uint8_t y);
  /**
   * @brief Disable visible cursor
   */
  void disableCursor();
  /**
   * @brief Destroy the LCD wrapper object
   */
  ~LCD();
};
#endif // __METEOCLOCK_LCD_HPP__