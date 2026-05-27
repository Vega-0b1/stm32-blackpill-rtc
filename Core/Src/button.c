#include "button.h"

void Button_Init() {
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitTypeDef btn = {0};
  btn.Pin = GPIO_PIN_0 | GPIO_PIN_7;
  btn.Mode = GPIO_MODE_INPUT;
  btn.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &btn);
}

uint8_t Button_Pressed(Button_t btn) {
  uint16_t pin = (btn == BUTTON_MODE) ? GPIO_PIN_0 : GPIO_PIN_7;
  return HAL_GPIO_ReadPin(GPIOA, pin) == GPIO_PIN_RESET ? 1 : 0;
}
