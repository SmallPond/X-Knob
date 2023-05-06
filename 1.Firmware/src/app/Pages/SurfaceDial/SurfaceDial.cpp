#include "SurfaceDial.h"
using namespace Page;

const int motor_mode = MOTOR_UNBOUND_COARSE_DETENTS;
const int app_mode = APP_MODE_SUPER_DIAL;

SurfaceDial::SurfaceDial()
{
}

SurfaceDial::~SurfaceDial()
{
}

void SurfaceDial::onCustomAttrConfig()
{
	SetCustomCacheEnable(false);
}

void SurfaceDial::onViewLoad()
{
	Model = new SurfaceDialModel();
	View = new SurfaceDialView();
	HAL::surface_dial_init();

	Model->Init();
	View->Create(root);

	AttachEvent(root);
	AttachEvent(View->ui.meter);
}

void SurfaceDial::onViewDidAppear()
{

}

void SurfaceDial::onViewWillDisappear()
{

}

void SurfaceDial::onViewDidDisappear()
{
	lv_timer_del(timer);
}

void SurfaceDial::onViewDidUnload()
{
	View->Delete();
	Model->Deinit();

	delete View;
	delete Model;
}

void SurfaceDial::SurfaceDialEventHandler(lv_event_t* event, lv_event_code_t code)
{
	if (code == LV_EVENT_SHORT_CLICKED) {
		HAL::surface_dial_press();
	} else if (code == LV_EVENT_LONG_PRESSED_REPEAT) {
		// return to memu
		Model->ChangeMotorMode(MOTOR_UNBOUND_COARSE_DETENTS);
		Manager->Pop();
	} else if (code == LV_EVENT_RELEASED) {
		HAL::surface_dial_release();
	}
}
void SurfaceDial::AttachEvent(lv_obj_t* obj)
{
	lv_obj_set_user_data(obj, this);
	lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void SurfaceDial::onEvent(lv_event_t* event)
{
	lv_obj_t* obj = lv_event_get_target(event);
	lv_event_code_t code = lv_event_get_code(event);
	auto* instance = (SurfaceDial*)lv_obj_get_user_data(obj);
	instance->SurfaceDialEventHandler(event, code);
}

void SurfaceDial::onViewDidLoad()
{
}

void SurfaceDial::onViewWillAppear()
{
	Model->ChangeMotorMode(motor_mode);
	Model->SetPlaygroundMode(app_mode);
	View->SetPlaygroundMode(app_mode);
	timer = lv_timer_create(onTimerUpdate, 10, this);
}

void SurfaceDial::onTimerUpdate(lv_timer_t* timer)
{
	SurfaceDial* instance = (SurfaceDial*)timer->user_data;
	instance->Update();
}

void SurfaceDial::Update()
{
	SurfaceDialInfo info;
	Model->GetKnobStatus(&info);
	HAL::surface_dial_update(info.konb_direction);
	View->UpdateView(&info);
}