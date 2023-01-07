#include <Arduino.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <vector>

struct Alarm
{
    int id;
    int hour;
    int minute;
    int state;

    Alarm(int id, int hour, int minute, int state)
    {
        this->id = id;
        this->hour = hour;
        this->minute = minute;
        this->state = state;
    }
};

String twodigits(int t);
void initLcd(LiquidCrystal_I2C &lcd);
void currentTime(DateTime &now, LiquidCrystal_I2C &lcd);
void initRtc(RTC_DS1307 &rtc, LiquidCrystal_I2C &lcd);

void initAlarms(std::vector<Alarm> &alarms);