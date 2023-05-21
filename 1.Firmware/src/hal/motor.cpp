#include "hal/hal.h"
#include "motor.h"
#include <SimpleFOC.h>
#include "app/Accounts/Account_Master.h"

static const int spiClk = 1000000; // 400KHz
SPIClass* hspi = NULL;

BLDCMotor motor = BLDCMotor(7);
BLDCDriver3PWM driver = BLDCDriver3PWM(MO1, MO2, MO3);
//目标变量
float target_velocity = 0;

Account* actMotorStatus;

static XKnobConfig x_knob_configs[] = {
    // int32_t num_positions;        
    // int32_t position;             
    // float position_width_radians; 
    // float detent_strength_unit;  
    // float endstop_strength_unit;  
    // float snap_point;           
    // char descriptor[50]; 
    [MOTOR_UNBOUND_FINE_DETENTS] = {
        0,
        0,
        1 * PI / 180,
        2,
        1,
        1.1,
        "Fine values\nWith detents", //任意运动的控制  有阻尼 类似于机械旋钮
    },
    [MOTOR_UNBOUND_NO_DETENTS] = {
        0,
        0,
        1 * PI / 180,
        0,
        0.1,
        1.1,
        "Unbounded\nNo detents", //无限制  不制动
    },
    [MOTOR_SUPER_DIAL] = {
        0,
        0,
        5 * PI / 180,
        2,
        1,
        1.1,
        "Super Dial", //无限制  不制动
    },
    [MOTOR_UNBOUND_COARSE_DETENTS] = {
        .num_positions = 0,
        .position = 0,
        .position_width_radians = 8.225806452 * _PI / 180,
        .detent_strength_unit = 2.3,
        .endstop_strength_unit = 1,
        .snap_point = 1.1,
        "Fine values\nWith detents\nUnbound"
    },
    [MOTOR_BOUND_0_12_NO_DETENTS]= {
        13,
        0,
        10 * PI / 180,
        0,
        1,
        1.1,
        "Bounded 0-13\nNo detents",
    },
    [MOTOR_BOUND_LCD_BK_BRIGHTNESS]= {
        101,
        10,
        2 * PI / 180,
        2,
        1,
        1.1,
        "Bounded 0-101\nNo detents",
    },
    [MOTOR_BOUND_LCD_BK_TIMEOUT]= {
        31,
        0,
        5 * PI / 180,
        2,
        1,
        1.1,
        "Bounded 0-3601\nNo detents",
    },
    [MOTOR_COARSE_DETENTS] = {
        32,
        0,
        8.225806452 * PI / 180,
        2,
        1,
        1.1,
        "Coarse values\nStrong detents", //粗糙的棘轮 强阻尼
    },

    [MOTOR_FINE_NO_DETENTS] = {
        256,
        127,
        1 * PI / 180,
        0,
        1,
        1.1,
        "Fine values\nNo detents", //任意运动的控制  无阻尼
    },
    [MOTOR_ON_OFF_STRONG_DETENTS] = {
        2, 
        0,
        60 * PI / 180, 
        1,             
        1,
        0.55,                    // Note the snap point is slightly past the midpoint (0.5); compare to normal detents which use a snap point *past* the next value (i.e. > 1)
        "On/off\nStrong detent", //模拟开关  强制动
    },

};

XKnobConfig motor_config = {
    .num_positions = 0,
    .position = 0,
    .position_width_radians = 8.225806452 * _PI / 180,
    .detent_strength_unit = 2.3,
    .endstop_strength_unit = 1,
    .snap_point = 1.1,
};

// 死区制动百分率
static const float DEAD_ZONE_DETENT_PERCENT = 0.2;
// 死区RAD?
static const float DEAD_ZONE_RAD = 1 * _PI / 180;

// 怠速速度ewma alpha
static const float IDLE_VELOCITY_EWMA_ALPHA = 0.001;
// 怠速速度每秒钟弧度
static const float IDLE_VELOCITY_RAD_PER_SEC = 0.05;
// 怠速修正延迟millis
static const uint32_t IDLE_CORRECTION_DELAY_MILLIS = 500;
// 怠速校正最大角度rad
static const float IDLE_CORRECTION_MAX_ANGLE_RAD = 5 * PI / 180;
// 怠速修正率
static const float IDLE_CORRECTION_RATE_ALPHA = 0.0005;

// 当前相对位置
float current_detent_center = 0;
// 上次空闲开始状态
uint32_t last_idle_start = 0;
// 怠速检查速度
float idle_check_velocity_ewma = 0;

