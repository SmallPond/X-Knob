#ifndef __PLAGROUND_MODEL_H
#define __PLAGROUND_MODEL_H
#include "App/Utils/AccountSystem/Account.h"
#include "lvgl.h"
#include "PlaygroundView.h"
namespace Page
{

class PlaygroundModel
{
public:
    void Init();
    void Deinit();
    void Update(void* pg_ui);
    void GetKnobStatus(PlaygroundMotorInfo *info);
    void ChangeMotorMode(int mode);
    void SetPlaygroundMode(int16_t mode);
    int16_t playgroundMode;
private:
    Account* account;
};

}

#endif
