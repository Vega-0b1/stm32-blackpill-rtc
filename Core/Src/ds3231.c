#include "ds3231.h"
#include "stm32f4xx_hal_def.h"
#define DS3231_ADDR (0x68 << 1)

static uint8_t bcd_to_decimal(uint8_t bcd) {
  return (bcd >> 4) * 10 + (bcd & 0x0F);
}

static uint8_t decimal_to_bcd(uint8_t dec) {
  return (dec / 10) << 4 | (dec % 10);
}

HAL_StatusTypeDef ds3231_read(I2C_HandleTypeDef *hi2c, ds3231_time_t *time) {
  uint8_t buf[7] = {0};
  uint8_t reg = 0x00;

  HAL_StatusTypeDef status;

  status = HAL_I2C_Master_Transmit(hi2c, DS3231_ADDR, &reg, 1, 100);
  if(status != HAL_OK)
    return status;

  status = HAL_I2C_Master_Receive(hi2c, DS3231_ADDR, buf, sizeof buf, 100);
  if(status != HAL_OK)
    return status;


  time->seconds = bcd_to_decimal(buf[0]);
  time->minutes = bcd_to_decimal(buf[1]);
  time->hours = bcd_to_decimal(buf[2]);

  time->date = bcd_to_decimal(buf[4]);
  time->month = bcd_to_decimal(buf[5]);
  time->year = bcd_to_decimal(buf[6]);

  return HAL_OK;
}

HAL_StatusTypeDef ds3231_write(I2C_HandleTypeDef *hi2c, ds3231_time_t *time) {
  uint8_t buf[8];
  buf[0] = 0x00;
  buf[1] = decimal_to_bcd(time->seconds);
  buf[2] = decimal_to_bcd(time->minutes);
  buf[3] = decimal_to_bcd(time->hours);
  buf[4] = 0x01;
  buf[5] = decimal_to_bcd(time->date);
  buf[6] = decimal_to_bcd(time->month);
  buf[7] = decimal_to_bcd(time->year);

  return HAL_I2C_Master_Transmit(hi2c, DS3231_ADDR, buf, 8, 100);
}

void ds3231_increment(ds3231_time_t *time, ds3231_field_t field){
  switch (field) {
    case DS3231_FIELD_SECONDS: 
      time->seconds = (time->seconds + 1) % 60;
      break;
    case DS3231_FIELD_MINUTES:
      time->minutes = (time->minutes +1) % 60;
      break;
    case DS3231_FIELD_HOURS:
      time->hours = (time->hours + 1) % 24;
      break;
    case DS3231_FIELD_DATE:
  
  }
}