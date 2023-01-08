#include "PlaygroundView.h"

using namespace Page;


static int32_t MAX_VALUE = 100;
static int32_t MIN_VALUE = 0;
static int32_t SCALE_LEFT_BOUND_TICKS = 200;
static int32_t SCALE_ANGLE_RANGE = 140;
static int32_t SCALE_RIGHT_BOUND_TICKS = SCALE_LEFT_BOUND_TICKS + SCALE_ANGLE_RANGE;
static int32_t ARC_START_ROTATION = 180;

void Page::PlaygroundView::SetPlaygroundMode(int16_t mode)
{
	playgroundMode = mode;
	switch (playgroundMode) {
		case PLAYGROUND_MODE_UNBOUND:
			// This mode is default
			break;
		case PLAYGROUND_MODE_BOUND:
		    MIN_VALUE = 0;
			MAX_VALUE = 12;
			BoundView();
			break;
		default:
			break;
	}

}

void Page::PlaygroundView::UpdateView(PlaygroundMotorInfo *info)

{
	lv_label_set_text_fmt(
		ui.lable_value,
		"%d",
		info->xkonb_value
	);
	int32_t motor_pos = info->motor_pos;
	motor_pos = motor_pos % 360;
	if (motor_pos < 0) {
		motor_pos = 360 + motor_pos;
	}
	lv_meter_set_indicator_value(ui.meter, ui.nd_img_circle, motor_pos);
	// lv_style_set_bg_main_stop(&style.meter, 255-xkonb_value);
	// lv_style_set_bg_grad_stop(&style.meter, 255-xkonb_value);
	int value_map = map(info->xkonb_value, 0, MAX_VALUE, 0, 255);
	lv_obj_set_style_bg_main_stop(ui.meter, 255 - value_map, 0);
	lv_obj_set_style_bg_grad_stop(ui.meter, 255 - value_map, 0);


	switch (playgroundMode) {
		case PLAYGROUND_MODE_UNBOUND:
	    // This mode is default
		break;
		case PLAYGROUND_MODE_BOUND:
			if (info->angle_offset != 0) {
				if (info->xkonb_value == MIN_VALUE) {
					lv_meter_set_indicator_start_value(ui.meter, ui.arc, SCALE_LEFT_BOUND_TICKS - ARC_START_ROTATION - info->angle_offset);
					lv_meter_set_indicator_end_value(ui.meter, ui.arc, SCALE_LEFT_BOUND_TICKS - ARC_START_ROTATION);
				} else {
					lv_meter_set_indicator_start_value(ui.meter, ui.arc, SCALE_RIGHT_BOUND_TICKS - ARC_START_ROTATION);
					lv_meter_set_indicator_end_value(ui.meter, ui.arc, SCALE_RIGHT_BOUND_TICKS - ARC_START_ROTATION - info->angle_offset);
				}

			} else {
				lv_meter_set_indicator_start_value(ui.meter, ui.arc, 0);
				lv_meter_set_indicator_end_value(ui.meter, ui.arc, 0);
			}
			
			break;
		default:
			break;
	}
}

void PlaygroundView::CreateBoundView(void)
{

}
void PlaygroundView::BoundView(void)
{
	lv_meter_set_scale_ticks(ui.meter, ui.scale_pot, 13, 2, 0, lv_color_make(0xff, 0xff, 0xff));
	lv_meter_set_scale_major_ticks(ui.meter, ui.scale_pot, 12, 4, 20, lv_color_make(0xff, 0xff, 0xff), 10);
	lv_meter_set_scale_range(ui.meter, ui.scale_pot, 0, 12, SCALE_ANGLE_RANGE, SCALE_LEFT_BOUND_TICKS);

	// display out of bounds
	lv_meter_set_scale_ticks(ui.meter, ui.scale_arc, 40, 0, 0, lv_color_make(0xff, 0x00, 0x00));
	lv_meter_set_scale_range(ui.meter, ui.scale_arc, 0, 360, 360, ARC_START_ROTATION);
	lv_meter_set_indicator_start_value(ui.meter, ui.arc, 0);
	lv_meter_set_indicator_end_value(ui.meter, ui.arc, 0);

}



