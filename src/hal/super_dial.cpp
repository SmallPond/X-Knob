#include "hal/hal.h"
#include "BleKeyboard.h"

BleKeyboard bleKeyboard;



void HAL::super_dial_init(void)
{
    bleKeyboard.begin();
}

void HAL::super_dial_update(SuperDialMotion direction)
{
    if (bleKeyboard.isConnected()) {

        switch (direction)
        {
        case SUPER_DIAL_LEFT:
            bleKeyboard.sendDialReport(DIAL_L);
            break;
        case SUPER_DIAL_RIGHT:
            bleKeyboard.sendDialReport(DIAL_R);
            break;
        default:
            Serial.println("SuperDial: Error direction parameter");
            break;
        }
    }
}