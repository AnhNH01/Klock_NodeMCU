#pragma once


#include <millisDelay.h>
#include <Arduino.h>


// Mqtt constanst
#define MQTT_BROKER "192.168.82.115"
#define MQTT_PORT 1883

#define MQTT_TOPIC_SET_TIME "clock/time/set"
#define MQTT_TOPIC_SET_DATE "clock/date/set"
#define MQTT_TOPIC_SET_ALARM "clock/alarm/set"
#define MQTT_TOPIC_ALARM "clock/alarm"
#define MQTT_TOPIC_TIME "clock/time"
#define MQTT_TOPIC_DATE "clock/date"

// Wifi
#define SSID "Hai Anh"
#define PWD "67750105"

// Lcd
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// nodeMCU pins
#define BUTTON_PIN D5
#define BUZZER_PIN D6


// delays
const unsigned long SHOW_TIME_DELAY = 1000;
const unsigned long PUBLISH_DATETIME_DELAY = 15000;

