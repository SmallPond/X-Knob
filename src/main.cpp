#include <Arduino.h>
#include "RGB.h"
#include "app/app.h"
#include "usb_cdc.h"

void setup() {
    strip_init();
    HWSerial.begin(115200);
    usb_cdc_init();
    display_init();
}

void loop() {
    unsigned long currentMillis = millis();

    strip_start(currentMillis);
    display_run();
}
