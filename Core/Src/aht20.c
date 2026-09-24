#include "aht20.h"

#define AHT20_ADDR (0x38 << 1)
static I2C_HandleTypeDef *i2c_handle;

aht20_status_t aht20_init(I2C_HandleTypeDef *hi2c) {
  i2c_handle = hi2c;
  HAL_Delay(100);
  uint8_t status;
  if(HAL_I2C_Master_Receive(i2c_handle, AHT20_ADDR, &status, 1, 100) != HAL_OK)
    return AHT20_ERR_I2C;

  if((status & 0x18) != 0x18)
    return AHT20_ERR_UNCALIBRATED;

  return AHT20_OK;
}
