#include "hal/hal.h"
#include "motor.h"
#include <SimpleFOC.h>

static const int spiClk = 1000000; // 400KHz
SPIClass* hspi = NULL;

BLDCMotor motor = BLDCMotor(7);
BLDCDriver3PWM driver = BLDCDriver3PWM(MO1, MO2, MO3);
//目标变量
float target_velocity = 0;

static XKnobConfig x_knob_configs[] = {
    {
        0,
        0,
        10 * PI / 180,
        0,
        1,
        1.1,
        "Unbounded\nNo detents", //无限制  不制动
    },
    {
        32,
        0,
        8.225806452 * PI / 180,
        2,
        1,
        1.1,
        "Coarse values\nStrong detents", //粗糙的棘轮 强阻尼
    },
    {
        256,
        127,
        1 * PI / 180,
        1,
        1,
        1.1,
        "Fine values\nWith detents", //任意运动的控制  有阻尼 类似于机械旋钮
    },
    {
        256,
        127,
        1 * PI / 180,
        0,
        1,
        1.1,
        "Fine values\nNo detents", //任意运动的控制  无阻尼
    },
    {
        2, //可以运动的个数
        0,
        60 * PI / 180, //每一步60度
        1,             //制动强度为1
        1,
        0.55,                    // Note the snap point is slightly past the midpoint (0.5); compare to normal detents which use a snap point *past* the next value (i.e. > 1)
        "On/off\nStrong detent", //模拟开关  强制动
    },
};

XKnobConfig motor_config = {
    .num_positions = 32,
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

void HAL::update_motor_mode(int mode)
{
    motor_config = x_knob_configs[mode];
}

void update_motor_status(MOTOR_RUNNING_MODE_E motor_status)
{
    struct _knob_message *send_message;
    // send_message = &MOTOR_MSG;
    // send_message->ucMessageID = motor_status;
    // xQueueSend(motor_msg_Queue, &send_message, (TickType_t)0);
}

//开机初始化角度至0
void init_angle(void)
{
    float target_angle = 0;
    target_angle = sensor.getAngle();
    Serial.printf("init_angle to current target %f\n", target_angle);
    float delta = volt_limit / init_smooth;
    for (int i = 0; i <= init_smooth; i++)
    {
        motor.voltage_limit = delta * i;
        motor.loopFOC();
        motor.move(target_angle);
    }
    motor.voltage_limit = volt_limit;
}


TaskHandle_t handleTaskMotor;
void TaskMotorUpdate(void *pvParameters)
{
    // ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    current_detent_center = motor.shaft_angle;
    while(1) {
        sensor.update();
        motor.loopFOC();
        
        //监听页面状态
        // struct _knob_message *lvgl_message;
        // if (xQueueReceive(motor_rcv_Queue, &(lvgl_message), (TickType_t)0))
        // {
        //     Serial.print("motor_rcv_Queue --->");
        //     Serial.println(lvgl_message->ucMessageID);
        //     switch (lvgl_message->ucMessageID)
        //     {
        //     case CHECKOUT_PAGE:
        //     {
        //         //上次的相对位置
        //         current_detent_center = motor.shaft_angle;
        //         // motor.PID_velocity.limit = 10;

        //         const float derivative_lower_strength = motor_config.detent_strength_unit * 0.08;
        //         const float derivative_upper_strength = motor_config.detent_strength_unit * 0.02;
        //         const float derivative_position_width_lower = radians(3);
        //         const float derivative_position_width_upper = radians(8);
        //         const float raw = derivative_lower_strength + (derivative_upper_strength - derivative_lower_strength) / (derivative_position_width_upper - derivative_position_width_lower) * (motor_config.position_width_radians - derivative_position_width_lower);
        //         // CLAMP可以将随机变化的值限制在一个给定的区间[min,max]内
        //         motor.PID_velocity.D = CLAMP(
        //             raw,
        //             min(derivative_lower_strength, derivative_upper_strength),
        //             max(derivative_lower_strength, derivative_upper_strength));

        //         //存在页面切换 就震动一下
        //         motor_shake(2, 2);
        //     }
        //     break;
        //     case BUTTON_CLICK:
        //         motor_shake(2, 2);
        //         break;
        //     default:
        //         break;
        //     }
        // }

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
        if (last_idle_start > 0 && millis() - last_idle_start > IDLE_CORRECTION_DELAY_MILLIS && fabsf(motor.shaft_angle - current_detent_center) < IDLE_CORRECTION_MAX_ANGLE_RAD)
        {
            // Serial.println("slowly adjust the centerpoint to match the current position......");
            current_detent_center = motor.shaft_angle * IDLE_CORRECTION_RATE_ALPHA + current_detent_center * (1 - IDLE_CORRECTION_RATE_ALPHA);
        }

        //到控制中心的角度 差值
        float angle_to_detent_center = motor.shaft_angle - current_detent_center;

        if (angle_to_detent_center > motor_config.position_width_radians * motor_config.snap_point && (motor_config.num_positions <= 0 || motor_config.position > 0))
        {
            current_detent_center += motor_config.position_width_radians;
            angle_to_detent_center -= motor_config.position_width_radians;
            motor_config.position--;
        }
        else if (angle_to_detent_center < -motor_config.position_width_radians * motor_config.snap_point && (motor_config.num_positions <= 0 || motor_config.position < motor_config.num_positions - 1))
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
        bool out_of_bounds = motor_config.num_positions > 0 && ((angle_to_detent_center > 0 && motor_config.position == 0) || (angle_to_detent_center < 0 && motor_config.position == motor_config.num_positions - 1));
        motor.PID_velocity.limit = out_of_bounds ? 10 : 3;
        motor.PID_velocity.P = out_of_bounds ? motor_config.endstop_strength_unit * 4 : motor_config.detent_strength_unit * 4;

        // 处理float类型的取绝对值
        if (fabsf(motor.shaft_velocity) > 60)
        {
            //如果速度太高 则不增加扭矩
            // Don't apply torque if velocity is too high (helps avoid positive feedback loop/runaway)
            // Serial.println("(motor.shaft_velocity) > 60 !!!");
            motor.move(0);
        }
        else
        {
            float torque = motor.PID_velocity(-angle_to_detent_center + dead_zone_adjustment);
            motor.move(torque);
        }
        motor.monitor();
        // Serial.println(motor_config.position);
        vTaskDelay(1);
    }
    
}


void HAL::motor_init(void)
{
    update_motor_status(MOTOR_INIT);
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
    motor.controller = MotionControlType::angle;
    // 速度PI环设置
    motor.PID_velocity.P = 0.1;
    motor.PID_velocity.I = 15;
    // 最大电压
    motor.voltage_limit = 5;
    // 速度低通滤波时间常数
    motor.LPF_velocity.Tf = 0.01;
    // 设置最大速度限制
    motor.velocity_limit = 40;

    // 初始化电机
    motor.init();
    // 初始化 FOC
    // float zero_electric_offset = 1.62;
    // Direction foc_direction = Direction::CCW;
    // motor.initFOC(zero_electric_offset, foc_direction);
    motor.initFOC();
    update_motor_status(MOTOR_INIT_SUCCESS);
    init_angle();
    motor.controller = MotionControlType::torque;
    update_motor_status(MOTOR_INIT_END);

    motor.useMonitoring(Serial);
    motor.monitor_variables = _MON_TARGET | _MON_VEL | _MON_ANGLE; 
    // downsampling
    motor.monitor_downsample = 100; // default 10
    
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

