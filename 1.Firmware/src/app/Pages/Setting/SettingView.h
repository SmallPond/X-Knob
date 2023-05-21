#ifndef __SETTING_VIEW_H
#define __SETTING_VIEW_H
#include "Arduino.h"
#include "app/app.h"
#include "../Page.h"
#include "../Playground/PlaygroundView.h"
#include "SettingModel.h"
#include "lvgl.h"
#include <map>



#define SETTING_NUM 3

enum SETTING_VIEW_MODE {
    DEFAULT_VIEW,
    WIFI_SETTING_VIEW,
    LCD_BK_TIMEOUT_SETTING_VIEW,
    LCD_BK_BRIGHTNESS_SETTING_VIEW
};

typedef struct
{
    lv_obj_t* cont;   // include icon and label
    lv_obj_t* l_dev_name;
    uint8_t num;
} setting_t;

namespace Page
{

class SettingView: public PlaygroundView
{
public:
    SettingView(){}
    void Create(lv_obj_t* root);
    void Delete();
    void UpdateView(SettingInfo *info);
    void SetPlaygroundMode(int16_t mode);
    void UpdateFocusedSetting(const char* name);
    void SetCtrView(lv_obj_t *obj);
    void ClearCtrView(lv_obj_t *obj);
    void UpdateCtrlView(SettingInfo *info);
    char* GetEditedSettingName(void);
    int GetViewMode(void);
    void SetWiFiViewVisible(bool visible);
    struct setting_ui 
    {
        lv_obj_t* foucs_label;
        lv_obj_t* wifi;
        lv_obj_t* wifi_label;
        lv_obj_t* cont_row;
        lv_group_t* group;
        setting_t setting[0];
    } *m_ui;

private:
    int current_view = 0;
    std::map<lv_obj_t*, setting_t *>setting_map;
    struct
    {
        lv_style_t cont;
        lv_style_t focus;
        lv_style_t edit;
        lv_style_t label_name;
    } style;
    
    void group_init(void);
    void style_init(void);
    void setting_item_create(setting_t* item, lv_obj_t* par,
        const char* name, const char* img_src,uint8_t num);
    void wifi_ui_init(lv_obj_t* root);
};

}

#endif // !__VIEW_H
