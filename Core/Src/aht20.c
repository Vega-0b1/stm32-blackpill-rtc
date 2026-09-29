#include "aht20.h"
#define AHT20_ADDR  (0x38 << 1)
static I2C_HandleTypeDef *hi2c;

HAL_StatusTypeDef aht20_init(I2C_HandleTypeDef *i2c_handle){
    hi2c = i2c_handle;
    HAL_Delay(100);
    return HAL_I2C_IsDeviceReady(hi2c, AHT20_ADDR, 3, 100);
}

HAL_StatusTypeDef aht20_read(aht20_t *out){

}