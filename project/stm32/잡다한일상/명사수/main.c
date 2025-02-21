/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FND.h"

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
osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile uint16_t state = 0;
volatile uint16_t state_1 = 0;
volatile uint16_t state_2 = 0;

uint16_t score = 0;

const uint8_t gunshot_1 = 10;
const uint8_t gunshot_10 = 30;
const uint8_t gunshot_100 = 50;

uint32_t current_tick_1;
uint32_t current_tick_2;
uint32_t current_tick_3;
uint32_t old_tick_1;
uint32_t old_tick_2;
uint32_t old_tick_3;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    static uint32_t block_time = 0;  // 인터럽트를 블록할 기준 시간 저장
    static uint8_t is_blocking = 0; // 블록 상태를 나타내는 플래그

    uint32_t current_time = HAL_GetTick();

    // 블록 상태 확인
    if (is_blocking && (current_time - block_time < 500)) {
        // 현재 블록 상태이며 500ms 이내라면 인터럽트를 무시
        return;
    }

    // 블록 상태를 해제 (500ms 경과 시)
    if (current_time - block_time >= 500) {
        is_blocking = 0; // 블록 해제
    }

    // 인터럽트 핀에 따른 처리
    if (GPIO_Pin == han_Pin) {
        current_tick_1 = HAL_GetTick();
        int32_t diff_1 = (int32_t)(current_tick_1 - old_tick_1);
        if (diff_1 < 0) {
            diff_1 += UINT32_MAX + 1;
        }
        if (diff_1 > 200) {
            state++;
            old_tick_1 = current_tick_1;
        }
    } else if (GPIO_Pin == han2_Pin) {
        current_tick_2 = HAL_GetTick();
        int32_t diff_2 = (int32_t)(current_tick_2 - old_tick_2);
        if (diff_2 < 0) {
            diff_2 += UINT32_MAX + 1;
        }
        if (diff_2 > 200) {
            state_1++;
            old_tick_2 = current_tick_2;
        }
    } else if (GPIO_Pin == han3_Pin) {
        current_tick_3 = HAL_GetTick();
        int32_t diff_3 = (int32_t)(current_tick_3 - old_tick_3);
        if (diff_3 < 0) {
            diff_3 += UINT32_MAX + 1;
        }
        if (diff_3 > 200) {
            state_2++;
            old_tick_3 = current_tick_3;
        }
    }

    // 인터럽트 처리 완료 후 다른 인터럽트를 블록 상태로 설정
    is_blocking = 1;
    block_time = HAL_GetTick();  // 블록 시작 시간 설정
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, StartTask02, osPriorityIdle, 0, 128);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIO_PC13_LED_GPIO_Port, GPIO_PC13_LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PA3_LED_Pin|GPIO_PA4_LED_Pin|GPIO_PA5_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PA8_SCLK_Pin|GPIO_PA9_RCLK_Pin|GPIO_PA10_DIO_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : GPIO_PC13_LED_Pin */
  GPIO_InitStruct.Pin = GPIO_PC13_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIO_PC13_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : GPIO_PA2_Button_Pin */
  GPIO_InitStruct.Pin = GPIO_PA2_Button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIO_PA2_Button_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : GPIO_PA3_LED_Pin GPIO_PA4_LED_Pin GPIO_PA5_LED_Pin */
  GPIO_InitStruct.Pin = GPIO_PA3_LED_Pin|GPIO_PA4_LED_Pin|GPIO_PA5_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : han2_Pin han_Pin han3_Pin */
  GPIO_InitStruct.Pin = han2_Pin|han_Pin|han3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : GPIO_PA8_SCLK_Pin GPIO_PA9_RCLK_Pin GPIO_PA10_DIO_Pin */
  GPIO_InitStruct.Pin = GPIO_PA8_SCLK_Pin|GPIO_PA9_RCLK_Pin|GPIO_PA10_DIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  uint8_t checkButton = 0;


  for(;;)
  {
	  display_number(score, 80);
	  if(HAL_GPIO_ReadPin(GPIO_PA2_Button_GPIO_Port, GPIO_PA2_Button_Pin) == 0){
		  ++checkButton;
		  if(checkButton > 10){
			  HAL_GPIO_TogglePin(GPIO_PC13_LED_GPIO_Port, GPIO_PC13_LED_Pin);
			  score = 0;
			  checkButton = 0;
		  }
	  }

	  osDelay(10);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for(;;)
  {
      if (state != 0) {
          state = 0;
          HAL_GPIO_WritePin(GPIOA, GPIO_PA3_LED_Pin, GPIO_PIN_SET); // LED ON
          score += gunshot_1;
          osDelay(200); // 500ms
      } else {
          HAL_GPIO_WritePin(GPIOA, GPIO_PA3_LED_Pin, GPIO_PIN_RESET); // LED OFF
      }

      if (state_1 != 0) {
    	  state_1 = 0;
          HAL_GPIO_WritePin(GPIOA, GPIO_PA4_LED_Pin, GPIO_PIN_SET); // LED ON
          score += gunshot_10;
          osDelay(200); // 500ms
      } else {
          HAL_GPIO_WritePin(GPIOA, GPIO_PA4_LED_Pin, GPIO_PIN_RESET); // LED OFF
      }

      if (state_2 != 0) {
    	  state_2 = 0;
          HAL_GPIO_WritePin(GPIOA, GPIO_PA5_LED_Pin, GPIO_PIN_SET); // LED ON
          score += gunshot_100;
          osDelay(200); // 500ms
      } else {
          HAL_GPIO_WritePin(GPIOA, GPIO_PA5_LED_Pin, GPIO_PIN_RESET); // LED OFF
      }
  }
  /* USER CODE END StartTask02 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
