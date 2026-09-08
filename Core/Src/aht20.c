#include "aht20.h"

#define AHT20_ADDR (0x38 << 1)
static I2C_HandleTypeDef *i2c_handle;

aht20_status_t aht20_init(I2C_HandleTypeDef *hi2c) {
  i2c_handle = hi2c;
  HAL_Delay(100);
}
