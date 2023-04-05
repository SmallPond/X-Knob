#ifndef SURFACEDIAL_H_
#define SURFACEDIAL_H_
#include "SurfaceDialView.h"
#include "SurfaceDialModel.h"
namespace Page
{
	class SurfaceDial : public PageBase
	{
	 public:
		SurfaceDial();
		virtual ~SurfaceDial();

		virtual void onCustomAttrConfig();
		virtual void onViewLoad();
		virtual void onViewDidLoad();
		virtual void onViewWillAppear();
		virtual void onViewDidAppear();
		virtual void onViewWillDisappear();
		virtual void onViewDidDisappear();
		virtual void onViewDidUnload();
		void SurfaceDialEventHandler(lv_event_t* event, lv_event_code_t code);

	 private:
		void AttachEvent(lv_obj_t* obj);
		static void onEvent(lv_event_t* event);
		static void onTimerUpdate(lv_timer_t* timer);
		void Update();

	 private:
		SurfaceDialView* View;
		SurfaceDialModel* Model;
		lv_timer_t* timer;
	};
}
#endif //SURFACEDIAL_H_
