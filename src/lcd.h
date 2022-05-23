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