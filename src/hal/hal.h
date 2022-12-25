#ifndef __HAL_H__
#define __HAL_H__
#include "button_event.h"

void knob_init(void (*button_handler)(ButtonEvent* btn, int event));
void knob_update(void);

#endif