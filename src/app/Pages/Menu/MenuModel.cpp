#include "MenuModel.h"
#include <stdio.h>
#include <Arduino.h>
using namespace Page;

void MenuModel::Init()
{
    account = new Account("MenuModel", AccountSystem::Broker(), 0, this);
    account->Subscribe("Motor");
    // account->Subscribe("IMU");
    // account->Subscribe("Power");
    // account->Subscribe("Storage");
}

void MenuModel::Deinit()
{
    if (account)
    {
        delete account;
        account = nullptr;
    }
}


// void MenuModel::GetBatteryInfo(
//     int* usage,
//     float* voltage,
//     char* state, uint32_t len
// )
// {
//     HAL::Power_Info_t power;
//     account->Pull("Power", &power, sizeof(power));
//     *usage = power.usage;
//     *voltage = power.voltage / 1000.0f;
//     strncpy(state, power.isCharging ? "CHARGE" : "DISCHARGE", len);
// }

// void MenuModel::GetStorageInfo(
//     bool* detect,
//     char* usage, uint32_t len
// )
// {
//     AccountSystem::Storage_Basic_Info_t info;
//     account->Pull("Storage", &info, sizeof(info));
//     *detect = info.isDetect;
//     snprintf(
//         usage, len,
//         "%0.1f GB",
//         info.totalSizeMB / 1024.0f
//     );
// }


void MenuModel::ChangeMotorMode(int mode)
{
    Serial.printf("MenuModel: Change Motor Mode\n");
    AccountSystem::Motor_Info_t info;
    info.cmd = AccountSystem::MOTOR_CMD_CHANGE_MODE;
    info.motor_mode = mode;
    // 第一个参数是通知发布者，即本 Account 应该 subscribe 第一个参数指向的 Account 
    account->Notify("Motor", &info, sizeof(info));  
}
