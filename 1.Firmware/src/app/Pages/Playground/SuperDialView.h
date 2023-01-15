#ifndef __SUPER_DIAL_VIEW_H
#define __SUPER_DIAL_VIEW_H
#include "Arduino.h"
#include "app/app.h"
#include "../Page.h"
#include "PlaygroundView.h"


namespace Page
{

class SuperDialView: public PlaygroundView
{
public:
    void Create(lv_obj_t* root);
    void Delete();
    void UpdatePlaygroundView(PlaygroundMotorInfo *info);
    void SetPlaygroundMode(int16_t mode);
    int16_t playgroundMode;


public:


private:
};

}

#endif // !__VIEW_H
