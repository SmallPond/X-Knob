#include "SurfaceDialView.h"

using namespace Page;

/*
 * 默认视图显示： 圆点，原点所在位置 label_value
 * 此函数根据需要增加或 hidden 对象
*/
void SurfaceDialView::SetPlaygroundMode(int16_t mode)
{
	lv_obj_add_flag(ui.label_value, LV_OBJ_FLAG_HIDDEN);
	lv_meter_set_scale_ticks(ui.meter, ui.scale_pot, 73, 2, 0, lv_color_black());
	lv_meter_set_scale_range(ui.meter, ui.scale_pot, 0, 72, 360, 270);
}
		

void SurfaceDialView::UpdateView(SurfaceDialInfo *info)
{

	// static bool is_connected = false;
	int32_t motor_pos = info->motor_pos;
	motor_pos = motor_pos % (360/5); // 5 为surface dial 电机模式最小角度
	if (motor_pos < 0) {
		motor_pos = 360/5 + motor_pos;
	}
	lv_meter_set_indicator_value(ui.meter, ui.nd_img_circle, motor_pos);

	if (info->is_ble_connected) {
		lv_color_t color = info->is_ble_connected ? lv_color_make(0x00, 0x82, 0xfc):lv_color_white();
		lv_obj_set_style_img_recolor_opa(m_ui.img_super_dial, LV_OPA_COVER, 0);
		lv_obj_set_style_img_recolor(m_ui.img_super_dial, color, 0);
		// is_connected = info->is_ble_connected;
	}
}

void SurfaceDialView::Create(lv_obj_t* root)
{
	PlaygroundView::Create(root);

	m_ui.img_super_dial = lv_img_create(root);
    lv_img_set_src(m_ui.img_super_dial, Resource.GetImage("dialpad"));
	lv_obj_align(m_ui.img_super_dial, LV_ALIGN_CENTER, 0, 0);
}

void SurfaceDialView::Delete()
{
	lv_group_del(ui.group);
	lv_style_reset(&style.meter);
	lv_style_reset(&style.ticks);
}