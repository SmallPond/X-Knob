#include <Arduino.h>
#include "RGB.h"
#include "app/app.h"
#include "usb_cdc.h"
#include "app/ui/ui.h"

void setup() {
    strip_init();
    HWSerial.begin(115200);
    usb_cdc_init();
    display_init();
    ui_init();

    INIT_DONE();
}

void loop() {
    unsigned long currentMillis = millis();

    strip_start(currentMillis);
}
