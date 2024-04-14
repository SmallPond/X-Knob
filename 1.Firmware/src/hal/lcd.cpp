#include "hal.h"
#include "app/Accounts/Account_Master.h"
#include "hal/motor.h"
#include "hal/nvs.h"


TaskHandle_t handleTaskLcdBrightness;
Account* account;
bool last_btn_status = false;
int now_motor_position = 0;
int last_motor_position = 0;
u_int32_t timeout = 0;
#define LCD_BRIGHTNESS_TASK_PERIOD 20

void TaskLcdBrightnessUpdate(void *pvParameters)
{
    bool lcd_brightness_on = true;
    int set_timeout = get_lcd_bk_timeout() * (1000 / LCD_BRIGHTNESS_TASK_PERIOD) * 60;
    while(1) {
        bool now_btn_status = HAL::encoder_is_pushed();
        if (now_btn_status == false && last_btn_status == false && 
                    last_motor_position == now_motor_position) {
            if (set_timeout != 0) {           // 开启了背光功能
                if (timeout < set_timeout) {  // 记时
                    timeout++;
                }
            }
        } else {
            timeout = 0;
        }
        if (timeout == set_timeout) {  // 关闭背光
            timeout++;
            HAL::set_lcd_brightness( 0 );
            log_d("turn off LCD bk\n");
            lcd_brightness_on = false;
        } else if ( timeout == 0 && !lcd_brightness_on ){  // 开启背光
            lcd_brightness_on = true;
            HAL::set_lcd_brightness( get_lcd_bk_brightness() );
            log_d("turn on LCD bk\n");

        }
        last_btn_status = now_btn_status;
        last_motor_position = now_motor_position;
        vTaskDelay(LCD_BRIGHTNESS_TASK_PERIOD);
    }
}


int onMotorEvent(Account* account, Account::EventParam_t* param)
{
    MotorStatusInfo *info = (MotorStatusInfo*) (param->data_p);
    now_motor_position = info->position;
    return 0;
}


void HAL::lcd_brightness_init() 
{
    ledcSetup(0, 5000, 10);      //通道0， 5KHz，10位解析度
    ledcAttachPin(TFT_BLK, 0);  //pin25定义为通道0的输出引脚
    account = new Account("LcdBrightness", AccountSystem::Broker(), 0, NULL);
    account->SetEventCallback(onMotorEvent);
    account->Subscribe("MotorStatus");
    xTaskCreatePinnedToCore(
        TaskLcdBrightnessUpdate,
        "LcdBkThread",
        4096,
        nullptr,
        2,
        &handleTaskLcdBrightness,
        ESP32_RUNNING_CORE
    );
    // update_motor_config(0);
}


void HAL::set_lcd_brightness(int value)
{
    if (value >= 100) {
        value = 100;
    } else if (value <= 0) {
        value = 0;
    }
    ledcWrite(0, value * 10 );
}