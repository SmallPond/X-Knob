#include "SettingView.h"
#include "Setting.h"
#include "SettingModel.h"

using namespace Page;
#define ITEM_PAD 60
/*
* 默认视图显示： 圆点，原点所在位置 label_value
* 此函数根据需要增加或 hidden 对象
*/
void SettingView::SetPlaygroundMode(int16_t mode)
{
    // lv_label_set_text(ui.label_value, "Smart Home");
    lv_meter_set_scale_ticks(ui.meter, ui.scale_pot, 73, 2, 0, lv_color_make(0xff, 0x00, 0x00));
    lv_meter_set_scale_range(ui.meter, ui.scale_pot, 0, 72, 360, 270);
    SetWiFiViewVisible(false);
    switch(mode){
        case DEFAULT_VIEW:
            PlaygroundView::DefaultView();
            break;
        case WIFI_SETTING_VIEW:
            SetWiFiViewVisible(true);
            break;
        case LCD_BK_TIMEOUT_SETTING_VIEW:
            MIN_VALUE = 0;
            MAX_VALUE = 30;
            SCALE_LEFT_BOUND_TICKS = 220;
            SCALE_ANGLE_RANGE = 100;
            PlaygroundView::BoundZeroView();
            lv_obj_clear_flag(ui.label_value, LV_OBJ_FLAG_HIDDEN);
            break;
        case LCD_BK_BRIGHTNESS_SETTING_VIEW:
            MIN_VALUE = 10;
            MAX_VALUE = 100;
            SCALE_LEFT_BOUND_TICKS = 220;
            SCALE_ANGLE_RANGE = 100;
            PlaygroundView::BoundZeroView();
            lv_obj_clear_flag(ui.label_value, LV_OBJ_FLAG_HIDDEN);
            break;
        default:
            lv_obj_add_flag(ui.label_value, LV_OBJ_FLAG_HIDDEN);
            break;
    }
}

void SettingView::UpdateFocusedSetting(const char* name)
{
    lv_label_set_text(m_ui->foucs_label, name );
}

