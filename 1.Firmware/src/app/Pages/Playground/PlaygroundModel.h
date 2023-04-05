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
    void SetPlaygroundMode(int16_t mode);
private:
    Account* account;
};

}

#endif
