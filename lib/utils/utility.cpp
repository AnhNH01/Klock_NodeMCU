#include "utility.h"
#include "constants.h"

String twodigits(int t)
{
    return (t < 10) ? "0" + String(t) : String(t);
}

void print_current_time(DateTime &now, LiquidCrystal_I2C &lcd)
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

void init_lcd(LiquidCrystal_I2C &lcd)
{
    lcd.init();
    lcd.backlight();
    lcd.clear();
}

void init_rtc(RTC_DS1307 &rtc, LiquidCrystal_I2C &lcd)
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

void init_alarms(std::vector<Alarm> &alarms)
{
    alarms.emplace_back(Alarm(1, 12, 0, 0));
    alarms.emplace_back(Alarm(2, 13, 0, 0));
    alarms.emplace_back(Alarm(3, 14, 0, 0));
    alarms.emplace_back(Alarm(4, 15, 0, 0));
}

void parse_list_alarm(std::vector<Alarm> &alarms, String &out)
{
    for (auto alarm : alarms)
    {
        out += String(alarm.id) + '-' + String(alarm.hour) + '-' + String(alarm.minute) + '-' + String(alarm.state);
        out += ';';
    }
    out.remove(out.length() - 1);
}

void parse_time(DateTime &now, String &out)
{
    out = "";
    out += twodigits(now.hour());
    out += ':';
    out += twodigits(now.minute());
}

void parse_date(DateTime &now, String &out)
{
    out = "";
    out += twodigits(now.day());
    out += '/';
    out += twodigits(now.month());
    out += '/';
    out += twodigits(now.year());
}

bool connect_to_wifi()
{
    int tries = 0;
    WiFi.begin(SSID, PWD);
    while (WiFi.status() != WL_CONNECTED)
    {
        tries++;
        delay(500);
        Serial.println("Connecting to WiFi..");
    }

    if (tries < 30)
        return false;

    return true;
}

bool is_alarm_time(DateTime &now, std::vector<Alarm> &alarms)
{
    int time_hour = now.hour();
    int time_minute = now.minute();

    for (auto alarm : alarms)
    {
        if (alarm.state == 1 && alarm.hour == time_hour && alarm.minute == time_minute)
                return true;
    }
        
    return false;
}
