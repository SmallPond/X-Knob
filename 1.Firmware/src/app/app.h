#ifndef __APP_H__
#define __APP_H__
#include "hal/motor.h"
#define ACCOUNT_SEND_NOTIFY_CMD(ACT, CMD)\
do{\
    AccountSystem::ACT##_Info_t info;\
    memset(&info, 0, sizeof(info));\
    info.cmd = AccountSystem::CMD;\
    AccountSystem::Broker()->AccountMaster.Notify(#ACT, &info, sizeof(info));\
}while(0)

#ifdef ARDUINO
#include "port/display.h"
#define INIT_DONE() \
do{                 \
    xTaskNotifyGive(handleTaskLvgl); \
}while(0) \

#else
#define INIT_DONE() \
do{                 \
}while(0) \

#endif

#ifndef offsetof
    #define offsetof(type, member) ((size_t)&reinterpret_cast<char const volatile&>        
          ((((type*)0)->member)))
#endif

#ifndef container_of
#define container_of(ptr, type, member) ({          								\
     const typeof( ((type *)0)->member ) *__mptr = (ptr);    	\
     (type *)( (char *)__mptr - offsetof(type,member) );})
#endif


void display_init();

void App_Init();
void App_UnInit();


enum PLAYGROUND_MODE {
    PLAYGROUND_MODE_NO_EFFECTS,
    PLAYGROUND_MODE_FINE_DETENTS,
    PLAYGROUND_MODE_BOUND ,
    PLAYGROUND_MODE_ON_OFF,
    PLAYGROUND_MODE_MAX,
};

enum APP_MODE {
    APP_MODE_SUPER_DIAL = PLAYGROUND_MODE_MAX,
    APP_MODE_HOME_ASSISTANT,
    APP_MODE_SETTING,
    APP_MODE_MAX,
};

enum SETTING_MODE {
    SETTING_MODE_LCD_BK_BRIGHTNESS = APP_MODE_MAX,
    SETTING_MODE_LCD_BK_TIMEOUT,
    SETTING_MODE_MAX,
};

#endif /* __APP_H__ */