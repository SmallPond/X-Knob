#include "Setting.h"
#include "hal/hal.h"
#include "config.h"

using namespace Page;
TaskHandle_t handleTaskWiFi;

const int motor_mode = MOTOR_SUPER_DIAL;
const int app_mode = APP_MODE_SETTING;

Setting::Setting()
{	
}

Setting::~Setting()
{
}

void Setting::onCustomAttrConfig()
{
    SetCustomCacheEnable(false);
    // SetCustomLoadAnimType(PageManager::LOAD_ANIM_OVER_BOTTOM, 500, lv_anim_path_bounce);
}

void Setting::onViewLoad()
{
    Model = new SettingModel();
    View = new SettingView();

    Model->Init();
    View->Create(root);

    AttachEvent(root);
    AttachEvent(View->ui.meter);

    for(int i = 0; i < SETTING_NUM ; i++) {
        AttachEvent((View->m_ui->setting[i]).cont);
    }
}

void Setting::AttachEvent(lv_obj_t* obj)
{
    lv_obj_set_user_data(obj, this);
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Setting::onViewDidLoad()
{

}

void Setting::onViewWillAppear()
{
    Model->ChangeMotorMode(motor_mode);
    Model->SetPlaygroundMode(app_mode);
    View->SetPlaygroundMode(app_mode);

    timer = lv_timer_create(onTimerUpdate, 10, this);
}

void Setting::onViewDidAppear()
{

}

void Setting::onViewWillDisappear()
{

}

void Setting::onViewDidDisappear()
{
    lv_timer_del(timer);
}

void Setting::onViewDidUnload()
{
    View->Delete();
    Model->Deinit();
    delete View;
    delete Model;
}

void Setting::Update()
{
    SettingInfo info;
    Model->GetKnobStatus(&info);
    if (info.konb_direction != SUPER_DIAL_NULL){
        char* name = ((SettingView*)View)->GetEditedSettingName();
    }
    View->UpdateView(&info);
    int pos;
    switch(View->GetViewMode()){
        case LCD_BK_BRIGHTNESS_SETTING_VIEW:
            Model->SetLcdBkBrightness(info.xkonb_value);
            break;
    }
}

void Setting::onTimerUpdate(lv_timer_t* timer)
{
    Setting* instance = (Setting*)timer->user_data;

    instance->Update();
}

void Setting::SetWiFiTaskState(bool enable){
    if (enable) {
        #if DEBUG_PRINT
            printf("enable WiFi Task\n");
        #endif
        wifi_task_param.ui_wifi_label = View->m_ui->wifi_label;
        xTaskCreatePinnedToCore(
            wifi_server_begin,
            "wifi_server_begin", // 任務名稱. 
            8192,                // 任務的堆棧大小 
            &wifi_task_param,         // 任務的參數 
            4,                   // 任務的優先級 
            &handleTaskWiFi,     //跟蹤創建的任務的任務句柄 
            ESP32_RUNNING_CORE   // pin任務到核心1
    ); 
    } else {
        close_server();
        vTaskDelay(4);
        if (handleTaskWiFi != NULL){
            //vTaskSuspend(handleTaskWiFi);  //暂停执行
            #if DEBUG_PRINT
                printf("disable WiFi Task\n");
            #endif
            vTaskDelete(handleTaskWiFi);
            handleTaskWiFi = NULL;
        }
    }
    
}


void Setting::SettingEventHandler(lv_event_t* event, lv_event_code_t code)
{
    lv_obj_t* obj = lv_event_get_target(event);
    lv_obj_t* label = lv_obj_get_child(obj, 1);
    SettingInfo info;
    Model->GetKnobStatus(&info);
    if (code == LV_EVENT_FOCUSED) {
        if (label != NULL) {
            printf("fouces, name:%s\n", lv_label_get_text(label));
            ((SettingView*)View)->UpdateFocusedSetting(lv_label_get_text(label));
        }
    }
    if (code == LV_EVENT_SHORT_CLICKED){
        printf("LV_EVENT_SHORT_CLICKED\n");
        if (!lv_obj_has_state(obj, LV_STATE_EDITED)) {
            if (label != NULL) {
                printf("Control setting: %s\n", lv_label_get_text(label));
            }
            if ( lv_obj_has_state(obj, LV_STATE_USER_1) ){
                printf("LV_STATE_USER_1 \n");
                lv_obj_clear_state(obj, LV_STATE_USER_1);
                return;
            } else {
                lv_obj_add_state(obj, LV_STATE_EDITED);
            }
            ((SettingView*)View)->SetCtrView(obj);  //選擇UI
            switch(((SettingView*)View)->GetViewMode()){
                case DEFAULT_VIEW:
                    #if DEBUG_PRINT
                        printf("select DEFAULT_VIEW \n");
                    #endif
                case WIFI_SETTING_VIEW:
                    #if DEBUG_PRINT
                        printf("select WIFI_SETTING_VIEW \n");
                    #endif
                    SetWiFiTaskState(true);
                    HAL::encoder_disable();
                    View->SetPlaygroundMode(WIFI_SETTING_VIEW);
                    Model->ChangeMotorMode(MOTOR_UNBOUND_COARSE_DETENTS);
                    break;
                case LCD_BK_TIMEOUT_SETTING_VIEW:
                    #if DEBUG_PRINT
                        printf("select LCD_BK_TIMEOUT_SETTING_VIEW \n");
                    #endif
                    HAL::encoder_disable();
                    View->SetPlaygroundMode(LCD_BK_TIMEOUT_SETTING_VIEW);
                    Model->SetPlaygroundMode(SETTING_MODE_LCD_BK_TIMEOUT);
                    Model->ChangeMotorModeWithInitPosition(MOTOR_BOUND_LCD_BK_TIMEOUT, Model->knob_value );
                    break;
                case LCD_BK_BRIGHTNESS_SETTING_VIEW:
                    #if DEBUG_PRINT
                        printf("select LCD_BK_BRIGHTNESS_SETTING_VIEW \n");
                    #endif
                    HAL::encoder_disable();
                    View->SetPlaygroundMode(LCD_BK_BRIGHTNESS_SETTING_VIEW);
                    Model->SetPlaygroundMode(SETTING_MODE_LCD_BK_BRIGHTNESS);
                    Model->ChangeMotorModeWithInitPosition(MOTOR_BOUND_LCD_BK_BRIGHTNESS, Model->knob_value );
                    break;
            }
        } else {
            switch(((SettingView*)View)->GetViewMode()){
                case DEFAULT_VIEW:
                    #if DEBUG_PRINT
                        printf("Enter DEFAULT_VIEW \n");
                    #endif
                case WIFI_SETTING_VIEW:
                    #if DEBUG_PRINT
                        printf("Enter WIFI_SETTING_VIEW Press\n");
                    #endif
                    break;
                case LCD_BK_TIMEOUT_SETTING_VIEW:
                    #if DEBUG_PRINT
                        printf("Enter LCD_BK_TIMEOUT_SETTING_VIEW Press\n");
                    #endif
                    Model->ChangeMotorMode(MOTOR_UNBOUND_COARSE_DETENTS);
                    Model->SaveLcdBkTimeout(info.xkonb_value);
                    ((SettingView*)View)->ClearCtrView(obj);
                    View->SetPlaygroundMode(app_mode);  //關閉顯示數值
                    lv_obj_clear_state(obj, LV_STATE_EDITED);
                    HAL::encoder_enable();
                    break;
                case LCD_BK_BRIGHTNESS_SETTING_VIEW:
                    #if DEBUG_PRINT
                        printf("Enter LCD_BK_BRIGHTNESS_SETTING_VIEW Press\n");
                    #endif
                    Model->ChangeMotorMode(MOTOR_UNBOUND_COARSE_DETENTS);
                    Model->SaveLcdBkBrightness(info.xkonb_value);
                    View->SetPlaygroundMode(app_mode);  //關閉顯示數值
                    View->ClearCtrView(obj);
                    lv_obj_clear_state(obj, LV_STATE_EDITED);
                    HAL::encoder_enable();
                    break;
            }
        }
    } else if (code == LV_EVENT_LONG_PRESSED) {
        printf("Setting: LV_EVENT_LONG_PRESSED\n");
        if (lv_obj_has_state(obj, LV_STATE_EDITED)) {
            View->ClearCtrView(obj);
            SetWiFiTaskState(false);
            lv_obj_clear_state(obj, LV_STATE_EDITED);
            HAL::encoder_enable();
            View->SetPlaygroundMode(app_mode);  //關閉顯示數值
            Model->ChangeMotorMode(MOTOR_UNBOUND_COARSE_DETENTS);
        }
    } else if (code == LV_EVENT_LONG_PRESSED_REPEAT) {
        // return to memu
        if (!lv_obj_has_state(obj, LV_STATE_EDITED)){
            printf("Setting: LV_EVENT_LONG_PRESSED_REPEAT\n");
            Model->ChangeMotorMode(MOTOR_UNBOUND_COARSE_DETENTS);
            Manager->Pop();
        }
    }
}

void Setting::onEvent(lv_event_t* event)
{
    lv_obj_t* obj = lv_event_get_target(event);
    lv_event_code_t code = lv_event_get_code(event);
    auto* instance = (Setting*)lv_obj_get_user_data(obj);

    instance->SettingEventHandler(event, code);
}