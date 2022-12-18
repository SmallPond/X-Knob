// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.0.5
// LVGL VERSION: 8.2
// PROJECT: SquareLine_Project

#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if __has_include("lvgl.h")
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

extern lv_obj_t * ui_Screen1;
extern lv_obj_t * ui_Image1;
extern lv_obj_t * ui_Image2;
extern lv_obj_t * ui_Button1;
extern lv_obj_t * ui_Screen2;
extern lv_obj_t * ui_Button2;
extern lv_obj_t* meter;
extern lv_meter_indicator_t* line_indic;

LV_IMG_DECLARE(ui_img_miku1_png);    // assets\miku1.png
LV_IMG_DECLARE(ui_img_onion2_png);    // assets\onion2.png




void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
