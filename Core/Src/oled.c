#include "oled.h"
#include "ds3231.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>

void oled_init(I2C_HandleTypeDef *i2c_handle_arg) { ssd1306_Init(); }

void oled_clear(void) {
  ssd1306_Fill(Black);
  ssd1306_UpdateScreen();
}

void oled_set_cursor(uint8_t row, uint8_t col) {
  ssd1306_SetCursor(col, row == 0 ? 0 : 16);
}

void oled_print(const char *str) {
  ssd1306_WriteString((char *)str, Font_16x15, White);
}

void oled_print_rtc(ds3231_time_t *rtc, uint8_t curr_state) {
  char buf[17];
  oled_set_cursor(0, 0);
  snprintf(buf, sizeof buf, "%02d:%02d:%02d     M%d", rtc->hours, rtc->minutes, rtc->seconds,
          curr_state);
  oled_print(buf);

  oled_set_cursor(1, 0);
  snprintf(buf, sizeof buf, "%02d/%02d/%02d", rtc->month, rtc->date, rtc->year);
  oled_print(buf);
  ssd1306_UpdateScreen();
}
