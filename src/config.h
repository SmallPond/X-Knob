#ifndef __CONFIG_H__
#define __CONFIG_H__

#define SCLK 12
#define MOSI 11
#define TFT_CS 10
#define TFT_BLK 13
#define TFT_DC 14
#define TFT_RST 9

#define CONFIG_SCREEN_HOR_RES       240
#define CONFIG_SCREEN_VER_RES       240
#define CONFIG_SCREEN_BUFFER_SIZE   (CONFIG_SCREEN_HOR_RES * CONFIG_SCREEN_VER_RES /2)

#define PUSH_BUTTON_PIN 5

#endif 