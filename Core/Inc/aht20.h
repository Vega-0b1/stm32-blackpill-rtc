#pragma once
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"

typedef struct{
    float temperature;
    float humidity;
}aht20_t;

HAL_StatusTypeDef aht20_init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef aht20_read(I2C_HandleTypeDef *hi2c, aht20_t *out);