// 电机角度到当前位置的偏差
float angle_to_detent_center = 0;
//  ------------monitor--------------------
Commander commander = Commander(Serial, '\n', false);
void onPid(char* cmd){commander.pid(&motor.PID_velocity, cmd);}
void onMotor(char* cmd){commander.motor(&motor, cmd);}
// -------------monitor--------------------
//目标变量
static float readMySensorCallback(void) {
    hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
    digitalWrite(hspi->pinSS(), LOW);  // pull SS slow to prep other end for transfer
    uint16_t ag = hspi->transfer16(0);
    digitalWrite(hspi->pinSS(), HIGH); // pull ss high to signify end of data transfer
    hspi->endTransaction();
    ag = ag >> 2;
    float rad = (float)ag * 2 * PI / 16384;
    if (rad < 0) {
        rad += 2 * PI;
    }
    return rad;
}
static void initMySensorCallback(void) {
    hspi = new SPIClass(HSPI);
    hspi->begin(MT6701_SCL, MT6701_SDA, -1, MT6701_SS); //SCLK, MISO, MOSI, SS
    pinMode(hspi->pinSS(), OUTPUT); //HSPI SS
}


GenericSensor sensor = GenericSensor(readMySensorCallback, initMySensorCallback);

/* 
 * 将随机变化的值限制在一个给定的区间[min,max]内
*/ 
static float CLAMP(const float value, const float low, const float high)
{
    return value < low ? low : (value > high ? high : value);
}

void HAL::motor_shake(int strength, int delay_time)
{
    motor.move(strength);
    for (int i = 0; i < delay_time; i++)
    {
        motor.loopFOC();
        vTaskDelay(1);
    }
    motor.move(-strength);
    for (int i = 0; i < delay_time; i++)
    {
        motor.loopFOC();
        vTaskDelay(1);
    }
}


int HAL::get_motor_position(void)
{
    return motor_config.position;
}

void HAL::update_motor_mode(int mode , int init_position)
{
    motor_config = x_knob_configs[mode];
    motor_config.position = init_position;
    #if XK_INVERT_ROTATION
        current_detent_center = -motor.shaft_angle;
    #else 
        current_detent_center = motor.shaft_angle;
    #endif
}

static void motor_status_publish(bool is_outbound)
{
    // position
    static int32_t last_position = 0;

    if (is_outbound || motor_config.position != last_position) {
        MotorStatusInfo info = {
            .is_outbound = is_outbound,
            .position = motor_config.position,
            .angle_offset = angle_to_detent_center * 180 / PI,  // 转换为角度
        };
        actMotorStatus->Commit((const void*)&info, sizeof(MotorStatusInfo));
        actMotorStatus->Publish();
        last_position = motor_config.position;
    }
    
}

