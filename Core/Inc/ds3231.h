#pragma once
#include "stm32f4xx_hal.h"

typedef struct {
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
  uint8_t date;
  uint8_t month;
  uint8_t year;
} DS3231_Time;

void DS3231_Read(DS3231_Time *time);
void DS3231_Write(DS3231_Time *time);
void DS3231_adjust_hours(DS3231_Time *rtc, uint8_t *curr_state);
void DS3231_adjust_minutes(DS3231_Time *rtc, uint8_t *curr_state);
void DS3231_adjust_seconds(DS3231_Time *rtc, uint8_t *curr_state);
void DS3231_adjust_month(DS3231_Time *rtc, uint8_t *curr_state);
void DS3231_adjust_date(DS3231_Time *rtc, uint8_t *curr_state);
void DS3231_adjust_year(DS3231_Time *rtc, uint8_t *curr_state);
