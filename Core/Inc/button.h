#pragma once
#include "stm32f4xx_hal.h"

typedef enum { BUTTON_MODE, BUTTON_UP } Button_t;

void Button_Init();
uint8_t Button_Pressed(Button_t btn);
