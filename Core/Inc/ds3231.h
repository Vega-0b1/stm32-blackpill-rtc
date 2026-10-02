#pragma once
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"

typedef struct {
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
  uint8_t date;
  uint8_t month;
  uint8_t year;
} ds3231_time_t;

typedef enum{
  DS3231_FIELD_SECONDS,
  DS3231_FIELD_MINUTES,
  DS3231_FIELD_HOURS,
  DS3231_FIELD_DATE,
  DS3231_FIELD_MONTH,
  DS3231_FIELD_YEAR
}ds3231_field_t;

HAL_StatusTypeDef ds3231_read(I2C_HandleTypeDef *hi2c, ds3231_time_t *time);
HAL_StatusTypeDef ds3231_write(I2C_HandleTypeDef *hi2c,ds3231_time_t *time);
void ds3231_increment(ds3231_time_t *time, ds3231_field_t field);