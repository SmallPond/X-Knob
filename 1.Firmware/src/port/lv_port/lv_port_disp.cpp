#include <Arduino.h>
#include <lvgl.h>
#include <Wire.h>
#include <SPI.h>
#include "port/display.h"
#include "config.h"

#define DISP_BUF_SIZE        CONFIG_SCREEN_BUFFER_SIZE
static uint32_t screenWidth;
static uint32_t screenHeight;
static lv_disp_draw_buf_t draw_buf;
lv_color_t *disp_draw_buf;
static lv_disp_drv_t disp_drv;
uint8_t lv_page = 0;



static void disp_flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    SCREEN_CLASS* screen = (SCREEN_CLASS*) disp->user_data;
    screen->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);

    lv_disp_flush_ready(disp);
}

void my_print(lv_log_level_t level, const char* file, uint32_t line, const char* fun, const char* dsc)
{

    // Serial.printf("%s@%d %s->%s\r\n", file, line, fun);
    // Serial.flush();
    
}


void lv_port_disp_init(SCREEN_CLASS* scr) {


    lv_log_register_print_cb(
        reinterpret_cast<lv_log_print_g_cb_t>(my_print)); /* register print function for debugging */

    screenWidth = scr->width();
    screenHeight = scr->height();
    // disp_draw_buf = (lv_color_t *)malloc(sizeof(lv_color_t) * screenWidth * 10);
    // if (!disp_draw_buf) {
    //     LV_LOG_WARN("LVGL disp_draw_buf allocate failed!");
    // }
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, DISP_BUF_SIZE);

    /* Initialize the display */
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = disp_flush_cb;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.user_data = scr;
    lv_disp_drv_register(&disp_drv);
    
    /* Initialize the (dummy) input device driver */
    // ui_init();
    // lv_img_set_pivot(ui_Image2, 16, 120); /*Rotate around the top left corner*/
    // lv_meter_set_indicator_value(meter, line_indic, 50);
}

