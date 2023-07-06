/**
 * @file lcd.cpp
 * @author WolodiaM (w_melnyk@outlook.com)
 * @brief LCDWrapper implementation
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
#include "Wire.h"
// Peoject headers
#include "LCDWrapper.hpp"
/**
 * @brief Big numbers charset
 */
namespace DIGITS {
uint8_t block[] = {0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};
uint8_t blockUp[] = {0x1F, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00};
uint8_t blockDown[] = {0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F};
uint8_t upRight[] = {0x1F, 0x1F, 0x0F, 0x0F, 0x07, 0x07, 0x03, 0x03};
uint8_t upLeft[] = {0x1F, 0x1F, 0x1E, 0x1E, 0x1C, 0x1C, 0x18, 0x18};
uint8_t downRight[] = {0x03, 0x03, 0x07, 0x07, 0x0F, 0x0F, 0x1F, 0x1F};
uint8_t downLeft[] = {0x18, 0x18, 0x1C, 0x1C, 0x1E, 0x1E, 0x1F, 0x1F};
} // namespace DIGITS
/**
 * @brief Custom symbols for settings
 */
namespace STNG {
uint8_t cog[] = {0x00, 0x0A, 0x15, 0x0A, 0x15, 0x0A, 0x00, 0x00};
uint8_t clock[] = {0x00, 0x0E, 0x04, 0x0E, 0x15, 0x15, 0x13, 0x0E};
uint8_t wifi[] = {0x00, 0x04, 0x0A, 0x11, 0x04, 0x0A, 0x04, 0x04};
uint8_t ok[] = {0x00, 0x01, 0x01, 0x11, 0x0A, 0x0A, 0x04, 0x00};
uint8_t save[] = {0x00, 0x00, 0x04, 0x04, 0x15, 0x0E, 0x04, 0x00};
uint8_t load[] = {0x00, 0x04, 0x0E, 0x15, 0x04, 0x04, 0x04, 0x00};
} // namespace STNG
/* LCDWrapper.hpp */
LCD::LCD() {
  this->lcd = new LiquidCrystal_I2C(0x27, 20, 4);
  this->lcd->init();
  this->loadCharset(charset::NUMBERS);
  this->toogle();
}
LCD::~LCD() { delete this->lcd; }
// Load charsets
void LCD::loadNumbers() {
  this->lcd->createChar(DIGITS::CH_BLOCK, DIGITS::block);
  this->lcd->createChar(DIGITS::CH_BLOCKDOWN, DIGITS::blockDown);
  this->lcd->createChar(DIGITS::CH_BLOCKUP, DIGITS::blockUp);
  this->lcd->createChar(DIGITS::CH_LEFTDOWN, DIGITS::downLeft);
  this->lcd->createChar(DIGITS::CH_LEFTUP, DIGITS::upLeft);
  this->lcd->createChar(DIGITS::CH_RIGHTDOWN, DIGITS::downRight);
  this->lcd->createChar(DIGITS::CH_RIGHTUP, DIGITS::upRight);
}
// Helper
int LCD::getDigit(int num, int pos) {
  switch (pos) {
  case 1:
    return num / 10;
    break;
  case 2:
    return num % 10;
    break;
  }
  return -1;
}
void LCD::printDigit(int x, int dg) {
  switch (dg) {
  case 0:
    this->print0(x);
    break;
  case 1:
    this->print1(x);
    break;
  case 2:
    this->print2(x);
    break;
  case 3:
    this->print3(x);
    break;
  case 4:
    this->print4(x);
    break;
  case 5:
    this->print5(x);
    break;
  case 6:
    this->print6(x);
    break;
  case 7:
    this->print7(x);
    break;
  case 8:
    this->print8(x);
    break;
  case 9:
    this->print9(x);
    break;
  }
}
// Big number print
#pragma region BigNum
void LCD::printdot(int x) {
  lcd->setCursor(x, 0);
  lcd->write(DIGITS::CH_BLOCKDOWN);
  lcd->setCursor(x, 1);
  lcd->write(DIGITS::CH_BLOCKUP);
  lcd->setCursor(x, 2);
  lcd->write(DIGITS::CH_BLOCKDOWN);
  lcd->setCursor(x, 3);
  lcd->write(DIGITS::CH_BLOCKUP);
}
void LCD::clrdot(int x) {
  lcd->setCursor(x, 0);
  lcd->print(" ");
  lcd->setCursor(x, 1);
  lcd->print(" ");
  lcd->setCursor(x, 2);
  lcd->print(" ");
  lcd->setCursor(x, 3);
  lcd->print(" ");
}
void LCD::print0(int x) {
  lcd->setCursor(x, 0);
  lcd->write(DIGITS::CH_RIGHTDOWN);
  lcd->setCursor(x + 1, 0);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x + 2, 0);
  lcd->write(DIGITS::CH_LEFTDOWN);
  lcd->setCursor(x, 1);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x, 2);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x + 1, 1);
  lcd->print(" ");
  lcd->setCursor(x + 1, 2);
  lcd->print(" ");
  lcd->setCursor(x + 2, 1);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x + 2, 2);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x, 3);
  lcd->write(DIGITS::CH_RIGHTUP);
  lcd->setCursor(x + 1, 3);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x + 2, 3);
  lcd->write(DIGITS::CH_LEFTUP);
}
void LCD::print1(int x) {
  lcd->setCursor(x, 0);
  lcd->print(" ");
  lcd->setCursor(x + 1, 0);
  lcd->write(DIGITS::CH_RIGHTDOWN);
  lcd->setCursor(x + 2, 0);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x, 1);
  lcd->print(" ");
  lcd->setCursor(x + 1, 1);
  lcd->print(" ");
  lcd->setCursor(x + 2, 1);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x, 2);
  lcd->print(" ");
  lcd->setCursor(x + 1, 2);
  lcd->print(" ");
  lcd->setCursor(x + 2, 2);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x, 3);
  lcd->print(" ");
  lcd->setCursor(x + 1, 3);
  lcd->print(" ");
  lcd->setCursor(x + 2, 3);
  lcd->write(DIGITS::CH_BLOCK);
}
void LCD::print2(int x) {
  lcd->setCursor(x, 0);
  lcd->write(DIGITS::CH_RIGHTDOWN);
  lcd->setCursor(x + 1, 0);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x + 2, 0);
  lcd->write(DIGITS::CH_LEFTDOWN);
  lcd->setCursor(x, 1);
  lcd->print(" ");
  lcd->setCursor(x + 1, 1);
  lcd->print(" ");
  lcd->setCursor(x + 2, 1);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x, 2);
  lcd->print(" ");
  lcd->setCursor(x + 1, 2);
  lcd->write(DIGITS::CH_RIGHTDOWN);
  lcd->setCursor(x + 2, 2);
  lcd->write(DIGITS::CH_LEFTUP);
  lcd->setCursor(x, 3);
  lcd->write(DIGITS::CH_RIGHTDOWN);
  lcd->setCursor(x + 1, 3);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x + 2, 3);
  lcd->write(DIGITS::CH_BLOCK);
}
void LCD::print3(int x) {
  lcd->setCursor(x, 0);
  lcd->write(DIGITS::CH_RIGHTDOWN);
  lcd->setCursor(x + 1, 0);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x + 2, 0);
  lcd->write(DIGITS::CH_LEFTDOWN);
  lcd->setCursor(x, 1);
  lcd->print(" ");
  lcd->setCursor(x + 1, 1);
  lcd->write(DIGITS::CH_BLOCKDOWN);
  lcd->setCursor(x + 2, 1);
  lcd->write(DIGITS::CH_LEFTUP);
  lcd->setCursor(x, 2);
  lcd->print(" ");
  lcd->setCursor(x + 1, 2);
  lcd->write(DIGITS::CH_BLOCKUP);
  lcd->setCursor(x + 2, 2);
  lcd->write(DIGITS::CH_LEFTDOWN);
  lcd->setCursor(x, 3);
  lcd->write(DIGITS::CH_RIGHTUP);
  lcd->setCursor(x + 1, 3);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x + 2, 3);
  lcd->write(DIGITS::CH_LEFTUP);
}
void LCD::print4(int x) {
  lcd->setCursor(x, 0);
  lcd->print(" ");
  lcd->setCursor(x + 1, 0);
  lcd->write(DIGITS::CH_RIGHTDOWN);
  lcd->setCursor(x + 2, 0);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x, 1);
  lcd->write(DIGITS::CH_RIGHTDOWN);
  lcd->setCursor(x + 1, 1);
  lcd->write(DIGITS::CH_LEFTUP);
  lcd->setCursor(x + 2, 1);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x, 2);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x + 1, 2);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x + 2, 2);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x, 3);
  lcd->print(" ");
  lcd->setCursor(x + 1, 3);
  lcd->print(" ");
  lcd->setCursor(x + 2, 3);
  lcd->write(DIGITS::CH_BLOCK);
}
void LCD::print5(int x) {
  lcd->setCursor(x, 0);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x + 1, 0);
  lcd->write(DIGITS::CH_BLOCKUP);
  lcd->setCursor(x + 2, 0);
  lcd->write(DIGITS::CH_BLOCKUP);
  lcd->setCursor(x, 1);
  lcd->write(DIGITS::CH_RIGHTUP);
  lcd->setCursor(x + 1, 1);
  lcd->write(DIGITS::CH_LEFTDOWN);
  lcd->setCursor(x + 2, 1);
  lcd->print(" ");
  lcd->setCursor(x, 2);
  lcd->print(" ");
  lcd->setCursor(x + 1, 2);
  lcd->write(DIGITS::CH_RIGHTUP);
  lcd->setCursor(x + 2, 2);
  lcd->write(DIGITS::CH_LEFTDOWN);
  lcd->setCursor(x, 3);
  lcd->write(DIGITS::CH_RIGHTUP);
  lcd->setCursor(x + 1, 3);
  lcd->write(DIGITS::CH_BLOCKDOWN);
  lcd->setCursor(x + 2, 3);
  lcd->write(DIGITS::CH_LEFTUP);
}
void LCD::print6(int x) {
  lcd->setCursor(x, 0);
  lcd->print(" ");
  lcd->setCursor(x + 1, 0);
  lcd->write(DIGITS::CH_RIGHTDOWN);
  lcd->setCursor(x + 2, 0);
  lcd->write(DIGITS::CH_LEFTUP);
  lcd->setCursor(x, 1);
  lcd->write(DIGITS::CH_RIGHTDOWN);
  lcd->setCursor(x + 1, 1);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x + 2, 1);
  lcd->print(" ");
  lcd->setCursor(x, 2);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x + 1, 2);
  lcd->print(" ");
  lcd->setCursor(x + 2, 2);
  lcd->write(DIGITS::CH_LEFTDOWN);
  lcd->setCursor(x, 3);
  lcd->write(DIGITS::CH_RIGHTUP);
  lcd->setCursor(x + 1, 3);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x + 2, 3);
  lcd->write(DIGITS::CH_LEFTUP);
}
void LCD::print7(int x) {
  lcd->setCursor(x, 0);
  lcd->write(DIGITS::CH_BLOCKUP);
  lcd->setCursor(x + 1, 0);
  lcd->write(DIGITS::CH_BLOCKUP);
  lcd->setCursor(x + 2, 0);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x, 1);
  lcd->print(" ");
  lcd->setCursor(x + 1, 1);
  lcd->write(DIGITS::CH_RIGHTDOWN);
  lcd->setCursor(x + 2, 1);
  lcd->write(DIGITS::CH_LEFTUP);
  lcd->setCursor(x, 2);
  lcd->write(DIGITS::CH_RIGHTDOWN);
  lcd->setCursor(x + 1, 2);
  lcd->write(DIGITS::CH_LEFTUP);
  lcd->setCursor(x + 2, 2);
  lcd->print(" ");
  lcd->setCursor(x, 3);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x + 1, 3);
  lcd->print(" ");
  lcd->setCursor(x + 2, 3);
  lcd->print(" ");
}
void LCD::print8(int x) {
  lcd->setCursor(x, 0);
  lcd->write(DIGITS::CH_RIGHTDOWN);
  lcd->setCursor(x + 1, 0);
  lcd->write(DIGITS::CH_BLOCKUP);
  lcd->setCursor(x + 2, 0);
  lcd->write(DIGITS::CH_LEFTDOWN);
  lcd->setCursor(x, 1);
  lcd->write(DIGITS::CH_RIGHTUP);
  lcd->setCursor(x + 1, 1);
  lcd->write(DIGITS::CH_BLOCKDOWN);
  lcd->setCursor(x + 2, 1);
  lcd->write(DIGITS::CH_LEFTUP);
  lcd->setCursor(x, 2);
  lcd->write(DIGITS::CH_RIGHTDOWN);
  lcd->setCursor(x + 1, 2);
  lcd->write(DIGITS::CH_BLOCKUP);
  lcd->setCursor(x + 2, 2);
  lcd->write(DIGITS::CH_LEFTDOWN);
  lcd->setCursor(x, 3);
  lcd->write(DIGITS::CH_RIGHTUP);
  lcd->setCursor(x + 1, 3);
  lcd->write(DIGITS::CH_BLOCKDOWN);
  lcd->setCursor(x + 2, 3);
  lcd->write(DIGITS::CH_LEFTUP);
}
void LCD::print9(int x) {
  lcd->setCursor(x, 0);
  lcd->write(DIGITS::CH_RIGHTDOWN);
  lcd->setCursor(x + 1, 0);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x + 2, 0);
  lcd->write(DIGITS::CH_LEFTDOWN);
  lcd->setCursor(x + 2, 1);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x + 1, 1);
  lcd->print(" ");
  lcd->setCursor(x, 1);
  lcd->write(DIGITS::CH_RIGHTUP);
  lcd->setCursor(x, 2);
  lcd->print(" ");
  lcd->setCursor(x + 1, 2);
  lcd->write(DIGITS::CH_BLOCK);
  lcd->setCursor(x + 2, 2);
  lcd->write(DIGITS::CH_LEFTUP);
  lcd->setCursor(x, 3);
  lcd->write(DIGITS::CH_RIGHTDOWN);
  lcd->setCursor(x + 1, 3);
  lcd->write(DIGITS::CH_LEFTUP);
  lcd->setCursor(x + 2, 3);
  lcd->print(" ");
}
#pragma endregion BigNum
// Direct print functions
void LCD::writeNum(int num, uint8_t x, uint8_t y, int pad) {
  this->lcd->setCursor(x, y);
  if (pad <= 0) {
    this->lcd->printf("%d", num);
  } else {
    String str = "%" + String(pad) + "d";
    this->lcd->printf(str.c_str(), num);
  }
}
void LCD::writeNum(float num, uint8_t x, uint8_t y, uint8_t prescion) {
  this->lcd->setCursor(x, y);
  String val = String("%.") + String(prescion) + String("f");
  this->lcd->printf(val.c_str(), num);
}
void LCD::writeStr(String str, uint8_t x, uint8_t y) {
  this->lcd->setCursor(x, y);
  this->lcd->print(str);
}
void LCD::writeBigNumber(int h, int m) {
  // Print hours
  this->printDigit(0, this->getDigit(h, 1));
  this->printDigit(3, this->getDigit(h, 2));
  // Print minutes
  this->printDigit(9, this->getDigit(m, 1));
  this->printDigit(12, this->getDigit(m, 2));
}
void LCD::writeCont(String str) { this->lcd->print(str); }
void LCD::writeCont(int num, int pad) {
  if (pad <= 0) {
    this->lcd->printf("%d", num);
  } else {
    String str = "%" + String(pad) + "d";
    this->lcd->printf(str.c_str(), num);
  }
}
void LCD::writeCont(float num, uint8_t prescion) {
  String val = String("%.") + String(prescion) + String("f");
  this->lcd->printf(val.c_str(), num);
}
void LCD::writeBigDots(bool state) {
  if (state == true) {
    this->printdot(7);
  } else {
    this->clrdot(7);
  }
}
void LCD::writeSym(uint8_t symbol, uint8_t x, uint8_t y) {
  this->lcd->setCursor(x, y);
  this->lcd->write(symbol);
}
// Utility functions
void LCD::toogle() {
  this->backlight = !(this->backlight);
  if (this->backlight == true) {
    this->lcd->backlight();
  } else {
    this->lcd->noBacklight();
  }
}
void LCD::clear() { this->lcd->clear(); }
LiquidCrystal_I2C *LCD::getLCD() { return this->lcd; }
void LCD::loadCharset(charset ch) {
  switch (ch) {
  case charset::NUMBERS:
    this->loadNumbers();
    return;
  case charset::STNG_ICONS:
    this->lcd->createChar(STNG::COG, STNG::cog);
    this->lcd->createChar(STNG::CLOCK, STNG::clock);
    this->lcd->createChar(STNG::WIFI, STNG::wifi);
    this->lcd->createChar(STNG::OK, STNG::ok);
    this->lcd->createChar(STNG::LOAD, STNG::load);
    this->lcd->createChar(STNG::SAVE, STNG::save);
    break;
  default:
    return;
  }
}
void LCD::setCursor(uint8_t x, uint8_t y) {
  this->lcd->setCursor(x, y);
  this->lcd->cursor_on();
}
void LCD::disableCursor() { this->lcd->cursor_off(); }