void SettingView::SetCtrView(lv_obj_t* obj)
{
    setting_t *setting = setting_map[obj];
    
    switch (setting->num) {
        case DEFAULT_VIEW:
            PlaygroundView::DefaultView();
            break;
        case WIFI_SETTING_VIEW:
            break;
        case LCD_BK_TIMEOUT_SETTING_VIEW:
            current_view = LCD_BK_TIMEOUT_SETTING_VIEW;
            break;
        case LCD_BK_BRIGHTNESS_SETTING_VIEW:
            current_view = LCD_BK_BRIGHTNESS_SETTING_VIEW;
            break;
        default:
            break;
    }
    printf("set view: %d\n",current_view);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
}
/*
* Gets the name of the controlled setting
*/
char* SettingView::GetEditedSettingName(void)
{
    std::map<lv_obj_t*, setting_t *>::iterator iter;
    iter = setting_map.begin();
    while(iter != setting_map.end()) {
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

//回到選單
void SettingView::ClearCtrView(lv_obj_t *obj)
{
    setting_t *setting = setting_map[obj];
    printf("back to setting menu\n");
    current_view = DEFAULT_VIEW;
    PlaygroundView::DefaultView();
    lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, 0);
}

int SettingView::GetViewMode(void)
{
    return current_view;
}


void SettingView::UpdateCtrlView(SettingInfo* info)
{
    int _value = 0;
    int32_t motor_pos = info->motor_pos;
    lv_meter_set_indicator_value(ui.meter, ui.nd_img_circle, motor_pos);
    switch (current_view) {
        case DEFAULT_VIEW:
            break;
        case WIFI_SETTING_VIEW:
            break;
        case LCD_BK_TIMEOUT_SETTING_VIEW:
            _value = info->xkonb_value;
            SettingView::UpdateBackgroundView(info);
            lv_label_set_text_fmt(
                ui.label_value,
                "%dmin",
                _value
            );
            lv_meter_set_indicator_value(ui.meter, ui.nd_img_circle,  _value );
            break;
        case LCD_BK_BRIGHTNESS_SETTING_VIEW:
            _value = info->xkonb_value;
            SettingView::UpdateBackgroundView(info);
            lv_label_set_text_fmt(
                ui.label_value,
                "%d",
                _value
            );
            lv_meter_set_indicator_value(ui.meter, ui.nd_img_circle,  _value );
            break;
        default:
            break;
    }
}
void SettingView::UpdateView(SettingInfo* info)
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

void SettingView::SetWiFiViewVisible(bool visible) 
{
    if(visible){
        lv_obj_clear_flag(m_ui->wifi, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(m_ui->cont_row, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(m_ui->foucs_label, LV_OBJ_FLAG_HIDDEN);
    }else{
        lv_obj_clear_flag(m_ui->cont_row, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(m_ui->foucs_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(m_ui->wifi, LV_OBJ_FLAG_HIDDEN);
    }
}

void SettingView::setting_item_create(
    setting_t* item,
    lv_obj_t* par,
    const char* name,
    const char* img_src,
    uint8_t num)
{
    lv_obj_t *cont = lv_obj_create(par);
    // lv_obj_remove_style_all(cont);
    // lv_obj_set_size(cont, 42, 62);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);

    lv_obj_t *img = lv_img_create(cont);
    lv_img_set_src(img, Resource.GetImage(img_src));
    // lv_obj_set_size(img, 42, 42);
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

    lv_obj_set_pos(ui.label_value, 70, 30);

    item->num = num;
    setting_map[cont] = item;
    // lv_obj_add_event_cb(item->cont, on_event, LV_EVENT_ALL, NULL);
    printf("create an item: %p\n", item);
}

void on_focus_setting(lv_group_t* g)
{
    lv_obj_t* cont = lv_group_get_focused(g);
    lv_obj_t* cont_row = lv_obj_get_parent(cont);
    lv_coord_t x = lv_obj_get_x(cont);
    lv_coord_t width = lv_obj_get_width(cont);
    lv_obj_scroll_to_x(cont_row, x, LV_ANIM_ON);
}

void SettingView::group_init(void)
{
    m_ui->group = lv_group_create();
    lv_group_set_focus_cb(m_ui->group, on_focus_setting);
    lv_indev_set_group(lv_get_indev(LV_INDEV_TYPE_ENCODER), m_ui->group);

    for(int i = 0; i < SETTING_NUM; i++) {
        lv_group_add_obj(m_ui->group, (m_ui->setting[i]).cont);
    }
    
    setting_t select = m_ui->setting[0];
    lv_obj_t* cont = select.cont;
    lv_group_focus_obj(cont);
    lv_obj_t* label = lv_obj_get_child(cont, 1);
    lv_obj_t* cont_row = lv_obj_get_parent(cont);
    lv_coord_t x = lv_obj_get_x(cont);
    lv_coord_t width = lv_obj_get_width(cont);
    lv_obj_scroll_to_x(cont_row, x, LV_ANIM_ON);
    UpdateFocusedSetting(lv_label_get_text(label));

    lv_obj_add_state(cont, LV_STATE_USER_1);
}

void SettingView::style_init(void)
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


    lv_style_init(&style.label_name);
    lv_style_set_text_font(&style.label_name, Resource.GetFont("bahnschrift_22"));
    lv_style_set_text_color(&style.label_name, lv_color_white());
}


void SettingView::wifi_ui_init(lv_obj_t* root) {
    lv_obj_t *ui_wifi = lv_obj_create(root);
    lv_obj_clear_flag(ui_wifi, LV_OBJ_FLAG_SCROLLABLE);  /// Flags
    lv_coord_t screen_width = lv_obj_get_width(lv_scr_act());  // 获取屏幕宽度
    lv_coord_t screen_height = lv_obj_get_height(lv_scr_act());  // 获取屏幕高度
    lv_obj_set_size(ui_wifi, screen_width, screen_height);
    lv_obj_add_flag(ui_wifi, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t *ui_spinner = lv_spinner_create(ui_wifi, 1000, 90);
    lv_obj_set_width(ui_spinner, 175);
    lv_obj_set_height(ui_spinner, 175);
    lv_obj_set_align(ui_spinner, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_spinner, LV_OBJ_FLAG_CLICKABLE);  /// Flags

    lv_obj_t *ui_label2 = lv_label_create(ui_wifi);
    lv_obj_set_width(ui_label2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_label2, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_x(ui_label2, 0);
    lv_obj_set_y(ui_label2, -10);
    lv_obj_set_align(ui_label2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_label2, "connect wifi");

    lv_obj_t *ui_label3 = lv_label_create(ui_wifi);
    lv_obj_set_width(ui_label3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_label3, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_x(ui_label3, 0);
    lv_obj_set_y(ui_label3, 12);
    lv_obj_set_align(ui_label3, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(ui_label3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_label3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    m_ui->wifi = ui_wifi;
    m_ui->wifi_label = ui_label3;
}

void SettingView::Create(lv_obj_t* root)
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
    
    m_ui = (setting_ui *)malloc(sizeof(setting_ui) + SETTING_NUM * sizeof(setting_t)); 
    if (m_ui == NULL) {
        printf("malloc m_ui failed\n");
        return;
    }

    wifi_ui_init(root);
    m_ui->cont_row = cont_row;

    setting_item_create(
    &(m_ui->setting[0]),
    cont_row,
    "WiFi Setting(OTA)",
    "setting_wifi",
    WIFI_SETTING_VIEW
    );

    setting_item_create(
    &(m_ui->setting[1]),
    cont_row,
    "Screen Auto-OFF Time",
    "setting_timer",
    LCD_BK_TIMEOUT_SETTING_VIEW
    );

    setting_item_create(
    &(m_ui->setting[2]),
    cont_row,
    "LCD BK Brightness",
    "setting_brightness",
    LCD_BK_BRIGHTNESS_SETTING_VIEW
    );

    //顯示目前選擇到設備
    m_ui->foucs_label = lv_label_create(root);
    lv_obj_add_style(m_ui->foucs_label, &style.label_name, 0);
    lv_label_set_text_fmt(m_ui->foucs_label, "%s", "NONE");
    lv_obj_align(m_ui->foucs_label, LV_ALIGN_CENTER, 0, ITEM_PAD+10);
    
    group_init();
}


void SettingView::Delete()
{
    lv_group_del(m_ui->group);
    lv_style_reset(&style.cont);
    lv_style_reset(&style.focus);
    lv_style_reset(&style.edit);
    lv_style_reset(&style.label_name);
    PlaygroundView::Delete();
}