#include <Arduino.h>
#include "RGB.h"
#include "app/app.h"

void setup() {
    strip_init();
    display_init();
}

void loop() {
    unsigned long currentMillis = millis();
    strip_start(currentMillis);

    display_run();
}
