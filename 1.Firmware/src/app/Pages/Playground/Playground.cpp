#include "Playground.h"
#include <Arduino.h>
#include "SurfaceDialView.h"
#include "SurfaceDialModel.h"
#include "../HASS/HassModel.h"
#include "../HASS/HassView.h"
using namespace Page;

typedef struct {
	MOTOR_RUNNING_MODE_E motor_mode;
} app_mode_config_t;

app_mode_config_t app_config[] = {
	[PLAYGROUND_MODE_NO_EFFECTS] = {
		.motor_mode = MOTOR_UNBOUND_NO_DETENTS,
	},
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
		.motor_mode = MOTOR_SUPER_DIAL,
	},
	[APP_MODE_HOME_ASSISTANT] = {
		.motor_mode = MOTOR_COARSE_DETENTS,
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
	app = PLAYGROUND_MODE_NO_EFFECTS;  // default 
	if (priv.Stash.ptr) {
		app = *((int16_t *)priv.Stash.ptr);
		Serial.printf("\nPlayground: app = %d\n", app);
	}
	switch (app) {
		case PLAYGROUND_MODE_NO_EFFECTS:
			Model = new PlaygroundModel();
			View = new PlaygroundView();
			break;
		case APP_MODE_SUPER_DIAL:
			Model = (SurfaceDialModel*) new SurfaceDialModel(); 
			View = (PlaygroundView*) new SurfaceDialView();
			HAL::surface_dial_init();
			break;
		case APP_MODE_HOME_ASSISTANT:
			Model = (SurfaceDialModel*) new HassModel(); 
			View = (PlaygroundView*) new HassView();
			break;
		default:
			break;
	};

	Model->Init();
	View->Create(root);

	
	// lv_label_set_text(View->ui.labelTitle, Name);

	AttachEvent(root);
	AttachEvent(View->ui.meter);

	if (app == APP_MODE_HOME_ASSISTANT) {
		AttachEvent(((HassView*)View)->m_ui.fan.cont);
		AttachEvent(((HassView*)View)->m_ui.monitor_light.cont);
		AttachEvent(((HassView*)View)->m_ui.air_conditioning.cont);
		AttachEvent(((HassView*)View)->m_ui.wash_machine.cont);
	}

}

void Playground::onViewDidLoad()
{

}

void Playground::onViewWillAppear()
{

	Model->ChangeMotorMode(app_config[app].motor_mode);
	Model->SetPlaygroundMode(app);
	View->SetPlaygroundMode(app);

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
	View->Delete();
	Model->Deinit();

	delete View;
	delete Model;

}

void Playground::AttachEvent(lv_obj_t* obj)
{
	lv_obj_set_user_data(obj, this);
	lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}



void Playground::Update()
{
	PlaygroundInfo info;
	Model->GetKnobStatus(&info);
	if (app == APP_MODE_SUPER_DIAL && info.konb_direction != SUPER_DIAL_NULL) {
		HAL::surface_dial_update(info.konb_direction);
	}

	View->UpdateView(&info);
}


void Playground::onTimerUpdate(lv_timer_t* timer)
{
	Playground* instance = (Playground*)timer->user_data;

	instance->Update();
}

void Playground::SurfaceDialEventHandler(lv_event_t* event, lv_event_code_t code)
{
	if (code == LV_EVENT_PRESSED) {
		if (app == APP_MODE_SUPER_DIAL) {
			Serial.printf("Playground: press\n");
			HAL::surface_dial_press();
		}	
	} else if (code == LV_EVENT_LONG_PRESSED_REPEAT) {
		// return to memu
		Serial.printf("Playground: LV_EVENT_LONG_PRESSED_REPEAT\n");
		Model->ChangeMotorMode(MOTOR_UNBOUND_COARSE_DETENTS);
		Manager->Pop();
	} else if (code == LV_EVENT_RELEASED) {
		if (app == APP_MODE_SUPER_DIAL) {
			Serial.printf("Playground: realse\n");
			HAL::surface_dial_release();
		}
	}
}

void Playground::PlayEventHandler(lv_event_t* event, lv_event_code_t code)
{
	if (code == LV_EVENT_PRESSED) {
		
		int app = Model->app + 1;
	
		if (app != PLAYGROUND_MODE_MAX) {
			Model->ChangeMotorMode(app_config[app].motor_mode);
			Model->SetPlaygroundMode(app);
			View->SetPlaygroundMode(app);
		} else {
			// return to memu
			Model->ChangeMotorMode(MOTOR_UNBOUND_COARSE_DETENTS);
			Manager->Pop();
		}
		
		if (app == APP_MODE_SUPER_DIAL) {
			Serial.printf("Playground: press\n");
			HAL::surface_dial_press();
		}	
	} else if (code == LV_EVENT_LONG_PRESSED) {
		// return to memu
		Serial.printf("Playground: LV_EVENT_LONG_PRESSED\n");
		Model->ChangeMotorMode(MOTOR_UNBOUND_COARSE_DETENTS);
		Manager->Pop();
	} 
}

void Playground::HassEventHandler(lv_event_t* event, lv_event_code_t code)
{	
	lv_obj_t* obj = lv_event_get_target(event);
    lv_obj_t* label = lv_obj_get_child(obj, 1);
	
	if (code < LV_EVENT_RELEASED) {
		printf("code: %d\n", code);
	}
	
	if (code == LV_EVENT_FOCUSED) {
        if (label != NULL) {
			printf("fouces, name:%s\n", lv_label_get_text(label));
			((HassView*)View)->UpdateFocusedDevice(lv_label_get_text(label));
		}
    }
	if (code == LV_EVENT_PRESSED)
	{
		if (label != NULL) {
			printf("Control device: %s\n", lv_label_get_text(label));
		}
        lv_obj_add_state(obj, LV_STATE_EDITED);
		((HassView*)View)->SetCtrView(obj);
		HAL::encoder_disable();
        
	} else if (code == LV_EVENT_LONG_PRESSED) {
		Serial.printf("Hass: LV_EVENT_LONG_PRESSED\n");
		if (lv_obj_has_state(obj, LV_STATE_EDITED)) {
			((HassView*)View)->ClearCtrView(obj);
			lv_obj_clear_state(obj, LV_STATE_EDITED);
			HAL::encoder_enable();
		} 
	} else if (code == LV_EVENT_LONG_PRESSED_REPEAT) {
		// return to memu
		if (!lv_obj_has_state(obj, LV_STATE_EDITED)){
			Serial.printf("Playground: LV_EVENT_LONG_PRESSED_REPEAT\n");
			Model->ChangeMotorMode(MOTOR_UNBOUND_COARSE_DETENTS);
			Manager->Pop();
		}
	} 
}

void Playground::onEvent(lv_event_t* event)
{
	lv_obj_t* obj = lv_event_get_target(event);
	lv_event_code_t code = lv_event_get_code(event);
	auto* instance = (Playground*)lv_obj_get_user_data(obj);

	switch (app) {
		case PLAYGROUND_MODE_NO_EFFECTS:
		case PLAYGROUND_MODE_FINE_DETENTS:
    	case PLAYGROUND_MODE_BOUND:
    	case PLAYGROUND_MODE_ON_OFF:
			instance->PlayEventHandler(event, code);
			break;
		case APP_MODE_SUPER_DIAL:
			instance->SurfaceDialEventHandler(event, code);
			break;
		case APP_MODE_HOME_ASSISTANT:
			instance->HassEventHandler(event, code);
			break;
		default:
			break;
	}
}
