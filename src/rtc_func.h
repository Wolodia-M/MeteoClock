void rtc_check_old()
{
    DateTime now = rtc.now();
    lcd.setCursor(0, 0);
    lcd.print("      RTC Data");
    lcd.setCursor(3, 1);
    lcd.print(now.year());
    lcd.print('.');
    lcd.print(now.month());
    lcd.print('.');
    lcd.print(now.day());
    lcd.setCursor(3, 2);
    lcd.print(daysOfTheWeek[now.dayOfWeek()]);
    lcd.setCursor(3, 3);
    lcd.print(now.hour());
    lcd.print(':');
    lcd.print(now.minute());
    lcd.print(':');
    lcd.setCursor(10, 3);
    lcd.print(" ");
    lcd.setCursor(9, 3);
    lcd.print(now.second());
}

void rtc_check()
{
#ifdef MILLIS
    static unsigned long tmr = 0;
    if (millis() - tmr >= 500)
    {
        dotCounter = !dotCounter;
        now = rtc.now();
        nmb.dot(7, dotCounter);
    }
#endif
    nmb.printNumber(0, nmb.convert(now.hour(), 1));
    nmb.printNumber(3, nmb.convert(now.hour(), 2));
    nmb.printNumber(9, nmb.convert(now.minute(), 1));
    nmb.printNumber(12, nmb.convert(now.minute(), 2));
    lcd.setCursor(16, 0);
    lcd.print(daysOfTheWeekShort[now.dayOfWeek()]);
    lcd.setCursor(15, 1);
    lcd.print(now.month());
    lcd.print(".");
    lcd.print(now.day());
    lcd.print(".");
    lcd.setCursor(15, 2);
    lcd.print(now.year());
#ifndef NODATA
    lcd.setCursor(15, 3);
#endif
#if defined(TEMPDHT)
    lcd.print(dht.readTemperature());
#elif defined(TEMPBMP)
    lcd.print(bmp.readTemperature());
#elif defined(HUMDHT)
    lcd.print(dht.readHumidity());
#elif defined(PRESSBMP)
    lcd.print(bmp.readPressure() / 133.3223684);
#elif defined(SECOND)
    lcd.setCursor(17, 3);
    lcd.print(" ");
    lcd.setCursor(16, 3);
    lcd.print(now.second());
#endif
}

#ifdef HARDVARETIMER
void tmri()
{
    counter++;
    dotCounter = !dotCounter;
    nmb.dot(7, dotCounter);
    if (counter == 5)
    {
        now = rtc.now();
        counter = 0;
    }
}
#endif