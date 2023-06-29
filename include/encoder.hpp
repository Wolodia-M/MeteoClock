/**
 * @file encoder.hpp
 * @author WolodiaM (w_melnyk@outlook.com)
 * @brief Second encoder and button wrapper
 * @date 2023-06-26
 *
 * Wrapper around encoder and button libraries
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
// Arduino libraries
#include "Arduino.h"
#include "RotaryEncoder.h"
#include "btnapi.h"
// Project includes
// Code
#ifndef __ENCODER_HPP__
#define __ENCODER_HPP__
/**
 * @brief State of encoder-button
 */
enum encstate { CLICK, ROTATION, HOLD_ROTATION, NONE };
/**
 * @class Encoder
 * @brief Encoder and button wrapper
 */
class Encoder {
private:
  /**
   * @brief Button handler object
   */
  btnapi btn;
  /**
   * @brief Encoder handler object
   */
  RotaryEncoder enc;
  // Scratch variables
  int	   dir;
  encstate st;
  bool	   click;
  bool	   pressed;
  bool	   rotated;
  long	   prev_enc;

public:
  /**
   * @brief Create new Encoder object
   *
   * @param enk_key => int -> Button pin
   * @param enc_s1 => int -> Encoder S1 pin
   * @param enc_s2 => int -> Encoder S2 pin
   */
  Encoder(int enk_key, int enc_s1, int enc_s2);
  /**
   * @brief Get state of encoder-button system
   *
   * @return encstate -> state
   */
  encstate state();
  /**
   * @brief Ltach status of encoder-button
   */
  void latch();
  /**
   * @brief Tick underlying objects, not latch values
   */
  void tick();
  /**
   * @brief Get direction of encoder
   * enum class Direction {
   *   NOROTATION = 0,
   *   CLOCKWISE = 1,
   *   COUNTERCLOCKWISE = -1
   * };
   * @return int -> Direction
   */
  int direction();
};
#endif // __ENCODER_HPP__
