#pragma once
#include "stm32f4xx_hal.h"

typedef enum { BUTTON_MODE, BUTTON_UP } Button_t;

void button_init();
uint8_t button_pressed(Button_t btn);
