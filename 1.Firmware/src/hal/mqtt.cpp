
#include "hal/hal.h"

#if XK_MQTT

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "config.h"
#include "hal/nvs.h"

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);
int mqtt_last_connect_time_ = 0;


void setup_wifi() {
    String ssid,password;
    get_wifi_config(ssid,password);
    #if DEBUG_PRINT
        printf("connecting to WiFi..: %s\n", ssid);
    #endif
    const char *wifi_name = ssid.c_str();  //去掉const會顯示編譯錯誤
    const char *wifi_pass = password.c_str();  //去掉const會顯示編譯錯誤
    WiFi.begin(wifi_name, wifi_pass );
    
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    #if DEBUG_PRINT
        printf("Connected to network %s\n", ssid);
    #endif
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
    String password,host,username,topic;
    uint16_t port;
    get_mqtt_config(host,port,username,password,topic);
    const char *mqtt_user = username.c_str();  //去掉const會顯示編譯錯誤
    const char *mqtt_pass = password.c_str();  //去掉const會顯示編譯錯誤

    Serial.println("Attempting MQTT connection...");
    bool result;

    if(password.length() > 0){
        #if DEBUG_PRINT
            printf("MQTT connect with password |%s| , |%s|\n" , mqtt_user , mqtt_pass );
        #endif
        result = mqtt_client.connect(mqtt_user, mqtt_user, mqtt_pass);
    }else{
        #if DEBUG_PRINT
            printf("MQTT connect |%s| \n" , mqtt_user );
        #endif
        result = mqtt_client.connect(mqtt_user);
    }
    #if DEBUG_PRINT
    if (result) {
        printf("MQTT connected\n");
    } else {
        printf("MQTT failed rc=%d will try again in 5 seconds\n", mqtt_client.state());
    }
    #endif

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

char mqtt_host[50];

void HAL::mqtt_init(void) {
    
    setup_wifi();
    String password,host,username,topic;
    uint16_t port;
    get_mqtt_config(host,port,username,password,topic);
    sprintf(mqtt_host , "%s" , host.c_str());
    #if DEBUG_PRINT
        printf("MQTT connect host |%s|:|%d|\n" , mqtt_host , port );
    #endif
    mqtt_client.setServer(mqtt_host, port);
    // connectMQTT();
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