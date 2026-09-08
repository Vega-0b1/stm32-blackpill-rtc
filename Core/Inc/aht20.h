#pragma once
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"

typedef enum {
  AHT20_OK = 0,
  AHT20_ERR_I2C,
  AHT20_ERR_BUSY,
  AHT20_ERR_UNCALIBRATED
} aht20_status_t;

typedef struct {
  float temperature;
  float humidity;
} aht20_data_t;

aht20_status_t aht20_init(I2C_HandleTypeDef *hi2c);
aht20_status_t aht20_read(aht20_data_t *out);
