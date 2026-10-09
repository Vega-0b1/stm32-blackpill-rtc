#include "button.h"
#include "stm32f4xx_hal_gpio.h"
static uint8_t prev_pressed[2] = {0};

void button_init(button_t *btn, GPIO_TypeDef *port, uint16_t pin) {
  btn->port = port;
  btn->pin = pin;
  btn->prev_pressed = 0;

  GPIO_InitTypeDef cfg = {0};
  cfg.Pin = pin;
  cfg.Mode = GPIO_MODE_INPUT;
  cfg.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(port, &cfg);
}

uint8_t button_pressed(Button_t *btn) {
  uint8_t now_pressed = HAL_GPIO_ReadPin(btn->port, btn->pin) == GPIO_PIN_RESET;
  uint8_t edge = now_pressed && !btn->prev_pressed;

  btn->prev_pressed = now_pressed;
  return edge;
}
