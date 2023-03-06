#include "mqttconnection.h"
#include "utility.h"
#include "constants.h"

/*
fuction to connect to mqtt broker, with 10 tries, return true if connect successfully, else false
*/
bool mqtt::connect(PubSubClient &client, String client_id)
{
    int tries = 0;

    client_id += String(WiFi.macAddress());

    client.setServer(MQTT_BROKER, MQTT_PORT);
    client.setCallback(callback);

    while (!client.connected())
    {
        tries += 1;
        Serial.printf("The client %s trying to connect to mqtt broker\n", client_id.c_str());

        if (client.connect(client_id.c_str()))
        {
            Serial.println("Connected to broker\n");
            return true;
        }
        else
        {
            Serial.printf("MQTT Connection failed with state: %d\n", client.state());
            delay(1000);
        }

        if (tries > 10)
            return false;
    }
}

/*
    subscribe to topics
*/
void mqtt::subscribe(PubSubClient &client)
{
    client.subscribe(MQTT_TOPIC_SET_TIME);
    client.subscribe(MQTT_TOPIC_SET_DATE);
    client.subscribe(MQTT_TOPIC_SET_ALARM);
}

/*
    mqtt callback: what to do depends on message received
*/
void mqtt::callback(char *topic, byte *payload, unsigned int length)
{
    Serial.println("Msg arrived: @ " + String(topic) + ' ' + String((char *)payload));
    StaticJsonDocument<256> deserialized_message;
    deserializeJson(deserialized_message, payload);

    if (String(topic) == MQTT_TOPIC_SET_TIME)
    {
        //process_set_time_message(deserialized_message, client);
    }
    else if (String(topic) == MQTT_TOPIC_SET_DATE)
    {
        //process_set_date_message(deserialized_message, client);
    }
    else if (String(topic) == MQTT_TOPIC_SET_ALARM)
    {
        //process_set_alarm_message(deserialized_message, client);
    }
}

/*
    publish date, time to broker
*/
void mqtt::publish_date_time(PubSubClient &client, DateTime &now)
{
    String msg;

    parse_time(now, msg);
    client.publish(MQTT_TOPIC_TIME, msg.c_str(), true);

    parse_date(now, msg);
    client.publish(MQTT_TOPIC_DATE, msg.c_str(), true);
}
