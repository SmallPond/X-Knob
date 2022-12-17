#ifndef __STRIP_H_
#define __STRIP_H_
#include <Adafruit_NeoPixel.h>
void strip1(void);
void strip2(void);
void strip3(void);
void rainbow1(void);
void rainbow2(void);
void role_fill();

void strip_init(void);
void strip_start(unsigned long currentMillis);
#endif