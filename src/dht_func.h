void dht_check()
{
    lcd.setCursor(0, 0);
    lcd.print("     DHT Sensor");
    lcd.setCursor(0, 1);
    ;
    lcd.print("Temp: ");
    lcd.print(dht.readTemperature());
    lcd.print("C;");
    lcd.setCursor(0, 2);
    lcd.print("Hum:  ");
    lcd.print(dht.readHumidity());
    lcd.print("%;");
    lcd.setCursor(0, 3);
    lcd.print("     MeteoClock");
}