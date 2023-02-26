#include "display.h"
#include "config.h"
#include "hal/hal.h"

TaskHandle_t handleTaskLvgl;
void TaskLvglUpdate(void* parameter)
{
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    for (;;)
    {
        lv_task_handler();

        delay(5);
    }
}

void display_init(void)
{
    Arduino_DataBus *bus = new Arduino_HWSPI(TFT_DC, TFT_CS, SCLK, MOSI, MISO);
    static SCREEN_CLASS screen(bus, TFT_RST, 2 /* 屏幕方向 0: 0, 1: 90, 2: 180*/, true /* IPS */);
    // Init Display
    screen.begin();
    screen.fillScreen(BLACK);
#ifdef TFT_BLK
    pinMode(TFT_BLK, OUTPUT);
    digitalWrite(TFT_BLK, HIGH);
#endif
    lv_init();
    lv_port_disp_init(&screen);
    lv_port_indev_init();
    // Update display in parallel thread.
    xTaskCreatePinnedToCore(
        TaskLvglUpdate,
        "LvglThread",
        20000,
        nullptr,
        configMAX_PRIORITIES - 1,
        &handleTaskLvgl,
        LVGL_RUNNING_CORE);

}