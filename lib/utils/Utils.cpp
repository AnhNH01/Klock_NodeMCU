#include "Utils.h"
String twodigits(int t)
{
    return (t < 10) ? "0" + String(t) : String(t);
}

void currentTime(DateTime &now, LiquidCrystal_I2C &lcd)
{
    lcd.setCursor(0, 0);
    lcd.printf("TIME: %s:%s:%s",
               twodigits(now.hour()).c_str(),
               twodigits(now.minute()).c_str(),
               twodigits(now.second()).c_str());

    lcd.setCursor(0, 1);
    lcd.printf("DATE: %s/%s/%d",
               twodigits(now.day()).c_str(),
               twodigits(now.month()).c_str(),
               now.year());
}

void initLcd(LiquidCrystal_I2C &lcd)
{
    lcd.init();
    lcd.backlight();
    lcd.clear();
}

void initRtc(RTC_DS1307 &rtc, LiquidCrystal_I2C &lcd)
{
    if (!rtc.begin())
    {
        lcd.print("Couldn't find RTC");
        while (1)
            ;
    }
    if (!rtc.isrunning())
        lcd.print("RTC is NOT running!");
}

void initAlarms(std::vector<Alarm> &alarms)
{
    alarms.emplace_back(Alarm(1, 12, 0, 0));
    alarms.emplace_back(Alarm(2, 13, 0, 0));
    alarms.emplace_back(Alarm(2, 14, 0, 0));
    alarms.emplace_back(Alarm(2, 15, 0, 0));
}