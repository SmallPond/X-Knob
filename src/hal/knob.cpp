
#include "hal.h"
#include <Arduino.h>
#include "config.h"

static ButtonEvent push_button(500);



static bool push_button_is_pushed(void)
{
    return (digitalRead(PUSH_BUTTON_PIN) == LOW);
}

void knob_update(void)
{
    push_button.EventMonitor(push_button_is_pushed());
}

void knob_init(void (*button_handler)(ButtonEvent* btn, int event))
{
    pinMode(PUSH_BUTTON_PIN, INPUT_PULLUP);

    // attachInterrupt(CONFIG_ENCODER_A_PIN, Encoder_A_IrqHandler, CHANGE);
    push_button.EventAttach(button_handler);

}