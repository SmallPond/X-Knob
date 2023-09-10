#include "HassView.h"
#include "Hass.h"
#include "HassModel.h"

using namespace Page;
#define ITEM_PAD 60
/*
 * 默认视图显示： 圆点，原点所在位置 label_value
 * 此函数根据需要增加或 hidden 对象
*/
void HassView::SetPlaygroundMode(int16_t mode)
{
	lv_obj_add_flag(ui.label_value, LV_OBJ_FLAG_HIDDEN);
	lv_meter_set_scale_ticks(ui.meter, ui.scale_pot, 73, 2, 0, lv_color_make(0xff, 0x00, 0x00));
	lv_meter_set_scale_range(ui.meter, ui.scale_pot, 0, 72, 360, 270);
}

void HassView::UpdateFocusedDevice(const char* name)
{
	lv_label_set_text(m_ui->foucs_label, name);
}

void HassView::SetCtrView(lv_obj_t* obj)
{
	device_t *device = device_map[obj];

	if (device->is_set_value) {
		/*
		 * temporarily no display
		 * we can't get the status of the device
		*/  
	} if (!device->is_set_value && device->is_on_off) {
		PlaygroundView::OnOffView();
		current_view = VIEW_MODE_ON_OFF;
	}
	printf("set view: %d\n",current_view);
	lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
}
/*
 * Gets the name of the controlled device
*/
char* HassView::GetEditedDeviceName(void)
{
	std::map<lv_obj_t*, device_t *>::iterator iter;
    iter = device_map.begin();
    while(iter != device_map.end()) {
        if (lv_obj_has_state(iter->first, LV_STATE_EDITED)) {
			lv_obj_t* label = lv_obj_get_child(iter->first, 1);
			if (label != NULL) {
				return lv_label_get_text(label);
			}
		}
		iter++;
    }
	return NULL;
}

void HassView::ClearCtrView(lv_obj_t *obj)
{
	device_t *device = device_map[obj];

	if (device->is_set_value) {
		/*
		 * temporarily no display
		 * we can't get the status of the device
		*/  
	} if (!device->is_set_value && device->is_on_off) {
		PlaygroundView::DefaultView();
		current_view = 0;
	}

	lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, 0);
}
int HassView::GetViewMode(void)
{
	return current_view;
}
void HassView::UpdateCtrlView(HassInfo* info)
{
	int _value = 0;
	int32_t motor_pos = info->motor_pos;
	lv_meter_set_indicator_value(ui.meter, ui.nd_img_circle, motor_pos);

	switch (current_view) {
		case VIEW_MODE_ON_OFF:
			_value = info->xkonb_value;
			HassView::UpdateBackgroundView(info);
			break;
		default:
			break;
	}

	lv_label_set_text_fmt(
		ui.label_value,
		"%s",
		_value?"ON":"OFF"
	);
}
void HassView::UpdateView(HassInfo* info)
{
	if (current_view) {
		UpdateCtrlView(info);
		return;
	}
	int32_t motor_pos = info->motor_pos;
	motor_pos = motor_pos % (360/5); // 5 为surface dial 电机模式最小角度
	if (motor_pos < 0) {
		motor_pos = 360/5 + motor_pos;
	}
	lv_meter_set_indicator_value(ui.meter, ui.nd_img_circle, motor_pos);

}

void HassView::device_item_create(
    device_t* item,
	lv_obj_t* par,
	const char* name,
	const char* img_src,
	bool is_on_off,
	bool is_set_value)
{

    lv_obj_t *cont = lv_obj_create(par);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);

    lv_obj_t *img = lv_img_create(cont);
    lv_img_set_src(img, Resource.GetImage(img_src));

    lv_obj_set_style_img_recolor_opa(img, LV_OPA_COVER, 0);
    lv_obj_set_style_img_recolor(img, lv_color_white(), 0);

    item->l_dev_name = lv_label_create(cont);
    lv_label_set_text_fmt(item->l_dev_name, "%s", name);
    lv_obj_set_size(item->l_dev_name, 0, 0);

    lv_obj_add_style(cont, &style.focus, LV_STATE_FOCUSED);
    lv_obj_add_style(cont, &style.cont, 0);

    lv_obj_add_style(cont, &style.edit, LV_STATE_EDITED);
    
    item->cont = cont;

    lv_obj_set_flex_align(
		cont,
		LV_FLEX_ALIGN_CENTER,
		LV_FLEX_ALIGN_CENTER,
		LV_FLEX_ALIGN_CENTER
	);

	item->is_on_off = is_on_off;
	item->is_set_value = is_set_value;
	device_map[cont] = item;
}

void on_focus(lv_group_t* g)
{
	lv_obj_t* cont = lv_group_get_focused(g);
	lv_obj_t* cont_row = lv_obj_get_parent(cont);
	lv_coord_t x = lv_obj_get_x(cont);
    lv_coord_t width = lv_obj_get_width(cont);
	lv_obj_scroll_to_x(cont_row, x, LV_ANIM_ON);
}

