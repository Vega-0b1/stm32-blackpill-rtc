#include "aht20.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_i2c.h"
#define AHT20_ADDR  (0x38 << 1)

HAL_StatusTypeDef aht20_init(I2C_HandleTypeDef *hi2c){
    HAL_Delay(100);
    return HAL_I2C_IsDeviceReady(hi2c, AHT20_ADDR, 3, 100);
}

HAL_StatusTypeDef aht20_read(I2C_HandleTypeDef *hi2c, aht20_t *out){
    uint8_t cmds[3] = {0xAC, 0x33, 0x00};
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(hi2c, AHT20_ADDR, cmds, sizeof cmds, 100);

    if(status != HAL_OK)
        return status;

    HAL_Delay(80);
    uint8_t data[6] = {0};

    status = HAL_I2C_Master_Receive(hi2c, AHT20_ADDR, data, sizeof data, 100);
    if(status != HAL_OK)
        return status;

    if((data[0] & 0x80)!=0)
        return HAL_BUSY;

    uint32_t raw_hum = (data[1] << 12 | data[2]<< 4 | data[3] >> 4);
    uint32_t raw_temp = ( (data[3] & 0xF) << 16 | data[4] << 8 | data[5] );
}