#include "oled.h"
#include "ds3231.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>

void OLED_Init(I2C_HandleTypeDef *i2c_handle_arg) { ssd1306_Init(); }

void OLED_Clear(void) {
  ssd1306_Fill(Black);
  ssd1306_UpdateScreen();
}

void OLED_SetCursor(uint8_t row, uint8_t col) {
  ssd1306_SetCursor(col, row == 0 ? 0 : 16);
}

void OLED_Print(const char *str) {
  ssd1306_WriteString((char *)str, Font_16x15, White);
}

void OLED_Print_Rtc(DS3231_Time *rtc, uint8_t curr_state) {
  char buf[17];
  OLED_SetCursor(0, 0);
  sprintf(buf, "%02d:%02d:%02d     M%d", rtc->hours, rtc->minutes, rtc->seconds,
          curr_state);
  OLED_Print(buf);

  OLED_SetCursor(1, 0);
  sprintf(buf, "%02d/%02d/%02d", rtc->month, rtc->date, rtc->year);
  OLED_Print(buf);
  ssd1306_UpdateScreen();
}
