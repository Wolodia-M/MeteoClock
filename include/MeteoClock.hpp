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
// Project headers
#ifndef __MEOCLOCK_HEADER_HPP__
#define __MEOCLOCK_HEADER_HPP__
// Defines
// Pins
#define PIN_S1 12
#define PIN_S2 14
#define PIN_KEY 13
// Debugs
#define DEBUG           // Enable / disable debug - DEBUG/NODEBUG
#define NO_DBG_GENERICK // Some generick debug DBG_GENERICK/NO_DBG_GENERICK
#define NO_DBG_NUM      // Debug numbers DBG_NUM/NO_DBG_NUM
#define DBG_STATUS      // Debug some status info DBG_STATUS/NO_DBG_STATUS
// Utility
#define BOOT_SECONDS 10 // Time to show boot text
#define VER_STR String("v2.0")
// Pages
#define MAX_PAGE 1
#define RTC_PAGE 1
#define STNG_PAGE 0
// Wifi
#define SSID "Xiaomi_HOME"
#define PASSWORD "i749-krid-n7jq"
#define NTP_SERVER "pool.ntp.org"
#define TIMEZONE 2
#endif // __MEOCLOCK_HEADER_HPP__