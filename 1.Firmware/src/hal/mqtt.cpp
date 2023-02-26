#if XK_MQTT
#include <Arduino.h>
#include "secrets.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include "config.h"
#include "hal/hal.h"

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);
int mqtt_last_connect_time_ = 0;


void setup_wifi() {
    Serial.printf("connecting to WiFi..: %s\n", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.printf("Connected to network %s\n", WIFI_SSID);


}

void mqttCallback(char *topic, byte *payload, unsigned int length) {
    char buf[256];
    snprintf(buf, sizeof(buf), "Received mqtt callback for topic %s, length %u", topic, length);
    Serial.printf(buf);
}

void connectMQTT() {

    if (WiFi.status() != WL_CONNECTED) {
        setup_wifi();
    }
    Serial.println("Attempting MQTT connection...");
#ifdef MQTT_PASSWORD 
    if (mqtt_client.connect(MQTT_USER, MQTT_USER, MQTT_PASSWORD)) {
#else 
    if (mqtt_client.connect(MQTT_USER)) {
#endif 
        Serial.printf("MQTT connected");
    } else {
        Serial.printf("MQTT failed rc=%d will try again in 5 seconds", mqtt_client.state());

    }

    Serial.flush();
}


TaskHandle_t handleTaskMqtt;
void TaskMqttUpdate(void *pvParameters) {

    while(1) {
        long now = millis();
        if (!mqtt_client.connected() && (now - mqtt_last_connect_time_) > 5000) {
            Serial.printf("Reconnecting MQTT");
            mqtt_last_connect_time_ = now;
            connectMQTT();
        }
        mqtt_client.loop();
        delay(5);
    }
}

int HAL::mqtt_subscribe(const char *topic)
{
    bool ret = mqtt_client.subscribe(topic);
    if (ret) {
        Serial.printf("Subscribe Error to topic:%s\n", topic);
    }
    return ret;
}

int HAL::mqtt_publish(const char *topic, const char *playload)
{
    bool ret = mqtt_client.publish(topic, playload);
    if (ret) {
        Serial.printf("Publish Error: topic:%s, playload:%s\n", topic, playload);
    }
    return ret;
}

void HAL::mqtt_init(void) {
    
    setup_wifi();
    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);
    // connectMQTT();
    mqtt_client.setCallback(mqttCallback);
    xTaskCreatePinnedToCore(
        TaskMqttUpdate,
        "MqttThread",
        4096,
        nullptr,
        2,
        &handleTaskMqtt,
        ESP32_RUNNING_CORE);
}
#endif