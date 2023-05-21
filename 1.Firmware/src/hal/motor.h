#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "hal.h"

#define pi 3.1415926
#define init_smooth 1000 // 该值越大，初始化越慢。以防受到干扰。
#define volt_limit 5.0000

typedef struct {
    bool is_outbound;
    int32_t position;
    float angle_offset;
}MotorStatusInfo;

struct XKnobConfig {
    // 可以运动的个数
    int32_t num_positions;        
    // 位置
    int32_t position;             
    // 位置宽度弧度 或者是每一步的度数
    float position_width_radians; 
    // 正常旋转时的制动强度
    float detent_strength_unit;  
    // 超出界限后的制动强度
    float endstop_strength_unit;  
    // 每一步弧度的放大值
    float snap_point; 
    // 描述符            
    char descriptor[50];          
};

typedef enum
{
    MOTOR_UNBOUND_FINE_DETENTS,        // Fine values\nWith detents
    MOTOR_UNBOUND_NO_DETENTS,
    MOTOR_SUPER_DIAL, 
    MOTOR_UNBOUND_COARSE_DETENTS, // Coarse values\nStrong detents\n unbound
    MOTOR_BOUND_0_12_NO_DETENTS,
    MOTOR_BOUND_LCD_BK_BRIGHTNESS,
    MOTOR_BOUND_LCD_BK_TIMEOUT,
    MOTOR_COARSE_DETENTS,       // Coarse values\nStrong detents
    MOTOR_FINE_NO_DETENTS,     // Fine values\nNo detents
    MOTOR_ON_OFF_STRONG_DETENTS,             // "On/off\nStrong detent"
    MOTOR_MAX_MODES, //

} MOTOR_RUNNING_MODE_E;

#endif // 