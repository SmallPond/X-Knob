#ifndef __PLAGROUND_PRESENTER_H
#define __PLAGROUND_PRESENTER_H

#include "PlaygroundView.h"
#include "PlaygroundModel.h"

namespace Page
{

class Playground : public PageBase
{
public:
    typedef struct
    {
        uint16_t time;
        lv_color_t color;
    } Param_t;

public:
    Playground();
    virtual ~Playground();

    virtual void onCustomAttrConfig();
    virtual void onViewLoad();
    virtual void onViewDidLoad();
    virtual void onViewWillAppear();
    virtual void onViewDidAppear();
    virtual void onViewWillDisappear();
    virtual void onViewDidDisappear();
    virtual void onViewDidUnload();
    void PlayEventHandler(lv_event_t* event, lv_event_code_t code);

private:
    void Update();
    void AttachEvent(lv_obj_t* obj);
    static void onTimerUpdate(lv_timer_t* timer);
    static void onEvent(lv_event_t* event);

private:
    PlaygroundView *View;
    PlaygroundModel *Model;
    lv_timer_t* timer;
};

}

#endif
