#include <Arduino.h>
#include "RGB.h"


void setup() {
    strip_init();
}

void loop() {
    unsigned long currentMillis = millis();
    strip_start(currentMillis);
}
