void charTest()
{
    lcd.setCursor(0, 0);
    lcd.write(BLOCKDOWN);
    lcd.setCursor(0, 1);
    lcd.write(BLOCKUP);
    lcd.setCursor(2, 0);
    lcd.write(BLOCK);
    lcd.setCursor(4, 0);
    lcd.write(RIGHTDOWN);
    lcd.setCursor(5, 0);
    lcd.write(LEFTDOWN);
    lcd.setCursor(4, 1);
    lcd.write(RIGHTUP);
    lcd.setCursor(5, 1);
    lcd.write(LEFTUP);
    while (1)
    {
    }
}

void numberTest()
{
    while (1)
    {
        static byte debflag = 0;
        if (debflag == 0)
        {
            nmb.printNumber(0, 0);
            nmb.printNumber(3, 1);
            nmb.printNumber(6, 2);
            nmb.printNumber(9, 3);
            nmb.printNumber(12, 4);
            nmb.printNumber(15, 5);
            nmb.dot(19, 50);
        }
        if (debflag == 1)
        {
            nmb.printNumber(0, 6);
            nmb.printNumber(3, 7);
            nmb.printNumber(6, 8);
            nmb.printNumber(9, 9);
            nmb.dot(19, 50);
        }
        if (debflag == 2)
        {
            nmb.dot(7, 100);
            nmb.printNumber(0, nmb.convert(46, 1));
            nmb.printNumber(3, nmb.convert(46, 2));
            nmb.printNumber(8, nmb.convert(19, 1));
            nmb.printNumber(11, nmb.convert(19, 2));
        }
        static unsigned long tmr = 0;
        if (millis() - tmr >= 3000)
        {
            tmr = millis();
            lcd.clear();
            debflag++;
            if (debflag > 2)
                debflag = 0;
        }
    }
}