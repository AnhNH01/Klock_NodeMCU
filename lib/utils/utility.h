#include <Arduino.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <vector>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

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
void init_lcd(LiquidCrystal_I2C &lcd);
void print_current_time(DateTime &now, LiquidCrystal_I2C &lcd);
void init_rtc(RTC_DS1307 &rtc, LiquidCrystal_I2C &lcd);

void init_alarms(std::vector<Alarm> &alarms);

void parse_list_alarm(std::vector<Alarm> &alarms, String &out);
void parse_time(DateTime &now, String &out);
void parse_date(DateTime &now, String &out);
bool connect_to_wifi();

bool is_alarm_time(DateTime &now, std::vector<Alarm> &alarms);