void bmp_check()
{
    lcd.setCursor(0, 0);
    lcd.print("     BMP Sensor");
    lcd.setCursor(0, 1);
    lcd.print("Press: ");
    lcd.print(bmp.readPressure() / 133.3223684);
    lcd.print("mmHg;");
    lcd.setCursor(0, 2);
    lcd.print("Temp:  ");
    lcd.print(bmp.readTemperature());
    lcd.print("C;");
    lcd.setCursor(0, 3);
    lcd.print("     MeteoClock");
}