#include "SuperDialView.h"

using namespace Page;


static int32_t MAX_VALUE = 100;
static int32_t MIN_VALUE = 0;
static int32_t SCALE_LEFT_BOUND_TICKS = 200;
static int32_t SCALE_ANGLE_RANGE = 140;
static int32_t SCALE_RIGHT_BOUND_TICKS = SCALE_LEFT_BOUND_TICKS + SCALE_ANGLE_RANGE;
static int32_t ARC_START_ROTATION = 180;
/*
 * 默认视图显示： 圆点，原点所在位置 label_value
 * 此函数根据需要增加或 hidden 对象
*/
void Page::SuperDialView::SetPlaygroundMode(int16_t mode)
{
	
	lv_obj_add_flag(ui.lable_value, LV_OBJ_FLAG_HIDDEN);

}




void SuperDialView::Create(lv_obj_t* root)
{
	PlaygroundView:Create(root);
	
}
