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

void TaskLcdBrightnessUpdate(void *pvParameters) {
    bool lcd_brightness_on = true;
    int set_timeout = get_lcd_bk_timeout() * (1000 / LCD_BRIGHTNESS_TASK_PERIOD) * 60;
    while(1) {
        bool now_btn_status = HAL::encoder_is_pushed();
        if( now_btn_status == false && last_btn_status == false && last_motor_position == now_motor_position ){
            if(set_timeout != 0){  //有設定關閉背光
                if( timeout < set_timeout ) {  //還未到設定時間
                    timeout++;
                }
            }
        }else{
            timeout = 0;
        }
        if( timeout == set_timeout ){  //到達設定時間，關閉背光
            timeout++;
            HAL::set_lcd_brightness( 0 );
            #if DEBUG_PRINT
                printf("turn off LCD bk\n");
            #endif
            lcd_brightness_on = false;
        }else if( timeout == 0 && !lcd_brightness_on ){  //按鈕有動作，開啟背光
            lcd_brightness_on = true;
            HAL::set_lcd_brightness( get_lcd_bk_brightness() );
            #if DEBUG_PRINT
                printf("turn on LCD bk\n");
            #endif
        }
        //printf("status %d %d %d %d %d\n", last_btn_status, now_btn_status, last_motor_position, now_motor_position, timeout);
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


void HAL::lcd_brightness_init() {
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


void HAL::set_lcd_brightness(int value) {
    if(value >= 100){
        value = 100;
    }else if(value <= 0){
        value = 0;
    }
    ledcWrite(0, value * 10 );
}