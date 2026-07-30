#pragma once
#include "ds3231.h"
#include "stm32f4xx_hal.h"

void OLED_Init(I2C_HandleTypeDef *hi2c);
void OLED_SetCursor(uint8_t row, uint8_t col);
void OLED_Print(const char *str);
void OLED_Clear(void);
void OLED_Print_Rtc(DS3231_Time *rtc, uint8_t curr_state);
