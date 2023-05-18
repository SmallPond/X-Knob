#include "SettingModel.h"
#include "app/Accounts/Account_Master.h"

using namespace Page;

SettingModel::SettingModel()
{
    app = APP_MODE_HOME_ASSISTANT;
}

void SettingModel::GetKnobStatus(SettingInfo* info)
{
    PlaygroundModel::GetKnobStatus(info);
}

void SettingModel::Init()
{
    PlaygroundModel::Init();
    printf("SettingModel: Init start\n");
}

void SettingModel::Deinit()
{
    PlaygroundModel::Deinit();
}

void SettingModel::ChangeMotorMode(int mode)
{
    PlaygroundModel::ChangeMotorMode(mode);
}

void SettingModel::SetPlaygroundMode(int16_t mode)
{
    switch(mode){
        case SETTING_MODE_LCD_BK_TIMEOUT:
            MAX_VALUE = 30;
            MIN_VALUE = 0;
            knob_value = get_lcd_bk_timeout();
            break;
        case SETTING_MODE_LCD_BK_BRIGHTNESS:
            MAX_VALUE = 100;
            MIN_VALUE = 10;
            knob_value = get_lcd_bk_brightness();
            break;
        default:
            PlaygroundModel::SetPlaygroundMode(mode);
            break;
    };
}


void SettingModel::SaveLcdBkTimeout(int value)
{
    //printf("SaveLcdBkTimeout %d\n" ,value);
    set_lcd_bk_timeout( value );
}

void SettingModel::SaveLcdBkBrightness(int value)
{
    //printf("SaveLcdBkBrightness %d\n" ,value);
    set_lcd_bk_brightness( value );
}