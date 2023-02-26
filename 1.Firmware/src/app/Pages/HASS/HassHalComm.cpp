#include "HassHalComm.h"
#include "hal/hal.h"
#include "config.h"

const char* playload_str[] {
    [SUPER_DIAL_NULL] = "NULL",
    [HASS_LEFT] = "LEFT",
    [HASS_RIGHT] = "RIGHT",
    [HASS_PUSH]  = "ON/OFF"
};

void hass_hal_init(void)
{
    HAL::mqtt_init();
}

int hass_hal_send(const char *device_name, int knob_value)
{
    char topic_name[128];

    snprintf(topic_name, sizeof(topic_name),"%s/HOME/%s", MQTT_HOST, device_name);
    if (knob_value < HASS_MAX && playload_str[knob_value] != NULL) {
        printf("mqtt send: %s:%s\n", topic_name, playload_str[knob_value]);
        return HAL::mqtt_publish(topic_name, playload_str[knob_value]);
    } else {
        printf("hass_hal_send: xknob_value is invalid\n");
        return -1;
    }
    return 0;
}