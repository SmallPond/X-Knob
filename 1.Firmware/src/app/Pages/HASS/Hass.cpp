#include "Hass.h"
#include "hal/hal.h"
#include "config.h"
#include "hal/nvs.h"

using namespace Page;

const char* playload_str[] {
	[SUPER_DIAL_NULL] = "NULL",
	[HASS_LEFT] = "LEFT",
	[HASS_RIGHT] = "RIGHT",
	[HASS_PUSH]  = "ON/OFF"
};
const int motor_mode = MOTOR_SUPER_DIAL;
const int app_mode = APP_MODE_HOME_ASSISTANT;

void hass_hal_init(void)
{
	HAL::mqtt_init();
}

int hass_hal_send(const char *device_name, int knob_value)
{
	char topic_name[128];
	uint16_t port;
	String password, host, username, topic;

	get_mqtt_config(host,port,username,password,topic);
	const char * mqtt_topic = topic.c_str();
	snprintf(topic_name, sizeof(topic_name),"%s/HOME/%s", mqtt_topic, device_name);
	if (knob_value < HASS_MAX && playload_str[knob_value] != NULL) {
		printf("mqtt send: %s:%s\n", topic_name, playload_str[knob_value]);
		return HAL::mqtt_publish(topic_name, playload_str[knob_value]);
	} else {
		printf("hass_hal_send: xknob_value is invalid\n");
		return -1;
	}
	return 0;
}

Hass::Hass()
{
}

Hass::~Hass()
{
}

void Hass::onCustomAttrConfig()
{
	SetCustomCacheEnable(false);
	// SetCustomLoadAnimType(PageManager::LOAD_ANIM_OVER_BOTTOM, 500, lv_anim_path_bounce);
}

void Hass::onViewLoad()
{
	Model = new HassModel();
	View = new HassView();

	Model->Init();
	View->Create(root);

	AttachEvent(root);
	AttachEvent(View->ui.meter);

	for(int i = 0; i < DEVICE_NUM; i++) {
		AttachEvent((View->m_ui->devices[i]).cont);
	}
}

void Hass::AttachEvent(lv_obj_t* obj)
{
	lv_obj_set_user_data(obj, this);
	lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Hass::onViewDidLoad()
{

}

void Hass::onViewWillAppear()
{
	Model->ChangeMotorMode(motor_mode);
	Model->SetPlaygroundMode(app_mode);
	View->SetPlaygroundMode(app_mode);

	hass_hal_init();
	timer = lv_timer_create(onTimerUpdate, 10, this);
}

void Hass::onViewDidAppear()
{

}

void Hass::onViewWillDisappear()
{

}

void Hass::onViewDidDisappear()
{
	lv_timer_del(timer);
}

void Hass::onViewDidUnload()
{
	View->Delete();
	Model->Deinit();

	delete View;
	delete Model;

}

void Hass::Update()
{
	HassInfo info;
	Model->GetKnobStatus(&info);
	if (info.konb_direction != SUPER_DIAL_NULL) {
		char* name = ((HassView*)View)->GetEditedDeviceName();
		if (name != NULL) {
			hass_hal_send(name, info.konb_direction);
		}
	}
	View->UpdateView(&info);
}

void Hass::onTimerUpdate(lv_timer_t* timer)
{
	Hass* instance = (Hass*)timer->user_data;

	instance->Update();
}

void Hass::HassEventHandler(lv_event_t* event, lv_event_code_t code)
{
	lv_obj_t* obj = lv_event_get_target(event);
	lv_obj_t* label = lv_obj_get_child(obj, 1);

	if (code == LV_EVENT_FOCUSED) {
		if (label != NULL) {
			printf("fouces, name:%s\n", lv_label_get_text(label));
			((HassView*)View)->UpdateFocusedDevice(lv_label_get_text(label));
		}
	}
	if (code == LV_EVENT_SHORT_CLICKED) {
		if (!lv_obj_has_state(obj, LV_STATE_EDITED)) {
			if (label != NULL) {
				printf("Control device: %s\n", lv_label_get_text(label));
			}
			lv_obj_add_state(obj, LV_STATE_EDITED);
			((HassView*)View)->SetCtrView(obj);
			HAL::encoder_disable();
			if (((HassView*)View)->GetViewMode() == VIEW_MODE_ON_OFF) {
				Model->ChangeMotorMode(MOTOR_ON_OFF_STRONG_DETENTS);
			}
		} else {
			hass_hal_send(lv_label_get_text(label), HASS_PUSH);
		}
	} else if (code == LV_EVENT_LONG_PRESSED) {
		if (lv_obj_has_state(obj, LV_STATE_EDITED)) {
			((HassView*)View)->ClearCtrView(obj);
			lv_obj_clear_state(obj, LV_STATE_EDITED);
			HAL::encoder_enable();
			Model->ChangeMotorMode(MOTOR_UNBOUND_COARSE_DETENTS);
		}
	} else if (code == LV_EVENT_LONG_PRESSED_REPEAT) {
		// return to memu
		if (!lv_obj_has_state(obj, LV_STATE_EDITED)){
			Model->ChangeMotorMode(MOTOR_UNBOUND_COARSE_DETENTS);
			Manager->Pop();
		}
	}
}

void Hass::onEvent(lv_event_t* event)
{
	lv_obj_t* obj = lv_event_get_target(event);
	lv_event_code_t code = lv_event_get_code(event);
	auto* instance = (Hass*)lv_obj_get_user_data(obj);

	instance->HassEventHandler(event, code);
}