TaskHandle_t handleTaskMotor;
void TaskMotorUpdate(void *pvParameters)
{
    // ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    #if XK_INVERT_ROTATION
        current_detent_center = -motor.shaft_angle;
    #else 
        current_detent_center = motor.shaft_angle;
    #endif
    while(1) {
        sensor.update();
        motor.loopFOC();

        idle_check_velocity_ewma = motor.shaft_velocity * IDLE_VELOCITY_EWMA_ALPHA + idle_check_velocity_ewma * (1 - IDLE_VELOCITY_EWMA_ALPHA);
        if (fabsf(idle_check_velocity_ewma) > IDLE_VELOCITY_RAD_PER_SEC)
        {
            last_idle_start = 0;
        }
        else
        {
            if (last_idle_start == 0)
            {
                last_idle_start = millis();
            }
        }

        // 如果我们没有移动，并且我们接近中心(但不是完全在那里)，慢慢调整中心点以匹配当前位置
        // If we are not moving and we're close to the center (but not exactly there), slowly adjust the centerpoint to match the current position
        if (last_idle_start > 0 && millis() - last_idle_start > IDLE_CORRECTION_DELAY_MILLIS 
                && fabsf(motor.shaft_angle - current_detent_center) < IDLE_CORRECTION_MAX_ANGLE_RAD)
        {
            // Serial.println("slowly adjust the centerpoint to match the current position......");
            current_detent_center = motor.shaft_angle * IDLE_CORRECTION_RATE_ALPHA + current_detent_center * (1 - IDLE_CORRECTION_RATE_ALPHA);
        }

        //到控制中心的角度 差值
        #if XK_INVERT_ROTATION
            angle_to_detent_center = -motor.shaft_angle - current_detent_center;
        #else 
            angle_to_detent_center = motor.shaft_angle - current_detent_center;
        #endif 
        // 每一步都乘以了 snap_point 的值

        if (angle_to_detent_center > motor_config.position_width_radians * motor_config.snap_point 
                && (motor_config.num_positions <= 0 || motor_config.position > 0))
        {
            current_detent_center += motor_config.position_width_radians;
            angle_to_detent_center -= motor_config.position_width_radians;
            /*
             * 这里判断为正转， position 应该 ++，这里反了之后，
             * encoder 的逻辑也需要反一下
            */
            motor_config.position--;   
        }
        else if (angle_to_detent_center < -motor_config.position_width_radians * motor_config.snap_point 
                    && (motor_config.num_positions <= 0 || motor_config.position < motor_config.num_positions - 1))
        {
            current_detent_center -= motor_config.position_width_radians;
            angle_to_detent_center += motor_config.position_width_radians;
            motor_config.position++;
        }

        // 死区调整
        float dead_zone_adjustment = CLAMP(
            angle_to_detent_center,
            fmaxf(-motor_config.position_width_radians * DEAD_ZONE_DETENT_PERCENT, -DEAD_ZONE_RAD),
            fminf(motor_config.position_width_radians * DEAD_ZONE_DETENT_PERCENT, DEAD_ZONE_RAD));

        // 出界
        bool out_of_bounds = motor_config.num_positions > 0 && 
                    ((angle_to_detent_center > 0 && motor_config.position == 0) 
                    || (angle_to_detent_center < 0 && motor_config.position == motor_config.num_positions - 1));
        motor.PID_velocity.limit = out_of_bounds ? 10 : 3;
        motor.PID_velocity.P = out_of_bounds ? motor_config.endstop_strength_unit * 4 : motor_config.detent_strength_unit * 4;

        // 处理float类型的取绝对值
        if (fabsf(motor.shaft_velocity) > 60)
        {
            // 如果速度太高 则不增加扭矩
            // Don't apply torque if velocity is too high (helps avoid positive feedback loop/runaway)
            // Serial.println("(motor.shaft_velocity) > 60 !!!");
            motor.move(0);
        }
        else
        {
            // 运算符重载，输入偏差计算 PID 输出值
            float torque = motor.PID_velocity(-angle_to_detent_center + dead_zone_adjustment);
            #if XK_INVERT_ROTATION
                torque = -torque;
            #endif
            motor.move(torque);
        }
        // motor.monitor();
        commander.run();
        motor_status_publish(out_of_bounds);
        // Serial.println(motor_config.position);
        vTaskDelay(1);
    }
    
}


void HAL::motor_init(void)
{
    // update_motor_status(MOTOR_INIT);
    // initialize sensor hardware
    sensor.init();
    // 连接 motor 对象与传感器对象
    motor.linkSensor(&sensor);
    // 供电电压设置 [V]
    driver.voltage_power_supply = 5;
    driver.init();
    motor.linkDriver(&driver);
    // FOC模型选择
    motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
    // 运动控制模式设置：先设置为角度控制，方便初次定位
    // motor.controller = MotionControlType::angle;
    motor.controller = MotionControlType::torque;
    // 速度PI环设置
    motor.PID_velocity.P = 1;
    motor.PID_velocity.I = 0;
    motor.PID_velocity.D = 0.01;
    // 最大电压
    motor.voltage_limit = 5;
    // 速度低通滤波时间常数
    motor.LPF_velocity.Tf = 0.01;
    // 设置最大速度限制
    motor.velocity_limit = 10;

    // 初始化电机
    motor.init();
    // 初始化 FOC
    // float zero_electric_offset = 1.62;
    // Direction foc_direction = Direction::CCW;
    // motor.initFOC(zero_electric_offset, foc_direction);
    motor.initFOC();
    // update_motor_status(MOTOR_INIT_SUCCESS);
    // init_angle();
    // motor.controller = MotionControlType::torque;
    // update_motor_status(MOTOR_INIT_END);

    // motor.useMonitoring(Serial);
    // motor.monitor_variables = _MON_TARGET | _MON_VEL | _MON_ANGLE; 
    // downsampling
    // motor.monitor_downsample = 100; // default 10
    
    actMotorStatus = new Account("MotorStatus", AccountSystem::Broker(), sizeof(MotorStatusInfo), nullptr);

    // commander.add('C', onPid, "PID vel");
    // commander.add('M', onMotor, "my motor");
    xTaskCreatePinnedToCore(
        TaskMotorUpdate,
        "MotorThread",
        4096,
        nullptr,
        2,
        &handleTaskMotor,
        ESP32_RUNNING_CORE);
    // update_motor_config(0);
}

