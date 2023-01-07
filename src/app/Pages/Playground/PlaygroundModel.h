#ifndef __PLAGROUND_MODEL_H
#define __PLAGROUND_MODEL_H
#include "App/Utils/AccountSystem/Account.h"
#include "lvgl.h"

namespace Page
{

class PlaygroundModel
{
public:
    void Init();
    void Deinit();
    void Update(void* pg_ui);
    void GetKnobStatus(int32_t *value, int32_t *motor_position);
private:
    Account* account;
};

}

#endif
