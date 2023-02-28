#include "SurfaceDialModel.h"
#include "app/Accounts/Account_Master.h"
#include <Arduino.h>
#include "hal/motor.h"
#include "hal/hal.h"

using namespace Page;

SurfaceDialModel::SurfaceDialModel()
{
    app = APP_MODE_SUPER_DIAL;
}

void SurfaceDialModel::GetKnobStatus(PlaygroundInfo *info)
{
    PlaygroundModel::GetKnobStatus(info);
	info->is_ble_connected = HAL::surface_dial_is_connected();
}

void SurfaceDialModel::SetPlaygroundMode(int16_t mode)
{
    
}


void SurfaceDialModel::Init()
{
    PlaygroundModel::Init();
}

void SurfaceDialModel::Deinit()
{

}