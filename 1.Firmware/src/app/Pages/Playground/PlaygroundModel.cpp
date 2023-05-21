#include "PlaygroundModel.h"
#include "app/Accounts/Account_Master.h"
#include "hal/motor.h"
#include "hal/hal.h"
#include "hal/nvs.h"
#include "Playground.h"
using namespace Page;


bool is_outbound = false;
int32_t arc_offset = 0;   // 超出界限时显示的 arch 长度
SuperDialMotion konb_direction = SUPER_DIAL_NULL;
int now_pos = 0;
int last_pos = 0;
int PlaygroundModel::MAX_VALUE = 100;
int PlaygroundModel::MIN_VALUE = 0;
int PlaygroundModel::knob_value = 50;

void PlaygroundModel::GetKnobStatus(PlaygroundInfo *info)
{
    info->xkonb_value = knob_value;
    info->motor_pos = now_pos;
    info->angle_offset = arc_offset;
    info->konb_direction = konb_direction;
    konb_direction = SUPER_DIAL_NULL;
}

void PlaygroundModel::SetPlaygroundMode(int16_t mode)
{
    app = mode;
    knob_value = 0;
    switch (app) {
    case PLAYGROUND_MODE_NO_EFFECTS:
        break;
    case PLAYGROUND_MODE_FINE_DETENTS:
        // This mode is default
        break;
    case PLAYGROUND_MODE_BOUND:
        MAX_VALUE = 12;
        MIN_VALUE = 0;
        break;
    case PLAYGROUND_MODE_ON_OFF:
        MAX_VALUE = 1;
        MIN_VALUE = 0;
        break;
    default:
        break;
    }

}

void PlaygroundModel::ChangeMotorMode(int mode)
{
    knob_value = 0;
    printf("MenuModel: Change Motor Mode [%d]\n", mode);
    AccountSystem::Motor_Info_t info;
    info.cmd = AccountSystem::MOTOR_CMD_CHANGE_MODE;
    info.motor_mode = mode;
    info.init_position = 0;
    // 第一个参数是通知发布者，即本 Account 应该 subscribe 第一个参数指向的 Account 
    account->Notify("Motor", &info, sizeof(info));  
}

void PlaygroundModel::ChangeMotorModeWithInitPosition(int mode, int init_position)
{
    knob_value = init_position;
    printf("MenuModel: Change Motor Mode [%d] with init position [%d] \n", mode , init_position);
    AccountSystem::Motor_Info_t info;
    info.cmd = AccountSystem::MOTOR_CMD_CHANGE_MODE;
    info.motor_mode = mode;
    info.init_position = init_position;
    // 第一个参数是通知发布者，即本 Account 应该 subscribe 第一个参数指向的 Account 
    account->Notify("Motor", &info, sizeof(info));  
}

void PlaygroundModel::SetLcdBkBrightness(int value)
{
    static int last_value = -1;
    if(last_value != value){
        last_value = value;
        printf("PlaygroundModel: Change LCD Bightness [%d]\n", value) ;
        HAL::set_lcd_brightness( value );
    }
}


int onEvent(Account* account, Account::EventParam_t* param)
{
    if (param->size != sizeof(AccountSystem::Motor_Info_t))
    {
        return Account::ERROR_SIZE_MISMATCH;
    }
    
    MotorStatusInfo *info = (MotorStatusInfo*) (param->data_p);
    
    now_pos = info->position;
    is_outbound = info->is_outbound;
    konb_direction = SUPER_DIAL_NULL;
    if (is_outbound) {
        arc_offset = (int)(info->angle_offset);
    } else {
        arc_offset = 0;
    }
    // Serial.printf("now_pos:%d, last_pos: %d knob_value:%d, arch offset: %f\n", 
    //         now_pos, last_pos, knob_value, info->angle_offset);
    if (now_pos > last_pos)
    {
        PlaygroundModel::knob_value++;
        konb_direction = SUPER_DIAL_RIGHT;
        if (PlaygroundModel::knob_value > PlaygroundModel::MAX_VALUE) {
            PlaygroundModel::knob_value = PlaygroundModel::MAX_VALUE;
        }
        last_pos = now_pos;
    }
    else if (now_pos < last_pos)
    {
        PlaygroundModel::knob_value--;
        konb_direction = SUPER_DIAL_LEFT;
        if (PlaygroundModel::knob_value < PlaygroundModel::MIN_VALUE) {
            PlaygroundModel::knob_value = PlaygroundModel::MIN_VALUE;
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
    app = PLAYGROUND_MODE_NO_EFFECTS;
    
}

void PlaygroundModel::Deinit()
{
    if (account)
    {
        delete account;
        account = nullptr;
    }
}