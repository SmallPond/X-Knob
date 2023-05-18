#include "nvs.h"
#include "config.h"
#include "secrets.h"


#define INIT_VALUE 0xdd

NvsConfig ncv_config;

void nvs_init(){
    Preferences prefs;     // 聲明Preferences對象
    prefs.begin(CONFIG_NAMESPACE);  // 打開命名空間mynamespace
    uint8_t value = prefs.getUChar(INIT_KEY, 0);
    if(value != INIT_VALUE){
        prefs.putUChar(FFAT_KEY, 0);
        set_lcd_bk_brightness(LCD_BK_DEFAULT_BRIGHTNESS); //default lcd bk
        set_lcd_bk_timeout(LCD_BK_DEFAULT_TIMEOUT); //default lcd bk time out 5mins
        set_wifi_config(WIFI_SSID, WIFI_PASSWORD);
        set_mqtt_config(MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD, MQTT_HOST );
        set_init_ffat(0);
        prefs.putUChar(INIT_KEY, INIT_VALUE);
    }
    ncv_config.init_ffat_flag = prefs.getUChar(FFAT_KEY, 0);
    //lcd
    ncv_config.lcd_bk_timeout =  prefs.getUShort(LCD_BK_TIME_OUT_KEY, 0);
    ncv_config.lcd_bk_brightness =  prefs.getUShort(LCD_BK_BRIGHTNESS_KEY, 0);
    //wifi
    ncv_config.wifi_ssid = prefs.getString(WIFI_SSID_KEY, "");
    ncv_config.wifi_password = prefs.getString(WIFI_PASSWORD_KEY, "");
    //mqtt
    ncv_config.mqtt_host = prefs.getString(MQTT_HOST_KEY, "");
    ncv_config.mqtt_port = prefs.getUInt(MQTT_PORT_KEY, 0);
    ncv_config.mqtt_username = prefs.getString(MQTT_USERNAME_KEY, "");
    ncv_config.mqtt_password = prefs.getString(MQTT_PASSWORD_KEY, "");
    ncv_config.mqtt_topic = prefs.getString(MQTT_TPOIC_KEY, "");
    #if DEBUG_PRINT
        printf("get FFAT_KEY Config: %d\n", ncv_config.init_ffat_flag );
        printf("get LCD_BK_TIME_OUT_KEY Config: %d\n", ncv_config.lcd_bk_timeout );
        printf("get LCD_BK_BRIGHTNESS_KEY Config: %d\n", ncv_config.lcd_bk_brightness );
        printf("get WIFI_SSID_KEY Config: %s\n", ncv_config.wifi_ssid );
        printf("get WIFI_PASSWORD_KEY Config: %s\n", ncv_config.wifi_password );
        printf("get MQTT_HOST_KEY Config: %s\n", ncv_config.mqtt_host );
        printf("get MQTT_PORT_KEY Config: %d\n", ncv_config.mqtt_port );
        printf("get MQTT_USERNAME_KEY Config: %s\n", ncv_config.mqtt_username );
        printf("get MQTT_PASSWORD_KEY Config: %s\n", ncv_config.mqtt_password );
        printf("get MQTT_TPOIC_KEY Config: %s\n", ncv_config.mqtt_topic );
    #endif
    prefs.end();
}

uint8_t get_init_ffat(){
    #if DEBUG_PRINT
        printf("get FFAT_KEY Config: %d\n", ncv_config.init_ffat_flag );
    #endif
    return ncv_config.init_ffat_flag;
}

uint16_t get_lcd_bk_brightness(){
    #if DEBUG_PRINT
        //printf("get LCD_BK_BRIGHTNESS_KEY Config: %d\n", ncv_config.lcd_bk_brightness );
    #endif
    return ncv_config.lcd_bk_brightness;
}

uint16_t get_lcd_bk_timeout(){
    #if DEBUG_PRINT
        printf("get LCD_BK_TIME_OUT_KEY Config: %d\n", ncv_config.lcd_bk_timeout );
    #endif
    return ncv_config.lcd_bk_timeout;
}

