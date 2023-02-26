#ifndef __HOME_ASSISTANT_VIEW_H
#define __HOME_ASSISTANT_VIEW_H
#include "Arduino.h"
#include "app/app.h"
#include "../Page.h"
#include "../Playground/PlaygroundView.h"
#include <map>

enum HASS_VIEW_MODE {
    VIEW_MODE_ON_OFF = 1,
};

typedef struct
{
    lv_obj_t* cont;   // include icon and label
    lv_obj_t* l_dev_name;
    bool is_on_off;  
    bool is_set_value;
} device_t;

namespace Page
{

class HassView: public PlaygroundView
{
public:
    HassView(){}
    void Create(lv_obj_t* root);
    void Delete();
    void UpdateView(PlaygroundInfo *info);
    void SetPlaygroundMode(int16_t mode);
    void UpdateFocusedDevice(const char* name);
    void SetCtrView(lv_obj_t *obj);
    void ClearCtrView(lv_obj_t *obj);
    void UpdateCtrlView(PlaygroundInfo *info);
    char* GetEditedDeviceName(void);
    int GetViewMode(void);
    struct
    {
        device_t fan;
        device_t monitor_light;
        device_t air_conditioning;
        device_t wash_machine;
        lv_obj_t* foucs_label;
        lv_group_t* group;
    } m_ui;

private:
    int current_view = 0;
    std::map<lv_obj_t*, device_t *>device_map;
    struct
    {
        lv_style_t cont;
        lv_style_t focus;
        lv_style_t edit;
        lv_style_t label_name;
    } style;
    void group_init(void);
    void style_init(void);
    void device_item_create(device_t* item, lv_obj_t* par,
        const char* name, const char* img_src,
        bool is_on_off, bool is_set_value);
};

}

#endif // !__VIEW_H
