#pragma once
#include "ds3231.h"
#include "stm32f4xx_hal.h"

void LCD_Init(I2C_HandleTypeDef *hi2c);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_Print(const char *str);
void LCD_Clear(void);
void LCD_Print_Rtc(DS3231_Time *rtc, uint8_t curr_state);
