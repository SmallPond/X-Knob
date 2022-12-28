#ifndef __HAL_H__
#define __HAL_H__
#include "button_event.h"
#include "HAL_Def.h"
#include <Arduino.h>
#include "config.h"
#include "CommonMacro.h"
namespace HAL
{
    void Init();
    void Update();

    void knob_init(void (*button_handler)(ButtonEvent* btn, int event));
    void knob_update(void);

    void motor_init(void);
    void motor_update(void);
}


#endif