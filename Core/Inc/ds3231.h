#pragma once
#include "stm32f4xx_hal.h"

typedef struct {
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
  uint8_t date;
  uint8_t month;
  uint8_t year;
} ds3231_time_t;

void ds3231_read(ds3231_time_t *time);
void ds3231_write(ds3231_time_t *time);
void ds3231_adjust_hours(ds3231_time_t *rtc, uint8_t *curr_state);
void ds3231_adjust_minutes(ds3231_time_t *rtc, uint8_t *curr_state);
void ds3231_adjust_seconds(ds3231_time_t *rtc, uint8_t *curr_state);
void ds3231_adjust_month(ds3231_time_t *rtc, uint8_t *curr_state);
void ds3231_adjust_date(ds3231_time_t *rtc, uint8_t *curr_state);
void ds3231_adjust_year(ds3231_time_t *rtc, uint8_t *curr_state);
