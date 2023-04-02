#ifndef __HASS_H__
#define __HASS_H__

#include "HassModel.h"
#include "HassView.h"

void hass_hal_init(void);
int hass_hal_send(const char *device_name, int knob_value);

namespace Page
{
	class Hass : public PageBase
	{
	 public:
		Hass();
		virtual ~Hass();

		virtual void onCustomAttrConfig();
		virtual void onViewLoad();
		virtual void onViewDidLoad();
		virtual void onViewWillAppear();
		virtual void onViewDidAppear();
		virtual void onViewWillDisappear();
		virtual void onViewDidDisappear();
		virtual void onViewDidUnload();

		void HassEventHandler(lv_event_t* event, lv_event_code_t code);

	 private:
		void Update();
		void AttachEvent(lv_obj_t* obj);
		static void onTimerUpdate(lv_timer_t* timer);
		static void onEvent(lv_event_t* event);

	 private:
		HassView* View;
		HassModel* Model;
		lv_timer_t* timer;
	};
}


#endif /*   __HASS_H__ */