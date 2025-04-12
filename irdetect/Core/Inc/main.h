/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define FLASH_IS_32K false
#define FLASH_IS_64K true
#define FLASH_IS_1024K false
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LEDG1_Pin GPIO_PIN_13
#define LEDG1_GPIO_Port GPIOC
#define LEDR1_Pin GPIO_PIN_14
#define LEDR1_GPIO_Port GPIOC
#define OUT1_Pin GPIO_PIN_15
#define OUT1_GPIO_Port GPIOC
#define EN1_Pin GPIO_PIN_0
#define EN1_GPIO_Port GPIOF
#define LEDG2_Pin GPIO_PIN_1
#define LEDG2_GPIO_Port GPIOF
#define LEDR2_Pin GPIO_PIN_0
#define LEDR2_GPIO_Port GPIOC
#define OUT2_Pin GPIO_PIN_1
#define OUT2_GPIO_Port GPIOC
#define EN2_Pin GPIO_PIN_2
#define EN2_GPIO_Port GPIOC
#define LEDG3_Pin GPIO_PIN_3
#define LEDG3_GPIO_Port GPIOC
#define LEDR3_Pin GPIO_PIN_0
#define LEDR3_GPIO_Port GPIOA
#define OUT3_Pin GPIO_PIN_1
#define OUT3_GPIO_Port GPIOA
#define UID_Pin GPIO_PIN_3
#define UID_GPIO_Port GPIOA
#define EN3_Pin GPIO_PIN_4
#define EN3_GPIO_Port GPIOF
#define LEDG4_Pin GPIO_PIN_5
#define LEDG4_GPIO_Port GPIOF
#define LEDR4_Pin GPIO_PIN_4
#define LEDR4_GPIO_Port GPIOA
#define OUT4_Pin GPIO_PIN_5
#define OUT4_GPIO_Port GPIOA
#define EN4_Pin GPIO_PIN_6
#define EN4_GPIO_Port GPIOA
#define LEDG5_Pin GPIO_PIN_7
#define LEDG5_GPIO_Port GPIOA
#define LEDR5_Pin GPIO_PIN_4
#define LEDR5_GPIO_Port GPIOC
#define OUT5_Pin GPIO_PIN_5
#define OUT5_GPIO_Port GPIOC
#define EN5_Pin GPIO_PIN_0
#define EN5_GPIO_Port GPIOB
#define LEDG6_Pin GPIO_PIN_1
#define LEDG6_GPIO_Port GPIOB
#define LEDR6_Pin GPIO_PIN_2
#define LEDR6_GPIO_Port GPIOB
#define OUT6_Pin GPIO_PIN_10
#define OUT6_GPIO_Port GPIOB
#define EN6_Pin GPIO_PIN_11
#define EN6_GPIO_Port GPIOB
#define LEDG7_Pin GPIO_PIN_12
#define LEDG7_GPIO_Port GPIOB
#define LEDR6B13_Pin GPIO_PIN_13
#define LEDR6B13_GPIO_Port GPIOB
#define OUT7_Pin GPIO_PIN_14
#define OUT7_GPIO_Port GPIOB
#define EN7_Pin GPIO_PIN_15
#define EN7_GPIO_Port GPIOB
#define LEDG8_Pin GPIO_PIN_6
#define LEDG8_GPIO_Port GPIOC
#define LEDR8_Pin GPIO_PIN_7
#define LEDR8_GPIO_Port GPIOC
#define OUT8_Pin GPIO_PIN_8
#define OUT8_GPIO_Port GPIOC
#define EN8_Pin GPIO_PIN_9
#define EN8_GPIO_Port GPIOC
#define LEDG9_Pin GPIO_PIN_8
#define LEDG9_GPIO_Port GPIOA
#define LEDR9_Pin GPIO_PIN_11
#define LEDR9_GPIO_Port GPIOA
#define OUT9_Pin GPIO_PIN_6
#define OUT9_GPIO_Port GPIOF
#define EN9_Pin GPIO_PIN_7
#define EN9_GPIO_Port GPIOF
#define LEDG10_Pin GPIO_PIN_15
#define LEDG10_GPIO_Port GPIOA
#define LEDR10_Pin GPIO_PIN_10
#define LEDR10_GPIO_Port GPIOC
#define OUT10_Pin GPIO_PIN_11
#define OUT10_GPIO_Port GPIOC
#define EN10_Pin GPIO_PIN_12
#define EN10_GPIO_Port GPIOC
#define LEDG11_Pin GPIO_PIN_2
#define LEDG11_GPIO_Port GPIOD
#define LEDR11_Pin GPIO_PIN_3
#define LEDR11_GPIO_Port GPIOB
#define OUT11_Pin GPIO_PIN_4
#define OUT11_GPIO_Port GPIOB
#define EN11_Pin GPIO_PIN_5
#define EN11_GPIO_Port GPIOB
#define LEDG12_Pin GPIO_PIN_6
#define LEDG12_GPIO_Port GPIOB
#define LEDR12_Pin GPIO_PIN_7
#define LEDR12_GPIO_Port GPIOB
#define OUT12_Pin GPIO_PIN_8
#define OUT12_GPIO_Port GPIOB
#define EN12_Pin GPIO_PIN_9
#define EN12_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
