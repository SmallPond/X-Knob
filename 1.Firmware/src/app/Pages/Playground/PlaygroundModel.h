#ifndef __PLAGROUND_MODEL_H
#define __PLAGROUND_MODEL_H
#include "app/Utils/AccountSystem/Account.h"
#include "lvgl.h"
#include "hal/hal.h"

typedef struct
{
    int32_t xkonb_value;
    int32_t motor_pos;
    int32_t angle_offset;
    SuperDialMotion konb_direction;
} PlaygroundInfo;
namespace Page
{

class PlaygroundModel
{
public:
    int16_t app;
    void Init();
    void Deinit();
    virtual void GetKnobStatus(PlaygroundInfo *info);
    void ChangeMotorMode(int mode);
    void ChangeMotorModeWithInitPosition(int mode, int init_position);
    void SetPlaygroundMode(int16_t mode);
    void SetLcdBkBrightness(int value);
    static int32_t MAX_VALUE;
    static int32_t MIN_VALUE;
    static int32_t knob_value;
private:
    Account* account;
};

}

#endif
