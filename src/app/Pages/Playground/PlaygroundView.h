#ifndef __PLAGROUND_VIEW_H
#define __PLAGROUND_VIEW_H
#include "Arduino.h"
#include "../Page.h"

enum PLAYGROUND_MODE {
    PLAYGROUND_MODE_UNBOUND = 0,
    PLAYGROUND_MODE_BOUND,
    PLAYGROUND_MODE_MULTI_RCV,
};

namespace Page
{

class PlaygroundView
{
public:
    void Create(lv_obj_t* root);
    void UpdateView(int32_t xkonb_value, int32_t motor_pos);
    void SetPlaygroundMode(int16_t mode);
public:
    struct
    {
        lv_obj_t* meter;
        lv_meter_scale_t* scale_circle;
        lv_meter_scale_t* scale_value;
        lv_obj_t* lable_value;
        lv_meter_indicator_t* nd_img_circle;
        lv_meter_indicator_t* arc;
        lv_group_t* group;
    } ui;

    struct
    {
        lv_style_t meter;
        lv_style_t ticks;
    } style;

private:
    int16_t m_playgroundMode;
};

}

#endif // !__VIEW_H
