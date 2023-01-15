
#include "hal.h"
#include <Arduino.h>
#include "config.h"

static volatile int16_t EncoderDiff = 0;

static ButtonEvent EncoderPush(2000);

/* 
 * lvgl task call it in 5ms
*/ 
bool HAL::encoder_is_pushed(void)
{
    // static int press_cnt = 0;
    // bool is_pushed = false;
    // if (digitalRead(PUSH_BUTTON_PIN) == LOW) {
    //     press_cnt++;
    //     if (press_cnt > 3) { // 10ms
    //         if( digitalRead(PUSH_BUTTON_PIN) == LOW){
    //             return true;
    //         }
    //     }
    // } else {
    //     press_cnt = 0;
    // }
    // return false;
    if (digitalRead(PUSH_BUTTON_PIN) == LOW) {
        // Serial.printf("Push button Pressed\n");
        return true;
    } 
    return false;
}

void HAL::knob_update(void)
{
    // EncoderPush.EventMonitor(encoder_is_pushed());
}


static void Encoder_PushHandler(ButtonEvent* btn, int event)
{
    
    if (event == ButtonEvent::EVENT_PRESSED)
    {
        Serial.printf("push is pused\n");
        // HAL::Buzz_Tone(500, 20);
        // EncoderDiffDisable = true;
        ;
    } else if (event == ButtonEvent::EVENT_RELEASED)
    {
        // HAL::Buzz_Tone(700, 20);
        // EncoderDiffDisable = false;
        ;
    } else if (event == ButtonEvent::EVENT_LONG_PRESSED)
    {
        // HAL::Audio_PlayMusic("Shutdown");
        // HAL::Power_Shutdown();
        ;
    }
}

void HAL::knob_init(void)
{
    pinMode(PUSH_BUTTON_PIN, INPUT_PULLUP);
    EncoderPush.EventAttach(Encoder_PushHandler);
    // attachInterrupt(CONFIG_ENCODER_A_PIN, Encoder_A_IrqHandler, CHANGE);
    // push_button.EventAttach(button_handler);

}