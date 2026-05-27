#include "lcd.h"
#include "ds3231.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>

#define LCD_ADDR (0x27 << 1)
#define BACKLIGHT 0x08
#define EN 0x04
#define RS 0x01

static I2C_HandleTypeDef *i2c_handle;

static void lcd_write_byte(uint8_t byte) {
  HAL_I2C_Master_Transmit(i2c_handle, LCD_ADDR, &byte, 1, 10);
}

static void lcd_pulse_enable(uint8_t byte) {
  lcd_write_byte(byte | EN);
  HAL_Delay(1);
  lcd_write_byte(byte & ~EN);
  HAL_Delay(1);
}

static void lcd_send_nibble(uint8_t nibble, uint8_t rs) {
  uint8_t byte = (nibble << 4) | BACKLIGHT | rs;
  lcd_pulse_enable(byte);
}

static void lcd_send(uint8_t value, uint8_t rs) {
  lcd_send_nibble(value >> 4, rs);
  lcd_send_nibble(value & 0x0F, rs);
}

void LCD_Init(I2C_HandleTypeDef *i2c_handle_arg) {
  i2c_handle = i2c_handle_arg;
  HAL_Delay(50);
  lcd_send_nibble(0x03, 0);
  HAL_Delay(5);
  lcd_send_nibble(0x03, 0);
  HAL_Delay(1);
  lcd_send_nibble(0x03, 0);
  HAL_Delay(1);
  lcd_send_nibble(0x02, 0);

  lcd_send(0x28, 0);
  lcd_send(0x0C, 0);
  lcd_send(0x06, 0);
  lcd_send(0x01, 0);
  HAL_Delay(2);
}

void LCD_Clear(void) {
  lcd_send(0x01, 0);
  HAL_Delay(2);
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
  uint8_t addr = (row == 0) ? 0x80 : 0xC0;
  lcd_send(addr + col, 0);
}

void LCD_Print(const char *str) {
  while (*str)
    lcd_send((uint8_t)*str++, RS);
}

void LCD_Print_Rtc(DS3231_Time *rtc, uint8_t curr_state) {
  char buf[17];
  LCD_SetCursor(0, 0);
  sprintf(buf, "%02d:%02d:%02d      M%d", rtc->hours, rtc->minutes,
          rtc->seconds, curr_state);
  LCD_Print(buf);

  LCD_SetCursor(1, 0);
  sprintf(buf, "%02d/%02d/%02d", rtc->month, rtc->date, rtc->year);
  LCD_Print(buf);
}
