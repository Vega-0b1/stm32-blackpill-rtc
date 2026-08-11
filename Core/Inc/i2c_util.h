#pragma once
#include "stm32f4xx_hal.h"

uint8_t i2c_scan(I2C_HandleTypeDef *hi2c, uint8_t *out, uint8_t max);
