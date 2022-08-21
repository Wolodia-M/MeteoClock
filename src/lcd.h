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
void lcd_init_text()
{
  lcd.setCursor(0, 0);
  lcd.print("     MeteoClock");
  lcd.setCursor(0, 1);
  lcd.print("1 click - next page");
  lcd.setCursor(0, 2);
  lcd.print("hold btn - on/off");
  lcd.setCursor(0, 3);
  lcd.print("Created by WolodiaM");
  delay(INIT_DEL);
  lcd.clear();
}

void chars()
{
  lcd.createChar(0, block);
  lcd.createChar(1, blockUp);
  lcd.createChar(2, blockDown);
  lcd.createChar(4, upRight);
  lcd.createChar(5, upLeft);
  lcd.createChar(6, downRight);
  lcd.createChar(7, downLeft);
}