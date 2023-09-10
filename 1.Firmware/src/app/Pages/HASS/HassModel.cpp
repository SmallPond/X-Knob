#include "HassModel.h"
#include "app/Accounts/Account_Master.h"

using namespace Page;

HassModel::HassModel()
{
	app = APP_MODE_HOME_ASSISTANT;
}

void HassModel::GetKnobStatus(HassInfo* info)
{
	PlaygroundModel::GetKnobStatus(info);
}

void HassModel::Init()
{
	PlaygroundModel::Init();
}

void HassModel::Deinit()
{
	PlaygroundModel::Deinit();
}

void HassModel::ChangeMotorMode(int mode)
{
	PlaygroundModel::ChangeMotorMode(mode);
}

void HassModel::SetPlaygroundMode(int16_t mode)
{
	PlaygroundModel::SetPlaygroundMode(mode);
}