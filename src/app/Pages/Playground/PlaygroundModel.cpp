#include "PlaygroundModel.h"
#include "App/Accounts/Account_Master.h"
#include <Arduino.h>
#include "hal/motor.h"
using namespace Page;
static int32_t knob_value = 50;
static int now_pos = 0;
static int last_pos = 0;
static bool is_outbound = false;
static int32_t arc_offset = 0;   // 超出界限时显示的 arch 长度
static int32_t MAX_VALUE = 100;
static int32_t MIN_VALUE = 0;

void PlaygroundModel::GetKnobStatus(PlaygroundMotorInfo *info)
{
    info->xkonb_value = knob_value;
    info->motor_pos = now_pos;
    info->angle_offset = arc_offset;
}

void PlaygroundModel::SetPlaygroundMode(int16_t mode)
{
    playgroundMode = mode;
	switch (playgroundMode)
	{
	case PLAYGROUND_MODE_FINE_DETENTS:
	    // This mode is default
        MAX_VALUE = 100;
        MIN_VALUE = 0;

		break;
	case PLAYGROUND_MODE_BOUND:
	    MAX_VALUE = 12;
        MIN_VALUE = 0;
        knob_value = 0;
        break;
    case PLAYGROUND_MODE_ON_OFF:
	    MAX_VALUE = 1;
        MIN_VALUE = 0;
        knob_value = 0;
        break;
	default:
		break;
	}

}

void PlaygroundModel::ChangeMotorMode(int mode)
{
    Serial.printf("MenuModel: Change Motor Mode\n");
    AccountSystem::Motor_Info_t info;
    info.cmd = AccountSystem::MOTOR_CMD_CHANGE_MODE;
    info.motor_mode = mode;
    // 第一个参数是通知发布者，即本 Account 应该 subscribe 第一个参数指向的 Account 
    account->Notify("Motor", &info, sizeof(info));  
}

static int onEvent(Account* account, Account::EventParam_t* param)
{
    MotorStatusInfo *info = (MotorStatusInfo*) (param->data_p);
    
    now_pos = info->position;
    is_outbound = info->is_outbound;

    if (is_outbound) {
        arc_offset = (int)(info->angle_offset);
    } else {
        arc_offset = 0;
    }
    Serial.printf("now_pos:%d, last_pos: %d knob_value:%d, arch offset: %f\n", 
            now_pos, last_pos, knob_value, info->angle_offset);
    if (now_pos > last_pos)
    {
        knob_value++;
        if (knob_value > MAX_VALUE) {
            knob_value = MAX_VALUE;
        }
        last_pos = now_pos;
    }
    else if (now_pos < last_pos)
    {
        knob_value--;
        if (knob_value < MIN_VALUE) {
            knob_value = MIN_VALUE;
        }
        last_pos = now_pos;
    }

    return 0;
}

void PlaygroundModel::Init()
{
    knob_value = 0;
    account = new Account("PlaygroundModel", AccountSystem::Broker(), 0, this);

    account->SetEventCallback(onEvent);
    account->Subscribe("MotorStatus");
    account->Subscribe("Motor");
    playgroundMode = 0;
    
}

void PlaygroundModel::Deinit()
{
    if (account)
    {
        delete account;
        account = nullptr;
    }
}