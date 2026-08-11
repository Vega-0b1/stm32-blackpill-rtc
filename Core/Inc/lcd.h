#pragma once
#include "ds3231.h"
#include "stm32f4xx_hal.h"

void lcd_init(I2C_HandleTypeDef *hi2c);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_print(const char *str);
void lcd_clear(void);
void lcd_print_rtc(ds3231_time_t *rtc, uint8_t curr_state);
