#ifndef __HAL_H__
#define __HAL_H__
#include "button_event.h"
#include "HAL_Def.h"
#include <Arduino.h>
#include "../config.h"

namespace HAL
{
    void Init();
    void Update();

    void knob_init(void (*button_handler)(ButtonEvent* btn, int event));
    void knob_update(void);
}


#endif