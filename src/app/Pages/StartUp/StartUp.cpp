#include "StartUp.h"
#include "Arduino.h"
using namespace Page;

Startup::Startup()
{
}

Startup::~Startup()
{

}

void Startup::onCustomAttrConfig()
{
    SetCustomCacheEnable(false);
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_NONE);
}

void Startup::onViewLoad()
{
    Serial.printf("\nLoading startup view\n");
    Model.Init();
    Model.SetEncoderEnable(false);
    View.Create(root);
    lv_timer_t* timer = lv_timer_create(onTimer, 10000, this);
    lv_timer_set_repeat_count(timer, 1);
}

void Startup::onViewDidLoad()
{
    Serial.printf("onViewDidLoad\n");
    lv_obj_fade_out(root, 300, 1500);
}

void Startup::onViewWillAppear()
{
    Serial.printf("\n onViewWillAppear\n");
    lv_anim_timeline_start(View.ui.anim_timeline);
}

void Startup::onViewDidAppear()
{

}

void Startup::onViewWillDisappear()
{

}

void Startup::onViewDidDisappear()
{
    Serial.printf("\n onViewDidDisappear\n");
    StatusBar::Appear(true);
}

void Startup::onViewDidUnload()
{
    Serial.printf("\n onViewDidUnload\n");
    View.Delete();
    Model.SetEncoderEnable(true);
    Model.DeInit();
}

void Startup::onTimer(lv_timer_t* timer)
{
    Startup* instance = (Startup*)timer->user_data;

    instance->Manager->Push("Pages/Template");
}
