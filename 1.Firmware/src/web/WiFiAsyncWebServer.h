#ifndef __WIFIASYNCWEBSERVER_H__
#define __WIFIASYNCWEBSERVER_H__

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include "ArduinoJson.h"
#include "lvgl.h"
#include "config.h"
#include "hal/nvs.h"

typedef struct {
    lv_obj_t* ui_wifi_label;
} TaskParams;

#define FILESYSTEM FFat
// You only need to format the filesystem once
#define FORMAT_FILESYSTEM_IF_FAILED true
#define Serial Serial

#if FILESYSTEM == FFat
#include <FFat.h>
#endif
#if FILESYSTEM == SPIFFS
#include <SPIFFS.h>
#endif

#define MAX_STA_COUNT 20   //0.5*20 = 10s
extern AsyncWebServer *server;  // initialise webserver
void wifi_server_begin(void *parameter);
void close_server();
void ffat_init();
#endif