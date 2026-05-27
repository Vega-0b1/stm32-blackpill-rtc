#include "ds3231.h"
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
