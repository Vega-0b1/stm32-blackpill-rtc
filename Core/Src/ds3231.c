#include "ds3231.h"
#include "button.h"
#include "lcd.h"

#define DS3231_ADDR (0x68 << 1)
extern I2C_HandleTypeDef hi2c1;

static uint8_t bcd_to_decimal(uint8_t bcd) {
  return (bcd >> 4) * 10 + (bcd & 0x0F);
}

static uint8_t decimal_to_bcd(uint8_t dec) {
  return (dec / 10) << 4 | (dec % 10);
}

void DS3231_Read(DS3231_Time *time) {
  uint8_t buf[7];
  uint8_t reg = 0x00;

  HAL_I2C_Master_Transmit(&hi2c1, DS3231_ADDR, &reg, 1, 100);
  HAL_I2C_Master_Receive(&hi2c1, DS3231_ADDR, buf, 7, 100);

  time->seconds = bcd_to_decimal(buf[0]);
  time->minutes = bcd_to_decimal(buf[1]);
  time->hours = bcd_to_decimal(buf[2]);

  time->date = bcd_to_decimal(buf[4]);
  time->month = bcd_to_decimal(buf[5]);
  time->year = bcd_to_decimal(buf[6]);
}

void DS3231_Write(DS3231_Time *time) {
  uint8_t buf[8];
  buf[0] = 0x00;
  buf[1] = decimal_to_bcd(time->seconds);
  buf[2] = decimal_to_bcd(time->minutes);
  buf[3] = decimal_to_bcd(time->hours);
  buf[4] = 0x01;
  buf[5] = decimal_to_bcd(time->date);
  buf[6] = decimal_to_bcd(time->month);
  buf[7] = decimal_to_bcd(time->year);

  HAL_I2C_Master_Transmit(&hi2c1, DS3231_ADDR, buf, 8, 100);
}

void DS3231_adjust_hours(DS3231_Time *rtc, uint8_t *curr_state) {
  DS3231_Time temp;
  DS3231_Read(&temp);
  rtc->seconds = temp.seconds;
  rtc->minutes = temp.minutes;
  LCD_Print_Rtc(rtc, *curr_state);

  if (Button_Pressed(BUTTON_MODE) == 1)
    *curr_state += 1;
  else if (Button_Pressed(BUTTON_UP) == 1) {
    rtc->hours = (rtc->hours + 1) % 24;
    DS3231_Write(rtc);
  }
}

void DS3231_adjust_minutes(DS3231_Time *rtc, uint8_t *curr_state) {
  DS3231_Time temp;
  DS3231_Read(&temp);
  rtc->seconds = temp.seconds;
  rtc->minutes = temp.minutes;
  LCD_Print_Rtc(rtc, *curr_state);

  if (Button_Pressed(BUTTON_MODE) == 1)
    *curr_state += 1;
  else if (Button_Pressed(BUTTON_UP) == 1) {
    rtc->minutes = (rtc->minutes + 1) % 60;
    DS3231_Write(rtc);
  }
}

void DS3231_adjust_seconds(DS3231_Time *rtc, uint8_t *curr_state) {
  DS3231_Time temp;
  DS3231_Read(&temp);
  rtc->seconds = temp.seconds;
  rtc->minutes = temp.minutes;
  LCD_Print_Rtc(rtc, *curr_state);

  if (Button_Pressed(BUTTON_MODE) == 1)
    *curr_state += 1;
  else if (Button_Pressed(BUTTON_UP) == 1) {
    rtc->seconds = 0;
    DS3231_Write(rtc);
  }
}

void DS3231_adjust_month(DS3231_Time *rtc, uint8_t *curr_state) {
  DS3231_Time temp;
  DS3231_Read(&temp);
  rtc->seconds = temp.seconds;
  rtc->minutes = temp.minutes;
  LCD_Print_Rtc(rtc, *curr_state);

  if (Button_Pressed(BUTTON_MODE) == 1)
    *curr_state += 1;
  else if (Button_Pressed(BUTTON_UP) == 1) {
    rtc->month = (rtc->month % 12) + 1;
    DS3231_Write(rtc);
  }
}

void DS3231_adjust_date(DS3231_Time *rtc, uint8_t *curr_state) {
  DS3231_Time temp;
  DS3231_Read(&temp);
  rtc->seconds = temp.seconds;
  rtc->minutes = temp.minutes;

  LCD_Print_Rtc(rtc, *curr_state);
  if (Button_Pressed(BUTTON_MODE) == 1) {
    *curr_state += 1;
  } else if (Button_Pressed(BUTTON_UP) == 1) {
    if (rtc->month == 2) {
      if (rtc->year % 4 == 0) {
        rtc->date = (rtc->date % 29) + 1;
      } else {

        rtc->date = (rtc->date % 28) + 1;
      }
    } else if (rtc->month == 4 || rtc->month == 6 || rtc->month == 9 ||
               rtc->month == 11) {

      rtc->date = (rtc->date % 30) + 1;
    } else {
      rtc->date = (rtc->date % 31) + 1;
    }
    DS3231_Write(rtc);
  }
}

void DS3231_adjust_year(DS3231_Time *rtc, uint8_t *curr_state) {
  DS3231_Time temp;
  DS3231_Read(&temp);
  rtc->seconds = temp.seconds;
  rtc->minutes = temp.minutes;

  LCD_Print_Rtc(rtc, *curr_state);
  if (Button_Pressed(BUTTON_MODE) == 1) {
    *curr_state = 0;
    LCD_Clear();
  } else if (Button_Pressed(BUTTON_UP) == 1) {
    rtc->year = (rtc->year + 1) % 100;
    DS3231_Write(rtc);
  }
}
