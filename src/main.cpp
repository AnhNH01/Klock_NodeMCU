#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_I2CDevice.h>
#include "RTClib.h"
#include "Utils.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <vector>

#define MQTT_BROKER "192.168.1.15"
// #define MQTT_BROKER "172.20.10.3"
#define MQTT_PORT 1883
// #define SSID "Haianh's phone"
// #define PWD "haianh123"
#define SSID "Hai Anh"
#define PWD "67750105"
#define LCD_COLUMNS 16
#define LCD_ROWS 2

#define MQTT_TOPIC_SET_TIME "clock/time/set"
#define MQTT_TOPIC_SET_DATE "clock/date/set"
#define MQTT_TOPIC_SET_ALARM "clock/alarm/set"
#define MQTT_TOPIC_ALARM "clock/alarm"

RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, LCD_COLUMNS, LCD_ROWS);
WiFiClient espClient;
PubSubClient client(espClient);
String clientId = "ESP@";

void mqttConnection();
void callBack(char *topic, byte *payload, unsigned int length);

LiquidCrystal_I2C *plcd;

std::vector<Alarm> alarms;

// Problem: when sending a list of alarm, specify capacity to prevent incorrecly parsed data in android app --> leads to err

void setup()
{
  Serial.begin(9600);
  WiFi.mode(WIFI_AP_STA);
  initLcd(lcd);
  lcd.setCursor(0, 0);
  lcd.print("SETTING UP");
  initRtc(rtc, lcd);
  initAlarms(alarms);
  mqttConnection();
}

void loop()
{
  DateTime now = rtc.now();
  currentTime(now, lcd);
  delay(1000);
  client.loop();
}

void mqttConnection()
{
  int tries = 0;
  WiFi.begin(SSID, PWD);
  while (WiFi.status() != WL_CONNECTED)
  {
    tries++;
    delay(500);
    Serial.println("Connecting to WiFi..");
    if (tries > 30)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("NO WIFI");
      lcd.setCursor(0, 1);
      lcd.print("RESTART?");
      delay(3000);
      return;
    }
  }
  tries = 0;

  client.setServer(MQTT_BROKER, MQTT_PORT);
  clientId += String(WiFi.macAddress());
  client.setCallback(callBack);
  // set callback

  while (!client.connected())
  {
    tries++;
    Serial.printf("The client %s trying to connect to mqtt broker\n", clientId.c_str());
    if (client.connect(clientId.c_str()))
      Serial.println("Connected to broker\n");
    else
    {
      Serial.printf("MQTT Connection failed with state: %d\n", client.state());
      delay(1000);
    }
    if (tries > 10)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("BROKER ERROR");
      lcd.setCursor(0, 1);
      lcd.print("RESTART?");
      delay(3000);
      return;
    }
  }

  client.subscribe(MQTT_TOPIC_SET_TIME);
  client.subscribe(MQTT_TOPIC_SET_DATE);
  client.subscribe(MQTT_TOPIC_SET_ALARM);

  client.publish("test/hello", "Hello from esp8266");
}

void callBack(char *topic, byte *payload, unsigned int length)
{
  Serial.println("Msg arrived: @ " + String(topic) + ' ' + String((char *)payload));
  StaticJsonDocument<256> jsonDoc;
  deserializeJson(jsonDoc, payload);
  if (String(topic) == String(MQTT_TOPIC_SET_TIME))
  {
    DateTime now = rtc.now();
    int hour = jsonDoc["hour"];
    int minute = jsonDoc["minute"];
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), hour, minute, now.second()));
  }
  else if (String(topic) == String(MQTT_TOPIC_SET_DATE))
  {
    DateTime now = rtc.now();
    int day = jsonDoc["day"];
    int month = jsonDoc["month"];
    int year = jsonDoc["year"];
    rtc.adjust(DateTime(year, month, day, now.hour(), now.minute(), now.second()));
  }
  else if (String(topic) == String(MQTT_TOPIC_SET_ALARM))
  {
    payload[length] = '\0';
    String msg = String((char *)payload);
    if (msg == String("GET"))
    {
      String returnMsg = "";
      parseListAlarm(alarms, returnMsg);
      client.publish(MQTT_TOPIC_ALARM, returnMsg.c_str(), false);
    }
    else
    {
      int alarmId = jsonDoc["id"];
      int alarmHour = jsonDoc["hour"];
      int alarmMinute = jsonDoc["minute"];
      int alarmState = jsonDoc["state"];

      bool add = true;
      for (auto &a : alarms)
      {
        if (a.id == alarmId)
        {
          Serial.println("Update");
          a.hour = alarmHour;
          a.minute = alarmMinute;
          a.state = alarmState;
          add = false;
          break;
        }
      }
      if (add)
        alarms.emplace_back(Alarm(alarmId, alarmHour, alarmMinute, alarmState));

      String returnMsg = "";

      parseListAlarm(alarms, returnMsg);
      Serial.println(returnMsg);
      client.publish(MQTT_TOPIC_ALARM, returnMsg.c_str(), false);
    }
  }
}
