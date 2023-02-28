#ifndef __HOME_ASSISTANT_MODEL_H
#define __HOME_ASSISTANTL_MODEL_H
#include "app/Utils/AccountSystem/Account.h"
#include "lvgl.h"
#include "../Playground/PlaygroundView.h"
#include "../Playground/PlaygroundModel.h"

namespace Page
{

class HassModel: public PlaygroundModel
{
public:
    HassModel();
    void Init();
    void Deinit();
    void Update(void* pg_ui);
    void GetKnobStatus(PlaygroundInfo *info);
    void ChangeMotorMode(int mode);
    void SetPlaygroundMode(int16_t mode);

    void onEvent(Account* account, Account::EventParam_t* param);
private:
    Account* account;
};

}

#endif
