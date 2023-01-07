#include "PlaygroundModel.h"
#include "App/Accounts/Account_Master.h"
#include <Arduino.h>
using namespace Page;
static int32_t knob_value = 50;
static int now_pos = 0;
static int last_pos = 0;
static int32_t MAX_VALUE = 100;
static int32_t MIN_VALUE = 0;

void PlaygroundModel::GetKnobStatus(int32_t *value, int32_t *motor_position)
{
    *value = knob_value;
    *motor_position = now_pos;
}

static int onEvent(Account* account, Account::EventParam_t* param)
{
    
    now_pos = (*((int32_t*) (param->data_p))) ;
    Serial.printf("playground, now_pos:%d, last_pos: %d knob_value:%d\n", now_pos,last_pos, knob_value);
    if (now_pos > last_pos)
    {
        knob_value--;
        if (knob_value < MIN_VALUE) {
            knob_value = MIN_VALUE;
        }
        last_pos = now_pos;
    }
    else if (now_pos < last_pos)
    {
        knob_value++;
        if (knob_value > MAX_VALUE) {
            knob_value = MAX_VALUE;
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
    
}

void PlaygroundModel::Deinit()
{
    if (account)
    {
        delete account;
        account = nullptr;
    }
}