#pragma once

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <vector>

namespace mqtt
{
    bool connect(PubSubClient &client, String client_id);
    void subscribe(PubSubClient &client);
    void callback(char *topic, byte *payload, unsigned int length);

    void publish_date_time(PubSubClient &client, DateTime &now);
    void publish_alarm_list(PubSubClient &client);

    void process_set_time_message(StaticJsonDocument<256> &deserialized_message, PubSubClient &client);
    void process_set_date_message(StaticJsonDocument<256> &deserialized_message, PubSubClient &client);
    void process_set_alarm_message(StaticJsonDocument<256> &deserialized_message, PubSubClient &client);
}