void HassView::group_init(void)
{
	m_ui->group = lv_group_create();
	lv_group_set_focus_cb(m_ui->group, on_focus);
	lv_indev_set_group(lv_get_indev(LV_INDEV_TYPE_ENCODER), m_ui->group);

	for(int i = 0; i < DEVICE_NUM; i++) {
		lv_group_add_obj(m_ui->group, (m_ui->devices[i]).cont);
	}
	
}

void HassView::style_init(void)
{
    lv_style_init(&style.cont);
    lv_style_set_width(&style.cont, ITEM_PAD);
    lv_style_set_height(&style.cont, ITEM_PAD+10);
    lv_style_set_bg_color(&style.cont, lv_color_black());
    lv_style_set_bg_opa(&style.cont, LV_OPA_TRANSP);
    lv_style_set_text_font(&style.cont, Resource.GetFont("bahnschrift_17"));
    lv_style_set_text_color(&style.cont, lv_color_white());

    lv_style_init(&style.focus);
    lv_style_set_border_side(&style.focus, LV_BORDER_SIDE_FULL);
    lv_style_set_border_width(&style.focus, 2);
    lv_style_set_border_color(&style.focus, lv_color_hex(0x9130CC));
    // lv_obj_set_flex_flow(&style.cont, LV_FLEX_FLOW_COLUMN);

    lv_style_init(&style.edit);

    // lv_style_img_recolor_opa(&style.edit, LV_OPA_COVER);
    // lv_style_img_recolor(&style.edit, lv_color_hex(0xFF0633));
    lv_style_set_bg_color(&style.edit, lv_color_hex(0x870AEA));


    // static const lv_style_prop_t style_prop[] =
	// {
	// 	LV_STYLE_WIDTH,
	// 	LV_STYLE_PROP_INV
	// };

	// static lv_style_transition_dsc_t trans;
	// lv_style_transition_dsc_init(
	// 	&trans,
	// 	style_prop,
	// 	lv_anim_path_overshoot,
	// 	500,
	// 	0,
	// 	NULL
	// );
	// lv_style_set_transition(&style.focus, &trans);
	// lv_style_set_transition(&style.cont, &trans);

	lv_style_init(&style.label_name);
	lv_style_set_text_font(&style.label_name, Resource.GetFont("bahnschrift_22"));
	lv_style_set_text_color(&style.label_name, lv_color_white());
}
void HassView::Create(lv_obj_t* root)
{
	PlaygroundView::Create(root);

	lv_obj_t * cont_row = lv_obj_create(root);
    lv_obj_remove_style_all(cont_row);
    lv_obj_set_size(cont_row, ITEM_PAD * 3, 120);
    lv_obj_align(cont_row, LV_ALIGN_CENTER, 0, 5);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(
		cont_row,
		LV_FLEX_ALIGN_START,
		LV_FLEX_ALIGN_START,
		LV_FLEX_ALIGN_CENTER
	);
    lv_obj_set_style_pad_column(cont_row, 10, 0);
    lv_obj_set_style_pad_hor(cont_row, ITEM_PAD, 0);

    // lv_obj_remove_style(obj, style, LV_PART_SCROLLBAR)
    style_init();

    m_ui = (device_ui *)malloc(sizeof(device_ui) + DEVICE_NUM * sizeof(device_t)); 
	if (m_ui == NULL) {
		printf("malloc m_ui failed\n");
		return;
	}

    device_item_create(
      &(m_ui->devices[0]),
      cont_row,
      "Light",
      "home_bulb",
	  true,
	  true
    );

	device_item_create(
      &(m_ui->devices[1]),
      cont_row,
      "Fan",
      "home_fan",
	  true,
	  false
    );

    device_item_create(
      &(m_ui->devices[2]),
      cont_row,
      "Air Conditioning",
      "home_air_cond",
	  false,
	  false
    );

    device_item_create(
      &(m_ui->devices[3]),
      cont_row,
      "Wash Machine",
      "home_wash_machine",
	  false,
	  false
    );

    m_ui->foucs_label = lv_label_create(root);
	lv_obj_add_style(m_ui->foucs_label, &style.label_name, 0);
    lv_label_set_text_fmt(m_ui->foucs_label, "%s", "NONE");
    lv_obj_align(m_ui->foucs_label, LV_ALIGN_CENTER, 0, ITEM_PAD+10);
	
    // lv_obj_set_size(m_ui->foucs_label, 42, 20);
    // lv_obj_center(m_ui->foucs_label);

    group_init();
}


void HassView::Delete()
{
	lv_group_del(m_ui->group);
	lv_style_reset(&style.cont);
	lv_style_reset(&style.focus);
	lv_style_reset(&style.edit);
	lv_style_reset(&style.label_name);
	PlaygroundView::Delete();
}