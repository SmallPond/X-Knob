#ifndef __APP_H__
#define __APP_H__


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
#endif /* __APP_H__ */