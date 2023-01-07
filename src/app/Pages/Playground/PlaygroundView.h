#ifndef __PLAGROUND_VIEW_H
#define __PLAGROUND_VIEW_H

#include "../Page.h"

namespace Page
{

class PlaygroundView
{
public:
    void Create(lv_obj_t* root);
    void UpdateView(int32_t xkonb_value, int32_t motor_pos);
public:
    struct
    {
        lv_obj_t* meter;
        lv_meter_scale_t* scale_circle;
        lv_meter_scale_t* scale_value;
        lv_obj_t* lable_value;
        lv_meter_indicator_t* nd_img_circle;
        lv_meter_indicator_t* arc_value;
        lv_group_t* group;
    } ui;

private:

};

}

#endif // !__VIEW_H
