#ifndef __PLAGROUND_VIEW_H
#define __PLAGROUND_VIEW_H
#include "Arduino.h"
#include "app/app.h"
#include "../Page.h"
#include "PlaygroundModel.h"

namespace Page
{

class PlaygroundView
{
public:

    int32_t MAX_VALUE = 100;
    int32_t MIN_VALUE = 0;
    int32_t SCALE_LEFT_BOUND_TICKS = 200;
    int32_t SCALE_ANGLE_RANGE = 140;
    int32_t SCALE_RIGHT_BOUND_TICKS = SCALE_LEFT_BOUND_TICKS + SCALE_ANGLE_RANGE;
    int32_t ARC_START_ROTATION = 120;

    virtual void Create(lv_obj_t* root);
    // maybe we should use virtual
    void Delete();
    virtual void UpdateView(PlaygroundInfo *info);
    virtual void SetPlaygroundMode(int16_t mode);
    void UpdateBackgroundView(PlaygroundInfo *info); 
    int16_t app;


public:
    struct
    {
        lv_obj_t* meter;
        lv_meter_scale_t* scale_pot;   // 圆点的刻度
        lv_obj_t* label_value;
        lv_meter_indicator_t* nd_img_circle;
        lv_meter_scale_t* scale_arc;   // arc 的刻度
        lv_meter_indicator_t* arc;
        lv_group_t* group;
    } ui;

    struct
    {
        lv_style_t meter;
        lv_style_t ticks;
    } style;
protected:
    void OnOffView(void);
    void BoundZeroView(void);
    void DefaultView(void);
private:

};

}

#endif // !__VIEW_H
