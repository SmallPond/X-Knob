#ifndef __SETTING_MODEL_H
#define __SETTING_MODEL_H
#include "app/Utils/AccountSystem/Account.h"
#include "lvgl.h"
#include "hal/nvs.h"
#include "../Playground/PlaygroundView.h"
#include "../Playground/PlaygroundModel.h"
#include <map>

typedef struct SettingInfo : PlaygroundInfo
{
} SettingInfo;

namespace Page
{

class SettingModel: public PlaygroundModel
{

public:
    SettingModel();
    void Init();
    void Deinit();
    void Update(void* pg_ui);
    void GetKnobStatus(SettingInfo* info);
    static std::map<std::string,std::string> getStateMap();
    static void setStateMap(const char* entity_id, const char* state);
    void ChangeMotorMode(int mode);
    void SetPlaygroundMode(int16_t mode);
    void SaveLcdBkTimeout(int value);
    void SaveLcdBkBrightness(int value);
};

}

#endif
