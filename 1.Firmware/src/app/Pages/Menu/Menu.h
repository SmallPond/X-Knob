#ifndef __MENU_PRESENTER_H
#define __MENU_PRESENTER_H

#include "MenuView.h"
#include "MenuModel.h"

namespace Page
{

class Menu : public PageBase
{
public:

public:
    Menu();
    virtual ~Menu();

    virtual void onCustomAttrConfig();
    virtual void onViewLoad();
    virtual void onViewDidLoad();
    virtual void onViewWillAppear();
    virtual void onViewDidAppear();
    virtual void onViewWillDisappear();
    virtual void onViewDidDisappear();
    virtual void onViewDidUnload();

private:
    void Update();
    void AttachEvent(lv_obj_t* obj, lv_event_cb_t event_cb);
    static void onTimerUpdate(lv_timer_t* timer);
    static void onPlaygroundEvent(lv_event_t* event);
    static void onSuperDialEvent(lv_event_t* event);
    static void onSystemEvent(lv_event_t* event);
    static void onHassEvent(lv_event_t* event);
    static void onSettingEvent(lv_event_t* event);
private:
    MenuView View;
    MenuModel Model;
    lv_timer_t* timer;
};

}

#endif
