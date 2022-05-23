#include <Arduino.h>
#include "MeteoClock.h"

void setup()
{
  // init sensors
  dht.begin();                              // dht11 init
  bmp.begin();                              // bmp180 init
  rtc.begin();                              // rtc1302 init
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
// Timer init
#ifdef HARDVARETIMER
  Timer1.initialize(100000);
  Timer1.attachInterrupt(tmri); // run every 0.1 seconds
#endif
  delay(10); // delay for stability
}

void loop()
{
  btn.tick(); // tick for button
  static byte flag = 0;
  if (btn.isClick()) // if click -> lag++ or flag = 0
  {
    if (flag == MAX_SCREAN)
    { // if flag value is maximum -> flag = 0
      flag = 0;
    }
    else
    { // else -> flag = flag + 1
      flag++;
    }
    lcd.clear(); // clear lcd for new information
  }
  if (flag == 0)
    rtc_check(); // if flag = 0 -> display cool clock
  if (flag == 1)
    dht_check(); // if flag = 1 -> display information from dht
  if (flag == 2)
    bmp_check(); // if flag = 2 -> display information from bmt
  if (flag == 3)
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