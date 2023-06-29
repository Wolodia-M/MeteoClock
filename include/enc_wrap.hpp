/**
 * @file enc_wrap.hpp
 * @author WolodiaM (w_melnyk@outlook.com)
 * @brief Encoder wrapper
 * @version 1.0
 * @date 2023-06-15
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
// C++ libraries
#include "Arduino.h"
#include "RotaryEncoder.h"
#include "btnapi.h"
// Code
#ifndef __METEOCLOCK_ENCWRAP_HPP__
#define __METEOCLOCK_ENCWRAP_HPP__
class encapi {
private:
  RotaryEncoder enc;
  btnapi	btn;

public:
  encapi(int s1, int s2, int key)
      : enc(s1, s2, RotaryEncoder::LatchMode::FOUR3), btn(key) {}
  void			   tick() { this->enc.tick(); }
  void			   setPosition(long pos) { this->enc.setPosition(pos); }
  long			   getPosition() { return this->enc.getPosition(); }
  RotaryEncoder::Direction getDirection() { return this->enc.getDirection(); }
  unsigned long		   getRPM() { return this->enc.getRPM(); }
  bool			   rotatedHold() {
	if (this->btn.state()) {
	  return true;
	}
	return false;
  }
  bool btnClick() { return this->btn.isClick(); }
  bool btnState() { return this->btn.state(); }
  /**
   * @brief For use without RotaryEncoder classes:
   * NOROTATION = 0,
   * CLOCKWISE = 1,
   * COUNTERCLOCKWISE = -1
   */
  int getDir() { return (int)this->enc.getDirection(); }
};
#endif // __METEOCLOCK_ENCWRAP_HPP__
