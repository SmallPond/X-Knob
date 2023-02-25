/**
 * @file lv_port_indev_templ.c
 *
 */

/*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/

#include "lvgl.h"
#include "hal/hal.h"


static void encoder_init(void);
static void encoder_read(lv_indev_drv_t* indev_drv, lv_indev_data_t* data);

/**********************
 *  STATIC VARIABLES
 **********************/

static lv_indev_t* encoder_indev;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
static lv_indev_drv_t indev_drv;

void lv_port_indev_init(void)
{

    /*------------------
     * Encoder
     * -----------------*/

    /*Initialize your encoder if you have*/
    encoder_init();

    /*Register a encoder input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read;
    indev_drv.long_press_time = 2000;           // 按下 2s 为长按
    indev_drv.long_press_repeat_time = 500;    // 间隔 0.5s 发送LV_EVENT_LONG_PRESSED_REPEAT 事件
    encoder_indev = lv_indev_drv_register(&indev_drv);

    /* Later you should create group(s) with `lv_group_t * group = lv_group_create()`,
     * add objects to the group with `lv_group_add_obj(group, obj)`
     * and assign this input device to group to navigate in it:
     * `lv_indev_set_group(indev_encoder, group);` */
}


/**********************
 *   STATIC FUNCTIONS
 **********************/


/* Initialize your keypad */
static void encoder_init(void)
{
    /*Do it in Init*/
}


/* Will be called by the library to read the encoder */
static void encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    // data->enc_diff = HAL::Encoder_GetDiff();
    // data->state =  HAL::Encoder_GetIsPush() ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
    static int now_num = 0;
    static int old_num = 0;
    now_num = HAL::get_motor_position();

    data->enc_diff = 0;
    if (HAL::is_encoder_enabled()) {
        if (now_num > old_num) {
            data->enc_diff++;
            old_num = HAL::get_motor_position();
        } else if (now_num < old_num) {
            data->enc_diff--;
            old_num = HAL::get_motor_position();
        }

    }

    data->state =  HAL::encoder_is_pushed() ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
}

