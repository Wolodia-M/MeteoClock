// Meteo Clock - cool clock with meteostation on Arduino
// Copyright (C) 2022 WolodiaM

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#include <Arduino.h>
#include "MeteoClock.h"

void setup()
{
  // init sensors
  dht.begin();                              // dht11 init
  bmp.begin();                              // bmp180 init
  rtc.begin();                              // rtcxxxx init
  rtc.adjust(DateTime(__DATE__, __TIME__)); // set time to rtc
  // lcd init
  lcd.init();
  lcd.backlight();
  chars(); // initializing custom chars for number
#ifdef CHARTEST
  charTest();
#endif
#ifdef NUMBERTEST
  numberTest();
#endif
  // init text
  lcd_init_text();
  Serial.begin(115200);
  Serial.println("Inited");
  delay(10); // delay for stability
}

void loop()
{
  btn.tick(); // tick for button
  static byte screen = 0;
  if (btn.isClick()) // if click -> screen++ or screen = 0
  {
    Serial.println("Click");
    if (screen == MAX_SCREAN)
    { // if screen value is maximum -> screen = 0
      screen = 0;
    }
    else
    { // else -> flag = flag + 1
      screen++;
    }
    lcd.clear(); // clear lcd for new information
  }
  if (screen == 0)
    rtc_check(); // if flag = 0 -> display cool clock
  if (screen == 1)
    dht_check(); // if flag = 1 -> display information from dht
  if (screen == 2)
    bmp_check(); // if flag = 2 -> display information from bmt
  if (screen == 3)
    rtc_check_old(); // if flag = 3 -> display clock data only
  static bool holded = 0;
  if (!holded && btn.isHold()) // if button hold -> on/off display
  {
    static bool flagH = 1;
    flagH = !flagH;
    holded = 1;
    if (!flagH) // off display
    {
      lcd.noDisplay();
      lcd.noBacklight();
    }
    if (flagH) // on display
    {
      lcd.display();
      lcd.backlight();
    }
  }
  if (btn.state() == 0)
    holded = 0;
}