#ifndef __MENU_MODEL_H
#define __MENU_MODEL_H

#include "app/Accounts/Account_Master.h"

namespace Page
{

class MenuModel
{
public:
    void Init();
    void Deinit();

    void GetIMUInfo(
        char* info, uint32_t len
    );

    void GetBatteryInfo(
        int* usage,
        float* voltage,
        char* state, uint32_t len
    );

    void ChangeMotorMode(int mode);

private:
    Account* account;

private:

};

}

#endif
