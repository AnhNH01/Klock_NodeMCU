#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <vector>
#include "utility.h"
#include "mqttconnection.h"
#include "constants.h"

millisDelay show_time_delay;
millisDelay publish_time_delay;
RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, LCD_COLUMNS, LCD_ROWS);
WiFiClient espClient;
PubSubClient client(espClient);

String client_id = "ESP@";

std::vector<Alarm> alarms;

void setup()
{
    Serial.begin(9600);
    WiFi.mode(WIFI_AP_STA);

    // set pin modes
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);
    digitalWrite(BUZZER_PIN, HIGH);
    // init lcd
    init_lcd(lcd);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SETTING UP");

    // init rtc and alarms
    init_rtc(rtc, lcd);
    init_alarms(alarms);

    // connect to wifi
    if (connect_to_wifi())
    {
        Serial.println("Connected to Wifi");

        // if wifi connnected, connect to broker
        bool broker_connected = mqtt::connect(client, client_id);
        if (broker_connected)
        {
            Serial.println("Connected to broker");
        }
        else
        {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("BROKER ERROR");
            lcd.setCursor(0, 1);
            lcd.print("RESTART?");
            delay(3000);
        }
    }
    else
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("NO WIFI");
        lcd.setCursor(0, 1);
        lcd.print("RESTART?");
        delay(3000);
    }

    DateTime now = rtc.now();
    print_current_time(now, lcd);
    if (client.connected())
    {
        mqtt::publish_date_time(client, now);
    }
    show_time_delay.start(SHOW_TIME_DELAY);
    publish_time_delay.start(PUBLISH_DATETIME_DELAY);
}

void loop()
{
    DateTime now = rtc.now();
    if (show_time_delay.justFinished())
    {
        show_time_delay.repeat();
        print_current_time(now, lcd);
    }

    if (publish_time_delay.justFinished())
    {
        publish_time_delay.repeat();
        mqtt::publish_date_time(client, now);
    }
    
    // buzz if is alarm time
    if(is_alarm_time(now, alarms))
    {
        tone(BUZZER_PIN, 250);
    }
    else 
    {
        noTone(BUZZER_PIN);
    }

    int buttonPressed = digitalRead(BUTTON_PIN);
}