
#include "hal.h"
#include <Arduino.h>
#include "config.h"

static volatile int16_t EncoderDiff = 0;

static ButtonEvent EncoderPush(2000);
static int push_state = 0;

bool HAL::push_button_is_pushed(void)
{
    if (push_state == ButtonEvent::EVENT_PRESSED) {
        Serial.printf("push button pused\n");
        return true;

    } else {
        return false;
    }
}

void HAL::knob_update(void)
{
    // EncoderPush.EventMonitor(push_button_is_pushed());
}


static void Encoder_PushHandler(ButtonEvent* btn, int event)
{
    push_state = event;;
    if (event == ButtonEvent::EVENT_PRESSED)
    {
        
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