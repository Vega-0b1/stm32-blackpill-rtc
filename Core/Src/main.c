/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "button.h"
#include "ds3231.h"
#include "lcd.h"
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */
DS3231_Time rtc;
DS3231_Time temp;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick.
   */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  __HAL_RCC_I2C1_FORCE_RESET();
  HAL_Delay(10);
  __HAL_RCC_I2C1_RELEASE_RESET();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  Button_Init();
  LCD_Init(&hi2c1);

  uint8_t curr_state = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    switch (curr_state) {
    case 0:
      DS3231_Read(&rtc);
      if (rtc.hours == 0 && rtc.minutes == 0 && rtc.seconds == 0) {

        if (rtc.month == 2) {
          if (rtc.year % 4 == 0 && rtc.date > 29) {
            rtc.month = 3;
            rtc.date = 1;
            DS3231_Write(&rtc);
          } else if (rtc.date > 28) {
            rtc.month = 3;
            rtc.date = 1;
            DS3231_Write(&rtc);
          }
        } else if (rtc.month == 4 || rtc.month == 6 || rtc.month == 9 ||
                   rtc.month == 11) {
          if (rtc.date > 30) {
            rtc.month += 1;
            rtc.date = 1;
            DS3231_Write(&rtc);
          }

        } else {
          if (rtc.month == 12 && rtc.date > 31) {
            rtc.month = 1;
            rtc.date = 1;
            DS3231_Write(&rtc);
          } else if (rtc.date > 31) {
            rtc.month += 1;
            rtc.date = 1;
            DS3231_Write(&rtc);
          }
        }
      }
      LCD_Print_Rtc(&rtc, curr_state);
      if (Button_Pressed(BUTTON_MODE) == 1)
        curr_state += 1;
      break;

    case 1:
      DS3231_Read(&temp);
      rtc.seconds = temp.seconds;
      rtc.minutes = temp.minutes;

      LCD_Print_Rtc(&rtc, curr_state);
      if (Button_Pressed(BUTTON_MODE) == 1)
        curr_state += 1;
      else if (Button_Pressed(BUTTON_UP) == 1) {
        rtc.hours = (rtc.hours + 1) % 24;
        DS3231_Write(&rtc);
      }
      break;

    case 2:
      DS3231_Read(&temp);
      rtc.seconds = temp.seconds;
      rtc.minutes = temp.minutes;

      LCD_Print_Rtc(&rtc, curr_state);
      if (Button_Pressed(BUTTON_MODE) == 1) {
        curr_state += 1;
      } else if (Button_Pressed(BUTTON_UP) == 1) {
        rtc.minutes = (rtc.minutes + 1) % 60;
        DS3231_Write(&rtc);
      }
      break;

    case 3:
      DS3231_Read(&temp);
      rtc.seconds = temp.seconds;
      rtc.minutes = temp.minutes;

      LCD_Print_Rtc(&rtc, curr_state);
      if (Button_Pressed(BUTTON_MODE) == 1) {
        curr_state += 1;
      } else if (Button_Pressed(BUTTON_UP) == 1) {
        rtc.seconds = 0;
        DS3231_Write(&rtc);
      }
      break;

    case 4:
      DS3231_Read(&temp);
      rtc.seconds = temp.seconds;
      rtc.minutes = temp.minutes;

      LCD_Print_Rtc(&rtc, curr_state);
      if (Button_Pressed(BUTTON_MODE) == 1) {
        curr_state += 1;
      } else if (Button_Pressed(BUTTON_UP) == 1) {
        rtc.month = (rtc.month % 12) + 1;
        DS3231_Write(&rtc);
      }

      break;
    case 5:
      DS3231_Read(&temp);
      rtc.seconds = temp.seconds;
      rtc.minutes = temp.minutes;

      LCD_Print_Rtc(&rtc, curr_state);
      if (Button_Pressed(BUTTON_MODE) == 1) {
        curr_state += 1;
      } else if (Button_Pressed(BUTTON_UP) == 1) {
        if (rtc.month == 2) {
          if (rtc.year % 4 == 0) {
            rtc.date = (rtc.date % 29) + 1;
          } else {

            rtc.date = (rtc.date % 28) + 1;
          }
        } else if (rtc.month == 4 || rtc.month == 6 || rtc.month == 9 ||
                   rtc.month == 11) {

          rtc.date = (rtc.date % 30) + 1;
        } else {
          rtc.date = (rtc.date % 31) + 1;
        }
        DS3231_Write(&rtc);
      }
      break;

    case 6:
      DS3231_Read(&temp);
      rtc.seconds = temp.seconds;
      rtc.minutes = temp.minutes;

      LCD_Print_Rtc(&rtc, curr_state);
      if (Button_Pressed(BUTTON_MODE) == 1) {
        curr_state = 0;
        LCD_Clear();
      } else if (Button_Pressed(BUTTON_UP) == 1) {
        rtc.year = (rtc.year + 1) % 100;
        DS3231_Write(&rtc);
      }
      break;
    }
  }

  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
    Error_Handler();
  }
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return
   * state
   */
  __disable_irq();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
     file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
