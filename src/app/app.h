#ifndef __APP_H__
#define __APP_H__

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

void display_init();

void App_Init();
void App_UnInit();

#endif /* __APP_H__ */