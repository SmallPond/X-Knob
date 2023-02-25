#include "hal.h"
#include <Arduino.h>
#include "config.h"
#include <driver/rtc_io.h>

void HAL::power_init(void) 
{
    pinMode(BATTERY_OFF_PIN, OUTPUT);
    digitalWrite(BATTERY_OFF_PIN, LOW);  //

    pinMode(ON_OFF_PIN, OUTPUT);
    digitalWrite(ON_OFF_PIN, HIGH);
}

void HAL::power_off(void) 
{
    /* 
     * ___-__----..
     * MT3608: a high input at EN turns on the conveter
     * low input turns it off 
    */ 
    digitalWrite(BATTERY_OFF_PIN, HIGH);
    delay(100);
    digitalWrite(BATTERY_OFF_PIN, LOW);
    delay(200);
    digitalWrite(BATTERY_OFF_PIN, HIGH);

    // keep PUSH_PIN HIGH level in deep sleep mode  
    rtc_gpio_init((gpio_num_t)PUSH_BUTTON_PIN);
    rtc_gpio_pullup_en((gpio_num_t)PUSH_BUTTON_PIN);
    rtc_gpio_pulldown_dis((gpio_num_t)PUSH_BUTTON_PIN);
    gpio_deep_sleep_hold_en();
    // low level will trigger wakeup
    esp_sleep_enable_ext0_wakeup((gpio_num_t)PUSH_BUTTON_PIN, 0);
    esp_deep_sleep_start();
}