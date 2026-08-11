#pragma once
#include "ds3231.h"
#include "stm32f4xx_hal.h"

void oled_init(I2C_HandleTypeDef *hi2c);
void oled_set_cursor(uint8_t row, uint8_t col);
void oled_print(const char *str);
void oled_clear(void);
void oled_print_rtc(ds3231_time_t *rtc, uint8_t curr_state);