void PlaygroundView::Create(lv_obj_t* root)
{
	lv_obj_remove_style_all(root);
	lv_obj_set_size(root, LV_HOR_RES, LV_VER_RES);
	lv_obj_set_style_bg_color(root, lv_color_black(), 0);
	lv_obj_set_style_bg_opa(root, LV_OPA_COVER, 0);
	
	//Write style state: LV_STATE_DEFAULT for style_screen_main_main_default
	// static lv_style_t style_screen_main_main_default;
	// lv_style_init(&style_screen_main_main_default);
	// lv_style_set_bg_color(&style_screen_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	// lv_style_set_bg_opa(&style_screen_main_main_default, 0);
	// lv_obj_add_style(root, &style_screen_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes meter
	ui.meter = lv_meter_create(root);
	lv_obj_set_pos(ui.meter, 1, 0);
	lv_obj_set_size(ui.meter, LV_HOR_RES, LV_VER_RES);
	lv_obj_set_scrollbar_mode(ui.meter, LV_SCROLLBAR_MODE_OFF);

	//add scale ui.scale_circle
	ui.scale_pot = lv_meter_add_scale(ui.meter);
	lv_meter_set_scale_ticks(ui.meter, ui.scale_pot, 41, 2, 0, lv_color_make(0xff, 0x00, 0x00));
	lv_meter_set_scale_range(ui.meter, ui.scale_pot, 0, 360, 360, 270);

	//add needle img for ui.scale_circle
	ui.nd_img_circle = lv_meter_add_needle_img(ui.meter, ui.scale_pot,Resource.GetImage("dot_blue"), -100, 8);
	lv_meter_set_indicator_value(ui.meter, ui.nd_img_circle, 0);

	//add scale ui.scale_value
	ui.scale_arc = lv_meter_add_scale(ui.meter);
	lv_meter_set_scale_ticks(ui.meter, ui.scale_arc, 41, 0, 0, lv_color_make(0xff, 0x00, 0x00));
	lv_meter_set_scale_range(ui.meter, ui.scale_arc, 0, 100, 360, ARC_START_ROTATION);

	//add arc for ui.scale_value
	ui.arc = lv_meter_add_arc(ui.meter, ui.scale_arc, 6, lv_color_make(0xff, 0x00, 0x00), 5);
	lv_meter_set_indicator_start_value(ui.meter, ui.arc, 0);
	lv_meter_set_indicator_end_value(ui.meter, ui.arc, 0);

	//Write style state: LV_STATE_DEFAULT for meter_style
	lv_style_init(&style.meter);
	lv_style_set_radius(&style.meter, 240);
	lv_style_set_bg_color(&style.meter, lv_color_black());
	lv_style_set_bg_grad_color(&style.meter, lv_color_make(128,0,128));
	lv_style_set_bg_grad_dir(&style.meter, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style.meter, 255);
	lv_style_set_bg_main_stop(&style.meter, 200);
	lv_style_set_bg_grad_stop(&style.meter, 200);
	// lv_style_set_border_color(&style.meter, lv_color_make(0x12, 0x11, 0x12));
	lv_style_set_border_width(&style.meter, 0);
	lv_style_set_border_opa(&style.meter, 255);
	lv_obj_add_style(ui.meter, &style.meter, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_meter_1_main_ticks_default
	lv_style_init(&style.ticks);
	lv_style_set_text_color(&style.ticks, lv_color_white());
	lv_style_set_text_font(&style.ticks, &lv_font_montserrat_14);
	lv_obj_add_style(ui.meter, &style.ticks, LV_PART_TICKS|LV_STATE_DEFAULT);

	//Write codes screen_label_1

	ui.lable_value = lv_label_create(root);
	lv_obj_set_pos(ui.lable_value, 70, 67);
	lv_obj_set_size(ui.lable_value, 100, 32);
	lv_obj_set_scrollbar_mode(ui.lable_value, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui.lable_value, "0");
	lv_label_set_long_mode(ui.lable_value, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for lable_style
	static lv_style_t lable_style;
	lv_style_init(&lable_style);
	lv_style_set_radius(&lable_style, 0);
	lv_style_set_bg_color(&lable_style, lv_color_make(0x0f, 0x0f, 0x0f));
	lv_style_set_bg_grad_color(&lable_style, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&lable_style, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&lable_style, 0);  // 完全透明
	lv_style_set_text_color(&lable_style, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&lable_style, &lv_font_montserrat_26);
	lv_style_set_text_letter_space(&lable_style, 2);
	lv_style_set_text_line_space(&lable_style, 0);
	lv_style_set_text_align(&lable_style, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&lable_style, 0);
	lv_style_set_pad_right(&lable_style, 0);
	lv_style_set_pad_top(&lable_style, 8);
	lv_style_set_pad_bottom(&lable_style, 0);
	lv_obj_add_style(ui.lable_value, &lable_style, LV_PART_MAIN|LV_STATE_DEFAULT);

	ui.group = lv_group_create();
	lv_indev_set_group(lv_get_indev(LV_INDEV_TYPE_ENCODER), ui.group);

	lv_group_add_obj(ui.group, ui.meter);
	lv_group_focus_obj(ui.meter);

}
