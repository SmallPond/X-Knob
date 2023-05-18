#ifndef __MENU_VIEW_H
#define __MENU_VIEW_H

#include "../Page.h"

namespace Page
{

class MenuView
{
public:
    void Create(lv_obj_t* root);
    void Delete();

public:
    typedef struct
    {
        lv_obj_t* cont;
        lv_obj_t* icon;
        lv_obj_t* labelInfo;
        lv_obj_t* labelData;
    } item_t;

    struct
    {
        item_t dialpad;
        item_t switches;
        item_t system;
        item_t hass;   // home assistant
        item_t setting;
        item_t battery;
        item_t storage;
        lv_group_t* group;
    } ui;

public:

    void SetSystem(
        const char* firmVer,
        const char* authorName,
        const char* lvglVer,
        const char* bootTime,
        const char* compilerName,
        const char* bulidTime
    );

    void SetScrollToY(lv_obj_t* obj, lv_coord_t y, lv_anim_enable_t en);
    static void onFocus(lv_group_t* e);

private:
    struct
    {
        lv_style_t icon;
        lv_style_t focus;
        lv_style_t info;
        lv_style_t data;
    } style;

private:
    void Group_Init();
    void Style_Init();
    void Style_Reset();
    void Item_Create(
        item_t* item,
        lv_obj_t* par,
        const char* name,
        const char* img_src,
        const char* infos
    );
};

}

#endif // !__VIEW_H
