#include "Playground.h"
#include <Arduino.h>
using namespace Page;

typedef struct {
	MOTOR_RUNNING_MODE_E motor_mode;
} app_mode_config_t;

app_mode_config_t app_config[] = {
	[PLAYGROUND_MODE_FINE_DETENTS] = {
		.motor_mode = MOTOR_UNBOUND_FINE_DETENTS,
	},
	[PLAYGROUND_MODE_BOUND] = {
		.motor_mode = MOTOR_BOUND_0_12_NO_DETENTS,
	},
	[PLAYGROUND_MODE_ON_OFF] = {
		.motor_mode = MOTOR_ON_OFF_STRONG_DETENTS,
	},
	[APP_MODE_SUPER_DIAL] = {
		.motor_mode = MOTOR_UNBOUND_FINE_DETENTS,
	},
};

int16_t app = 0;

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
	app = PLAYGROUND_MODE_FINE_DETENTS;  // default 
	if (priv.Stash.ptr) {
		app = *((int16_t *)priv.Stash.ptr);
		Serial.printf("Playground: app = %d\n", app);
	}
	// lv_obj_set_style_bg_color(root, lv_color_white(), LV_PART_MAIN);
	
	Model.ChangeMotorMode(app_config[app].motor_mode);
	Model.SetPlaygroundMode(app);
	View.SetPlaygroundMode(app);

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
	if (app == APP_MODE_SUPER_DIAL && info.konb_direction != SUPER_DIAL_NULL) {
		HAL::super_dial_update(info.konb_direction);
	}
	View.UpdatePlaygroundView(&info);
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
	// if(code >= LV_EVENT_PRESSED && code <= LV_EVENT_KEY)
	// 	Serial.printf("Playground: event code:%d\n", code);
	if (code == LV_EVENT_PRESSED) {
		if(app < PLAYGROUND_MODE_MAX) {
			int app = instance->Model.playgroundMode + 1;
		
			if (app != PLAYGROUND_MODE_MAX) {
				instance->Model.ChangeMotorMode(app_config[app].motor_mode);
				instance->Model.SetPlaygroundMode(app);
				instance->View.SetPlaygroundMode(app);
			} else {
				// return to memu
				instance->Model.ChangeMotorMode(MOTOR_UNBOUND_COARSE_DETENTS);
				instance->Manager->Pop();
			}
		}	
	} else if (code == LV_EVENT_SHORT_CLICKED) {
		if (app == APP_MODE_SUPER_DIAL) {
			Serial.printf("Playground: press\n");
			HAL::suer_dial_press();
		}
	} else if (code == LV_EVENT_LONG_PRESSED_REPEAT) {
		// return to memu
		Serial.printf("Playground: LV_EVENT_LONG_PRESSED_REPEAT\n");
		instance->Model.ChangeMotorMode(MOTOR_UNBOUND_COARSE_DETENTS);
		instance->Manager->Pop();
	} else if (code == LV_EVENT_RELEASED) {
		if (app == APP_MODE_SUPER_DIAL) {
			Serial.printf("Playground: realse\n");
			HAL::suer_dial_release();
		}
	}
}