void get_wifi_config(String &ssid,String &password){
    ssid = ncv_config.wifi_ssid;
    password = ncv_config.wifi_password;
    #if DEBUG_PRINT
        printf("get WIFI_SSID_KEY Config: %s\n", ncv_config.wifi_ssid.c_str() );
        printf("get WIFI_PASSWORD_KEY Config: %s\n", ncv_config.wifi_password.c_str() );
    #endif
}

void get_mqtt_config(String &host,uint16_t &port,String &username,String &password,String &topic){
    host =  ncv_config.mqtt_host;
    port = ncv_config.mqtt_port;
    username = ncv_config.mqtt_username;
    password = ncv_config.mqtt_password;
    topic = ncv_config.mqtt_topic;
    #if DEBUG_PRINT
        printf("get MQTT_HOST_KEY Config: %s\n", ncv_config.mqtt_host.c_str() );
        printf("get MQTT_PORT_KEY Config: %d\n", ncv_config.mqtt_port );
        printf("get MQTT_USERNAME_KEY Config: %s\n", ncv_config.mqtt_username.c_str() );
        printf("get MQTT_PASSWORD_KEY Config: %s\n", ncv_config.mqtt_password.c_str() );
        printf("get MQTT_TPOIC_KEY Config: %s\n", ncv_config.mqtt_topic.c_str() );
    #endif
}

void set_init_ffat(uint8_t value){
    Preferences prefs; 
    prefs.begin(CONFIG_NAMESPACE);  
    prefs.putUChar(FFAT_KEY, value);
    prefs.end();
    ncv_config.init_ffat_flag = value;
    #if DEBUG_PRINT
        printf("set FFAT_KEY Config: %d\n", value );
    #endif
}

void set_lcd_bk_brightness(uint16_t value){
    Preferences prefs;     
    prefs.begin(CONFIG_NAMESPACE); 
    prefs.putUShort(LCD_BK_BRIGHTNESS_KEY, value);
    prefs.end();
    ncv_config.lcd_bk_brightness = value;
    #if DEBUG_PRINT
        printf("set LCD_BK_BRIGHTNESS_KEY Config: %d\n", value );
    #endif
}

void set_lcd_bk_timeout(uint16_t value){
    Preferences prefs;     
    prefs.begin(CONFIG_NAMESPACE); 
    prefs.putUShort(LCD_BK_TIME_OUT_KEY, value);
    prefs.end();
    ncv_config.lcd_bk_timeout = value;
    #if DEBUG_PRINT
        printf("set LCD_BK_TIME_OUT_KEY Config: %d\n", value );
    #endif
}

void set_wifi_config(String ssid,String password){
    Preferences prefs;     
    prefs.begin(CONFIG_NAMESPACE); 
    prefs.putString(WIFI_SSID_KEY, ssid);
    prefs.putString(WIFI_PASSWORD_KEY, password);
    prefs.end();
    ncv_config.wifi_ssid = ssid;
    ncv_config.wifi_password = password;
    #if DEBUG_PRINT
        printf("set WIFI_SSID_KEY Config: %s\n", ssid.c_str() );
        printf("set WIFI_PASSWORD_KEY Config: %s\n", password.c_str() );
    #endif
}

void set_mqtt_config(String host,uint16_t port,String username,String password,String topic){
    Preferences prefs;     
    prefs.begin(CONFIG_NAMESPACE); 
    prefs.putString(MQTT_HOST_KEY, host);
    prefs.putUInt(MQTT_PORT_KEY, port);
    prefs.putString(MQTT_USERNAME_KEY, username);
    prefs.putString(MQTT_PASSWORD_KEY, password);
    prefs.putString(MQTT_TPOIC_KEY, topic);
    prefs.end();
    ncv_config.mqtt_host = host;
    ncv_config.mqtt_port = port;
    ncv_config.mqtt_username = username;
    ncv_config.mqtt_password = password;
    ncv_config.mqtt_topic = topic;
    #if DEBUG_PRINT
        printf("set MQTT_HOST_KEY Config: %s\n", host.c_str() );
        printf("set MQTT_PORT_KEY Config: %d\n", port );
        printf("set MQTT_USERNAME_KEY Config: %s\n", username.c_str() );
        printf("set MQTT_PASSWORD_KEY Config: %s\n", password.c_str() );
        printf("set MQTT_TPOIC_KEY Config: %s\n", topic.c_str() );
    #endif
}
