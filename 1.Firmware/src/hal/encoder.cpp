
#include "hal.h"
#include <Arduino.h>

static bool encoder_stat = true;

bool HAL::is_encoder_enabled(void)
{
    return encoder_stat;
}

void HAL::encoder_disable(void)
{
    encoder_stat = false;
}

void HAL::encoder_enable(void)
{
    encoder_stat = true;
}