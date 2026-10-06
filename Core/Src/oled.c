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
  ssd1306_SetCursor(col, row * 11);
}

void oled_print(const char *str) {
  ssd1306_WriteString((char *)str, Font_7x10, White);
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

void oled_print_aht20(const aht20_t *data) {
  char buf[32] = {0};
  float temp = data->temperature * 9.0f / 5.0f + 32.0f;
  int temp_tenths = (int)(temp * 10.0f + (temp < 0.0f ? -0.5f : 0.5f));
  int hum_tenths = (int)(data->humidity * 10.0f + 0.5f);

  const char *sign = (temp_tenths < 0) ? "-" : "";
  if (temp_tenths < 0)
    temp_tenths = -temp_tenths;

  oled_set_cursor(2, 0);
  snprintf(buf, sizeof buf, "%s%d.%dF  %d.%d%%", sign, temp_tenths / 10,
           temp_tenths % 10, hum_tenths / 10, hum_tenths % 10);
  oled_print(buf);
}
