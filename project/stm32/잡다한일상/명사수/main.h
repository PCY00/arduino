/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GPIO_PC13_LED_Pin GPIO_PIN_13
#define GPIO_PC13_LED_GPIO_Port GPIOC
#define GPIO_PA2_Button_Pin GPIO_PIN_2
#define GPIO_PA2_Button_GPIO_Port GPIOA
#define GPIO_PA3_LED_Pin GPIO_PIN_3
#define GPIO_PA3_LED_GPIO_Port GPIOA
#define GPIO_PA4_LED_Pin GPIO_PIN_4
#define GPIO_PA4_LED_GPIO_Port GPIOA
#define GPIO_PA5_LED_Pin GPIO_PIN_5
#define GPIO_PA5_LED_GPIO_Port GPIOA
#define han2_Pin GPIO_PIN_12
#define han2_GPIO_Port GPIOB
#define han2_EXTI_IRQn EXTI15_10_IRQn
#define han_Pin GPIO_PIN_13
#define han_GPIO_Port GPIOB
#define han_EXTI_IRQn EXTI15_10_IRQn
#define han3_Pin GPIO_PIN_14
#define han3_GPIO_Port GPIOB
#define han3_EXTI_IRQn EXTI15_10_IRQn
#define GPIO_PA8_SCLK_Pin GPIO_PIN_8
#define GPIO_PA8_SCLK_GPIO_Port GPIOA
#define GPIO_PA9_RCLK_Pin GPIO_PIN_9
#define GPIO_PA9_RCLK_GPIO_Port GPIOA
#define GPIO_PA10_DIO_Pin GPIO_PIN_10
#define GPIO_PA10_DIO_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
