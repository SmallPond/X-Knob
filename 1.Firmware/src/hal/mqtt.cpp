
#include "hal/hal.h"

#if XK_MQTT

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "config.h"
#include "hal/nvs.h"

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);
int mqtt_last_connect_time = 0;


int setup_wifi() {
    int retry_cnt = 0;
    int max_retry_cnt = 10;
    String ssid,password;
    get_wifi_config(ssid,password);
    const char *wifi_name = ssid.c_str();  
    const char *wifi_pass = password.c_str(); 

    printf("connecting to WiFi..: %s\n", ssid);
    WiFi.begin(wifi_name, wifi_pass );
    
    while (WiFi.status() != WL_CONNECTED && retry_cnt < max_retry_cnt) {
        Serial.print(".");
        retry_cnt++;
        delay(100);
    }

   if (WiFi.status() == WL_CONNECTED) {
        printf("Connected to network %s\n", ssid);
   } else {
        printf("WiFi connection timeout: %s\n", ssid);
        return -1;
   }
   return 0;
}

void mqttCallback(char *topic, byte *payload, unsigned int length) {
    char buf[256];
    snprintf(buf, sizeof(buf), "Received mqtt callback for topic %s, length %u", topic, length);
    Serial.printf(buf);
}

void connectMQTT() {
    String password,host,username,topic;
    uint16_t port;
    get_mqtt_config(host,port,username,password,topic);
    const char *mqtt_user = username.c_str();  
    const char *mqtt_pass = password.c_str();
    bool result;
    Serial.println("Attempting MQTT connection...");

    if (WiFi.status() != WL_CONNECTED) {
        if (setup_wifi()) {
            return;
        }
    }

    if(password.length() > 0){
        result = mqtt_client.connect(mqtt_user, mqtt_user, mqtt_pass);
    } else {
        result = mqtt_client.connect(mqtt_user);
    }

    if (!result) {
        printf("MQTT failed rc=%d will try again in 5 seconds\n", mqtt_client.state());
    }
}


TaskHandle_t handleTaskMqtt;
void TaskMqttUpdate(void *pvParameters) {

    while(1) {
        long now = millis();
        if (!mqtt_client.connected() && (now - mqtt_last_connect_time) > 5000) {
            Serial.printf("Reconnecting MQTT");
            mqtt_last_connect_time = now;
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

char mqtt_host[50];

void HAL::mqtt_init(void) {
    String password,host,username,topic;
    uint16_t port;

    if (WiFi.status() != WL_CONNECTED) {
        setup_wifi();
    }
    
    get_mqtt_config(host,port,username,password,topic);
    sprintf(mqtt_host, "%s" , host.c_str());
    #if DEBUG_PRINT
        printf("MQTT connect host |%s|:|%d|\n" , mqtt_host , port );
    #endif
    mqtt_client.setServer(mqtt_host, port);

    mqtt_client.setCallback(mqttCallback);
    xTaskCreatePinnedToCore(
        TaskMqttUpdate,
        "MqttThread",
        4096,
        nullptr,
        2,
        &handleTaskMqtt,
        ESP32_RUNNING_CORE
    );
}
#else 
void HAL::mqtt_init(void) {

}

int HAL::mqtt_publish(const char *topic, const char *playload) {
    return 0;
}

int HAL::mqtt_subscribe(const char *topic){

}
#endif