#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "hal.h"

struct XKnobConfig {
    // 可以运动的个数
    int32_t num_positions;        
    // 位置
    int32_t position;             
    // 位置宽度弧度 或者是每一步的度数
    float position_width_radians; 
    // 制动强度
    float detent_strength_unit;  
    // end stop强度 
    float endstop_strength_unit;  
    // 快照点 
    float snap_point; 
    // 描述符            
    char descriptor[50];          
};

typedef enum
{
    MOTOR_INIT,
    MOTOR_INIT_SUCCESS,
    MOTOR_INIT_END,
    DEV_WORK_MODE,
    DEV_BLE_WORK,

} MOTOR_RUNNING_MODE_E;

#endif // 