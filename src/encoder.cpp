/**
 * @file encoder.cpp
 * @author WolodiaM (w_melnyk@outlook.com)
 * @brief Encoder wrapper
 * @date 2023-06-28
 *
 * Wrapper around RotaryEncoder and btnapi libs, used to eaily combine reads of
 * encoder and button state and convert it to some complex combined states
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
#include "RotaryEncoder.h"
#include "btnapi.h"
// Project includes
#include "encoder.hpp"
/* encoder.hpp */
Encoder::Encoder(int enk_key, int enc_s1, int enc_s2)
    : btn(enk_key), enc(enc_s1, enc_s2, RotaryEncoder::LatchMode::FOUR3) {
  this->dir	 = 0;
  this->prev_enc = 0;
  this->pressed	 = false;
  this->click	 = false;
  this->rotated	 = false;
  this->st	 = encstate::NONE;
  this->enc.setPosition(0);
}
void Encoder::latch() {
  // Button reads
  this->click	= this->btn.isClick();
  this->pressed = this->btn.state();
  // Encoder reads
  this->dir = 0;
  switch (this->enc.getDirection()) {
  case RotaryEncoder::Direction::CLOCKWISE:
    this->dir = 1;
    break;
  case RotaryEncoder::Direction::COUNTERCLOCKWISE:
    this->dir = -1;
    break;
  case RotaryEncoder::Direction::NOROTATION:
    this->dir = 0;
    break;
  }
  long encr	= this->enc.getPosition();
  this->rotated = false;
  // Calculate new state
  if (this->prev_enc != encr) {
    this->rotated = true;
    if (this->pressed) {
      this->st = encstate::HOLD_ROTATION;
    } else {
      this->st = encstate::ROTATION;
    }
  } else if (this->click) {
    this->st = encstate::CLICK;
  } else {
    this->st = encstate::NONE;
  }
  this->prev_enc = encr;
}
void Encoder::tick() {
  this->enc.tick();
  this->btn.tick();
}
encstate Encoder::state() { return this->st; }
int	 Encoder::direction() { return this->dir; }
