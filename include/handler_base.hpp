/**
 * @file handler_base.hpp
 * @author WolodiaM (w_melnyk@outlook.com)
 * @brief Base for handlers
 * @version 1.0
 * @date 2023-06-14
 *
 * @copyright (C) 2023 WolodiaM
 * @license GPL v3.0 or later
 *
 * Copyright (C) 2023  WolodiaM
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
#ifndef __METEOCLOCK_HANDLER_BASE_HPP__
#define __METEOCLOCK_HANDLER_BASE_HPP__
#define GET_PAGE_VAR(page) uint8_t *page = (uint8_t *)(*(args + 0))
#define GET_LCD_OBJ(lcd) LCD *lcd = (LCD *)(*(args + 1))
#define GET_RTC_OBJ(rtc) DS3231 *rtc = (DS3231 *)(*(args + 2))
#define GET_ENC_OBJ(enc) Encoder *enc = (Encoder *)(*(args + 3))
#define GET_UDP_OBJ(udp) WiFiUDP *udp = (WiFiUDP *)(*(args + 4))
#define GET_NTP_OBJ(ntp) NTPClient *ntp = (NTPClient *)(*(args + 5))
#define GET_BMP_OBJ(bmp) SFE_BMP180 *bmp = (SFE_BMP180 *)(*(args + 6))
#define GET_BTN_OBJ(btn) btnapi *btn = (btnapi *)(*(args + 7))
#define GET_DHT_OBJ(dht) DHTesp *dht = (DHTesp *)(*(args + 8))
#define GET_STNG_VAR(stng) settings_t *stng = (settings_t *)(*(args + 9))
/**
 * @brief How to change page based on handler_t::ctrl
 */
enum page_ch {
  /**
   * @brief Go to next page (or wrap around)
   */
  NEXT_PAGE,
  /**
   * @brief Go to previvious page (or wrap around)
   */
  PREV_PAGE,
  /**
   * @brief Page not changed
   */
  NO_CHANGE,
  /**
   * @brief Default page change handler -> if rotated without halt - change page
   */
  DEFAULT_CTRL
};
/**
 * @brief Handler storage object
 */
struct handler_t {
  /**
   * @brief Init function of this handler
   */
  void (*init)(handler_t *, void **);
  /**
   * @brief Draw function of this handler, call every 500 ms
   */
  void (*draw)(handler_t *, void **);
  /**
   * @brief Control function of this handler, call on every encoder change
   */
  page_ch (*ctrl)(handler_t *, void **);
};
#endif // __METEOCLOCK_HANDLER_BASE_HPP__
