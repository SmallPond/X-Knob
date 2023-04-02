#ifndef __HOME_ASSISTANT_MODEL_H
#define __HOME_ASSISTANT_MODEL_H
#include "app/Utils/AccountSystem/Account.h"
#include "lvgl.h"
#include "../Playground/PlaygroundView.h"
#include "../Playground/PlaygroundModel.h"
#include <map>

typedef struct HassInfo : PlaygroundInfo
{
	/**
	 * 正在被操作的设备
	 */
	const char* busy_device_entity_id;
} HassInfo;

namespace Page
{

class HassModel: public PlaygroundModel
{

 public:
	HassModel();
	void Init();
	void Deinit();
	void Update(void* pg_ui);
	void GetKnobStatus(HassInfo* info);
	static std::map<std::string,std::string> getStateMap();
	static void setStateMap(const char* entity_id, const char* state);
	void setBusyDeviceEntityId(const char* entity_id);
	const char* getBusyDeviceEntityId();
	void ChangeMotorMode(int mode);
	void SetPlaygroundMode(int16_t mode);

};

}

#endif
