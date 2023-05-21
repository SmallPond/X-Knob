#ifndef __EEPROM_H
#define __EEPROM_H


//#include <EEPROM.h>
#include <Preferences.h>
#include "config.h"


#define CONFIG_NAMESPACE         "config"
#define INIT_KEY                 "init"
#define FFAT_KEY                 "ffat"
#define LCD_BK_BRIGHTNESS_KEY    "lcd_bk"
#define LCD_BK_TIME_OUT_KEY      "lcd_time"
#define WIFI_SSID_KEY            "wifi_ssid"
#define WIFI_PASSWORD_KEY        "wifi_password"
#define MQTT_HOST_KEY            "mqtt_host"
#define MQTT_PORT_KEY            "mqtt_port"
#define MQTT_USERNAME_KEY        "mqtt_username"
#define MQTT_PASSWORD_KEY        "mqtt_password"
#define MQTT_TPOIC_KEY           "mqtt_topic"

typedef struct {
    bool init_ffat_flag;
    String wifi_ssid;
    String wifi_password;
    String mqtt_host;
    String mqtt_username;
    String mqtt_password;
    int mqtt_port;
    String mqtt_topic;
    int lcd_bk_timeout;
    int lcd_bk_brightness;
} NvsConfig;

void nvs_init();
uint8_t get_init_ffat();
void set_init_ffat(uint8_t value);
uint16_t get_lcd_bk_brightness();
void set_lcd_bk_brightness(uint16_t value);
uint16_t get_lcd_bk_timeout();
void set_lcd_bk_timeout(uint16_t value);
void get_wifi_config(String &ssid,String &password);
void set_wifi_config(String ssid,String password);
void get_mqtt_config(String &host,uint16_t &port,String &username,String &password,String &topic);
void set_mqtt_config(String host,uint16_t port,String username,String password,String topic);

#endif