#include "Menu.h"
#include "app/Configs/Version.h"
#include "hal/motor.h"
#include "app/app.h"
using namespace Page;

Menu::Menu()
{
}

Menu::~Menu()
{

}

void Menu::onCustomAttrConfig()
{

}

void Menu::onViewLoad()
{
	Model.Init();
	View.Create(root);
	AttachEvent(root, onPlaygroundEvent);
	AttachEvent(View.ui.dialpad.icon, onSuperDialEvent);
	AttachEvent(View.ui.switches.icon, onPlaygroundEvent);
	AttachEvent(View.ui.hass.icon, onHassEvent);
	AttachEvent(View.ui.system.icon, onSystemEvent);
	AttachEvent(View.ui.setting.icon, onSettingEvent);
	// AttachEvent(View.ui.imu.icon);
	// AttachEvent(View.ui.battery.icon);
	// AttachEvent(View.ui.storage.icon);
}

void Menu::onViewDidLoad()
{

}

void Menu::onViewWillAppear()
{
	lv_indev_set_group(lv_get_indev(LV_INDEV_TYPE_ENCODER), View.ui.group);
	// StatusBar::SetStyle(StatusBar::STYLE_BLACK);

	timer = lv_timer_create(onTimerUpdate, 100, this);
	lv_timer_ready(timer);

	View.SetScrollToY(root, -LV_VER_RES, LV_ANIM_OFF);
	lv_obj_fade_in(root, 300, 0);
}

void Menu::onViewDidAppear()
{
	View.onFocus(View.ui.group);
}

void Menu::onViewWillDisappear()
{
	lv_obj_fade_out(root, 300, 0);
}

void Menu::onViewDidDisappear()
{
	lv_timer_del(timer);
}

void Menu::onViewDidUnload()
{
	View.Delete();
	Model.Deinit();
}

void Menu::AttachEvent(lv_obj_t* obj, lv_event_cb_t event_cb)
{
	lv_obj_set_user_data(obj, this);
	lv_obj_add_event_cb(obj, event_cb, LV_EVENT_ALL, this);
}

void Menu::Update()
{
	char buf[64];

	/* System */
	View.SetSystem(
		VERSION_FIRMWARE_NAME " " VERSION_SOFTWARE,
		VERSION_AUTHOR_NAME,
		VERSION_LVGL,
		"dummy time",
		VERSION_COMPILER,
		VERSION_BUILD_TIME
	);
}

void Menu::onTimerUpdate(lv_timer_t* timer)
{
	Menu* instance = (Menu*)timer->user_data;

	instance->Update();
}

void Menu::onPlaygroundEvent(lv_event_t* event)
{
	lv_obj_t* obj = lv_event_get_target(event);
	lv_event_code_t code = lv_event_get_code(event);
	auto* instance = (Menu*)lv_obj_get_user_data(obj);

	if (code == LV_EVENT_PRESSED) {
		// instance->Model.ChangeMotorMode(MOTOR_FINE_DETENTS);
		instance->Manager->Push("Pages/Playground");
	}
}


void Menu::onSystemEvent(lv_event_t* event)
{
	lv_obj_t* obj = lv_event_get_target(event);
	lv_event_code_t code = lv_event_get_code(event);
	auto* instance = (Menu*)lv_obj_get_user_data(obj);
	if (code == LV_EVENT_PRESSED) {
		Serial.printf("Power off...\n");
		HAL::power_off();
	}
}

void Menu::onSettingEvent(lv_event_t* event)
{
	lv_obj_t* obj = lv_event_get_target(event);
	lv_event_code_t code = lv_event_get_code(event);
	auto* instance = (Menu*)lv_obj_get_user_data(obj);
	if (code == LV_EVENT_PRESSED) {
		printf("Menu: onSystemEvent LV_EVENT_PRESSED\n");
		instance->Manager->Push("Pages/Setting");
	}
}


void Menu::onSuperDialEvent(lv_event_t* event)
{
	lv_obj_t* obj = lv_event_get_target(event);
	lv_event_code_t code = lv_event_get_code(event);
	auto* instance = (Menu*)lv_obj_get_user_data(obj);

	if (code == LV_EVENT_PRESSED) {
		// instance->Model.ChangeMotorMode(MOTOR_FINE_DETENTS);
//		int16_t mode = APP_MODE_SUPER_DIAL;
//		Stash_t stash;
//		stash.ptr = &mode;
//		stash.size = sizeof(int16_t);
//		instance->Manager->Push("Pages/SurfaceDial", &stash);
		instance->Manager->Push("Pages/SurfaceDial");
	}
}

void Menu::onHassEvent(lv_event_t* event)
{
	lv_obj_t* obj = lv_event_get_target(event);
	lv_event_code_t code = lv_event_get_code(event);
	auto* instance = (Menu*)lv_obj_get_user_data(obj);

	if (code == LV_EVENT_SHORT_CLICKED) {
		printf("Menu: onHassEvent LV_EVENT_PRESSED\n");
		instance->Manager->Push("Pages/Hass");
	}
}
