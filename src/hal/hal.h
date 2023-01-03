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

    void knob_init();
    void knob_update(void);
    bool encoder_is_pushed(void);

    void motor_init(void);
    // void TaskMotorUpdate(void *pvParameters);
    int get_motor_position(void);
    void update_motor_mode(int mode);
    void motor_shake(int strength, int delay_time);
    
}


#endif