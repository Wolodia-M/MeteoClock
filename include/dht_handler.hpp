/**
 * @file dht_handler.hpp
 * @author WolodiaM (w_melnyk@outlook.com)
 * @brief DHT screen
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
// Project headers
#include "handler_base.hpp"
// Code
#ifndef __METEOCLOCK_DHT_HPP__
#define __METEOCLOCK_DHT_HPP__
#define DHT_TABLE_ENTRY [DHT_PAGE] = {dht_init, dht_draw, dht_ctrl}
void	dht_init(handler_t *handler, void **args);
void	dht_draw(handler_t *handler, void **args);
page_ch dht_ctrl(handler_t *handler, void **args);
#endif // __METEOCLOCK_DHT_HPP__
