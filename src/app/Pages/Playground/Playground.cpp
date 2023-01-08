#include "Playground.h"
#include <Arduino.h>
using namespace Page;

Playground::Playground()
{
}

Playground::~Playground()
{

}

void Playground::onCustomAttrConfig()
{
	SetCustomCacheEnable(false);
	// SetCustomLoadAnimType(PageManager::LOAD_ANIM_OVER_BOTTOM, 500, lv_anim_path_bounce);
}

void Playground::onViewLoad()
{
	Model.Init();
	View.Create(root);
	
	// lv_label_set_text(View.ui.labelTitle, Name);

	AttachEvent(root);
	AttachEvent(View.ui.meter);

}

void Playground::onViewDidLoad()
{

}

void Playground::onViewWillAppear()
{

	// lv_obj_set_style_bg_color(root, lv_color_white(), LV_PART_MAIN);
	Model.ChangeMotorMode(PLAYGROUND_MODE_FINE_DETENTS);
	Model.SetPlaygroundMode(PLAYGROUND_MODE_FINE_DETENTS);
	View.SetPlaygroundMode(PLAYGROUND_MODE_FINE_DETENTS);

	timer = lv_timer_create(onTimerUpdate, 10, this);
}

void Playground::onViewDidAppear()
{

}

void Playground::onViewWillDisappear()
{

}

void Playground::onViewDidDisappear()
{
	lv_timer_del(timer);
}

void Playground::onViewDidUnload()
{
	View.Delete();
	Model.Deinit();

}

void Playground::AttachEvent(lv_obj_t* obj)
{
	lv_obj_set_user_data(obj, this);
	lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Playground::Update()
{
	int32_t pos = 0;
	int32_t xknob_value = 2;
	PlaygroundMotorInfo info;
	Model.GetKnobStatus(&info);
	// Serial.printf("xknob_value %d, pos: %d\n", xknob_value, pos);
	View.UpdateView(&info);
}


void Playground::onTimerUpdate(lv_timer_t* timer)
{
	Playground* instance = (Playground*)timer->user_data;

	instance->Update();
}

void Playground::onEvent(lv_event_t* event)
{
	lv_obj_t* obj = lv_event_get_target(event);
	lv_event_code_t code = lv_event_get_code(event);
	auto* instance = (Playground*)lv_obj_get_user_data(obj);

	if (code == LV_EVENT_PRESSED) {
		int next_mod = 0;
		switch (instance->Model.playgroundMode)
		{
		case PLAYGROUND_MODE_FINE_DETENTS:
			next_mod = PLAYGROUND_MODE_BOUND;
			break;
		case PLAYGROUND_MODE_BOUND:
			next_mod = PLAYGROUND_MODE_ON_OFF;
			break;
		case PLAYGROUND_MODE_ON_OFF:
			next_mod = PLAYGROUND_MODE_MAX;  // END
			break; 
		default:
			break;
		}
		
		if (next_mod != PLAYGROUND_MODE_MAX) {
			instance->Model.ChangeMotorMode(next_mod);
			instance->Model.SetPlaygroundMode(next_mod);
			instance->View.SetPlaygroundMode(next_mod);
		} else {
			instance->Model.ChangeMotorMode(MOTOR_UNBOUND_COARSE_DETENTS);
			instance->Manager->Pop();
		}
			
	} else if (code == LV_EVENT_LONG_PRESSED) {
		instance->Model.ChangeMotorMode(MOTOR_UNBOUND_COARSE_DETENTS);
		instance->Manager->Pop();
	}
}
