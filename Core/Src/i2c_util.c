#include "i2c_util.h"

uint8_t i2c_scan(I2C_HandleTypeDef *hi2c, uint8_t *out, uint8_t max) {
  uint8_t count = 0;

  for (uint8_t addr = 0x08; addr <= 0x77; addr++) {
    if (HAL_I2C_IsDeviceReady(hi2c, (uint16_t)(addr << 1), 2, 10) == HAL_OK) {
      if (count < max) {
        out[count] = addr;
      }

      count++;
    }
  }

  return count;
}
