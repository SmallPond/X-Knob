#include "HassModel.h"
#include "app/Accounts/Account_Master.h"
#include <Arduino.h>
#include "hal/motor.h"
#include "hal/hal.h"

using namespace Page;

HassModel::HassModel()
{
    app = APP_MODE_HOME_ASSISTANT;
}

void HassModel::GetKnobStatus(PlaygroundInfo *info)
{
    PlaygroundModel::GetKnobStatus(info);
	info->is_ble_connected = HAL::surface_dial_is_connected();
}


void HassModel::Init()
{
    PlaygroundModel::Init();
}

void HassModel::Deinit()
{
    PlaygroundModel::Deinit();
}