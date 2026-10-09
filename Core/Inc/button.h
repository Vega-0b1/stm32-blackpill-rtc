#pragma once
#include "stm32f411xe.h"

typedef struct{
    GPIO_TypeDef *port;
    uint16_t pin;
    uint8_t prev_pressed;
}button_t;


void button_init(button_t *btn, GPIO_TypeDef *port, uint16_t pin);
uint8_t button_pressed(button_t *btn);
