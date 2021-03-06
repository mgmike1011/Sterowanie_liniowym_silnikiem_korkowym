/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "A4988_Drive.h"
#include "VL6180X.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FORWARD_NEEDLE 	RIGHT_DIR
#define BACK_NEEDLE 	LEFT_DIR
#define SUCK_SYRINGE	RIGHT_DIR
#define BLOW_SYRINGE	LEFT_DIR
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//
// Motor controllers
//
A4988_Drive Syringe = {	.NAME = "SYRINGE",
						.STEPS = 200,
						.RESOLUTION = 1,
						.PORT_DIR = DIR_S_GPIO_Port,
						.PIN_DIR = DIR_S_Pin,
						.PORT_ENABLE = ENABLE_S_GPIO_Port,
						.PIN_ENABLE = ENABLE_S_Pin,
						.PORT_MS1 = MS1_S_GPIO_Port,
						.PIN_MS1 = MS1_S_Pin,
						.PORT_MS2 = MS2_S_GPIO_Port,
						.PIN_MS2 = MS2_S_Pin,
						.PORT_MS3 = MS3_S_GPIO_Port,
						.PIN_MS3 = MS3_S_Pin,
						.STEP_mm_RESOLUTION = 0.005
						,.PORT_RESET = RESET_S_GPIO_Port,
						.PIN_RESET = RESET_S_Pin,
						.PORT_SLEEP = SLEEP_S_GPIO_Port ,
						.PIN_SLEEP = SLEEP_S_Pin
						};
A4988_Drive Needle = {	.NAME = "NEEDLE",
						.STEPS = 200,
						.RESOLUTION = 1,
						.STEP_mm_RESOLUTION = 0.005
						,.PORT_DIR = DIR_N_GPIO_Port,
						.PIN_DIR = DIR_N_Pin,
						.PORT_ENABLE = ENABLE_N_GPIO_Port,
						.PIN_ENABLE = ENABLE_N_Pin,
						.PORT_MS1 = MS1_N_GPIO_Port,
						.PIN_MS1 = MS1_N_Pin,
						.PORT_MS2 = MS2_N_GPIO_Port,
						.PIN_MS2 = MS2_N_Pin,
						.PORT_MS3 = MS3_N_GPIO_Port,
						.PIN_MS3 = MS3_N_Pin,
						.PORT_RESET = RESET_N_GPIO_Port,
						.PIN_RESET = RESET_N_Pin,
						.PORT_SLEEP = SLEEP_N_GPIO_Port,
						.PIN_SLEEP = SLEEP_N_Pin
						};
//
// Distance Sensors
//
VL6180X_ Syringe_sensor;
VL6180X_ Needle_sensor;
volatile uint16_t MESURE_needle = 0;
volatile uint16_t MESURE_syringe = 0;
//
// Control
//
volatile uint16_t Set_distance_needle = 0;
volatile uint16_t Set_distance_syringe = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_I2C2_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_I2C4_Init();
  /* USER CODE BEGIN 2 */
  // Drive initialization
Init_A4988(&Syringe);
Init_A4988(&Needle);
	// Sensor initialization
//VL6180X_Init(&Syringe_sensor, &hi2c4);
VL6180X_Init(&Needle_sensor, &hi2c2);
//configureDefault_VL6180X(&Syringe_sensor);
configureDefault_VL6180X(&Needle_sensor);
	// Measure timer Start
HAL_TIM_Base_Start_IT(&htim6); // Needle 	- 5Hz
HAL_TIM_Base_Start_IT(&htim7); // Syringe 	- 5Hz
// Initial measurement
MESURE_needle = readRangeSingleMillimeters_VL6180X(&Needle_sensor);
//MESURE_syringe = readRangeSingleMillimeters_VL6180X(&Syringe_sensor);
  /* USER CODE END 2 */

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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
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

/* USER CODE BEGIN 4 */
//
// TIM callback
//
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	// Motor driver callback
	if(htim->Instance == TIM3){	// Syringe
		HAL_TIM_PWM_Stop(&TIM_PWM_S, TIM_PWM_CHANNEL_S); // Stop syringe
	}
	else if(htim->Instance == TIM5){ // Needle
		HAL_TIM_PWM_Stop(&TIM_PWM_N, TIM_PWM_CHANNEL_N); // Stop needle
	}
	// Control callback
	else if (htim->Instance == TIM6) {
		MESURE_needle = readRangeSingleMillimeters_VL6180X(&Needle_sensor); // Measurement
		if(MESURE_needle > Set_distance_needle){
			HAL_TIM_PWM_Stop(&TIM_PWM_N, TIM_PWM_CHANNEL_N); // Stop needle
		}
	}
	else if (htim->Instance == TIM7) {
//		MESURE_syringe = readRangeSingleMillimeters_VL6180X(&Syringe_sensor); // Measurement
		if(MESURE_syringe > Set_distance_syringe){
			HAL_TIM_PWM_Stop(&TIM_PWM_S, TIM_PWM_CHANNEL_S); // Stop syringe
		}
	}
}
//
// Safety interlock (limit switch)
//
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == END_STOP_S_1_Pin){ // END STOP Syringe Near Drive
		HAL_TIM_PWM_Stop(&TIM_PWM_S, TIM_PWM_CHANNEL_S); // Stop syringe
		__HAL_TIM_SetCounter(&TIM_STEPS_COUNTER_S, 0); // Reset Counter Syringe
		Set_Direction_A4988(&Syringe, LEFT_DIR); // Set different direction
		Rotate_mm_A4988(&Syringe, 10); // Recoil
	}else if(GPIO_Pin == END_STOP_S_2_Pin){ // END STOP Syringe Near Syringe
		HAL_TIM_PWM_Stop(&TIM_PWM_S, TIM_PWM_CHANNEL_S); // Stop syringe
		__HAL_TIM_SetCounter(&TIM_STEPS_COUNTER_S, 0); // Reset Counter Syringe
		Set_Direction_A4988(&Syringe, RIGHT_DIR); // Set different direction
		Rotate_mm_A4988(&Syringe, 10); // Recoil
	}else if(GPIO_Pin == END_STOP_N_1_Pin){ // END STOP Needle Near Drive
		HAL_TIM_PWM_Stop(&TIM_PWM_N, TIM_PWM_CHANNEL_N); // Stop needle
		__HAL_TIM_SetCounter(&TIM_STEPS_COUNTER_N, 0); // Reset Counter Needle
		Set_Direction_A4988(&Needle, RIGHT_DIR); // Set different direction
		Rotate_mm_A4988(&Needle, 10); // Recoil
	}else if(GPIO_Pin == END_STOP_N_2_Pin){ // END STOP Needle Near Needle
		HAL_TIM_PWM_Stop(&TIM_PWM_N, TIM_PWM_CHANNEL_N); // Stop needle
		__HAL_TIM_SetCounter(&TIM_STEPS_COUNTER_N, 0); // Reset Counter Needle
		Set_Direction_A4988(&Needle, LEFT_DIR); // Set different direction
		Rotate_mm_A4988(&Needle, 10); // Recoil
	}
}
/* USER CODE END 4 */

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
