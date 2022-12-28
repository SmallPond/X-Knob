#include "hal/hal.h"
#include <SimpleFOC.h>

static const int spiClk = 1000000; // 400KHz
SPIClass* hspi = NULL;

BLDCMotor motor = BLDCMotor(7);
BLDCDriver3PWM driver = BLDCDriver3PWM(MO1, MO2, MO3);
//目标变量
float target_velocity = 0;
float last_angle = 0;
float now_angle = 0;


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

void HAL::motor_init(void)
{
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
    // 运动控制模式设置
    motor.controller = MotionControlType::velocity;
    // 速度PI环设置
    motor.PID_velocity.P = 0.1;
    motor.PID_velocity.I = 15;
    // 最大电机限制电机
    motor.voltage_limit = 5;
    // 速度低通滤波时间常数
    motor.LPF_velocity.Tf = 0.01;
    // 设置最大速度限制
    motor.velocity_limit = 40;

    motor.useMonitoring(Serial);
    // 初始化电机
    motor.init();
    // 初始化 FOC
    // float zero_electric_offset = 1.62;
    // Direction foc_direction = Direction::CCW;
    // motor.initFOC(zero_electric_offset, foc_direction);
    motor.initFOC();

    last_angle = motor.shaft_angle;
}

void HAL::motor_update(void)
{
    float p;// 最大力矩限制

    sensor.update();
    motor.loopFOC();
    
    // float now_angle = motor.shaft_angle - last_angle;
    // Serial.printf("now angle: %f, shaft_angle %f \n", now_angle, motor.shaft_angle);
    // if (abs(now_angle) > 30 * 0.005) {
    //     // 转动一度刷新指针位置
    //     last_angle = motor.shaft_angle;
    // }
    // p = -now_angle * 10;
    // if (abs(p) > 4)
    //     p = _sign(p) * 4;  
    motor.move(10);
}