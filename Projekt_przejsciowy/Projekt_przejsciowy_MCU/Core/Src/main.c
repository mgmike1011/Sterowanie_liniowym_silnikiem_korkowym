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
#include "A4988_Drive.h" // Biblioteka silnika sterownika silnika krokowego
#include "VL6180X.h" // Biblioteka czujnika odległości
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//
// Wybór sterowania
//
#define PETLA_OTWARTA 1
#define PETLA_ZAMKNIETA 2
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//
// Sterownik silnika
//
A4988_Drive	motor = {	.NAME = "MOTOR",
						.STEPS = 200,
						.RESOLUTION = 1,
						.PORT_DIR = DIR_GPIO_Port,
						.PIN_DIR = DIR_Pin,
						.PORT_ENABLE = ENABLE_GPIO_Port,
						.PIN_ENABLE = ENABLE_Pin,
						.PORT_MS1 = MS1_GPIO_Port,
						.PIN_MS1 = MS1_Pin,
						.PORT_MS2 = MS2_GPIO_Port,
						.PIN_MS2 = MS2_Pin,
						.PORT_MS3 = MS3_GPIO_Port,
						.PIN_MS3 = MS3_Pin,
						.STEP_mm_RESOLUTION = 0.005,
						.PORT_RESET = RESET_GPIO_Port,
						.PIN_RESET = RESET_Pin,
						.PORT_SLEEP = SLEEP_GPIO_Port ,
						.PIN_SLEEP = SLEEP_Pin,
						.TIM_STEP = &htim2,
						.TIM_STEP_CHANNEL = TIM_CHANNEL_1,
						.TIM_COUNTER_SLAVE = &htim3};
//
// Sensor odległości
//
VL6180X_ sensor;
volatile uint16_t Mesure_distance = 0;
//
// Zadana odległość
//
volatile uint16_t Set_distance = 0;
//
// Wybór sterowania
//
char control = PETLA_OTWARTA;
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
  MX_I2C2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  //
  // Inicjalizacja silnika
  //
  Init_A4988(&motor); // Domyślne parametry i włączenie pomiarów

  //
  // Inicjalizacja czujnika
  //
  VL6180X_Init(&sensor, &hi2c2); // Inicjalizacja połączenia
  configureDefault_VL6180X(&sensor); // Domyślne ustawienia pomiaru
  Mesure_distance = readRangeSingleMillimeters_VL6180X(&sensor); // Pomiar początkowy
  //
  // Sterowanie
  //
  if(control == PETLA_OTWARTA){
	  HAL_TIM_Base_Stop_IT(&htim4); // Wyłączenie pomiaru z czujnika
	  HAL_TIM_Base_Start_IT(motor.TIM_COUNTER_SLAVE); // Włączenie zliczania impulsów
	  Rotate_mm_A4988(&motor, Set_distance); // Obracanie
  }else if (control == PETLA_ZAMKNIETA){
	  HAL_TIM_Base_Start_IT(&htim4); // Włączenie pomiaru z częstotliwością 5Hz
	  Mesure_distance = readRangeSingleMillimeters_VL6180X(&sensor); // Pomiar początkowy
	  HAL_TIM_Base_Stop_IT(motor.TIM_COUNTER_SLAVE); // Wyłączenie zliczania impulsów
	  HAL_TIM_PWM_Start(motor.TIM_STEP, motor.TIM_STEP_CHANNEL); // Obracanie
  }
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
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	// Sterowanie w pętli otwartej - callback od sterownika silnika
	if(htim == motor.TIM_COUNTER_SLAVE){
		HAL_TIM_PWM_Stop(motor.TIM_STEP, motor.TIM_STEP_CHANNEL);
	}
	// Sterowanie w pętli zamkniętej - callback od pomiaru
	else if(htim->Instance == TIM4){
		Mesure_distance = readRangeSingleMillimeters_VL6180X(&sensor);
		if(Mesure_distance > Set_distance){
			HAL_TIM_PWM_Stop(motor.TIM_STEP, motor.TIM_STEP_CHANNEL);
		}
	}
}
//
// Bezpieczniki krańcowe
//
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == END_STOP_1_Pin){
//		TODO
	}else if(GPIO_Pin == END_STOP_2_Pin){
//		TODO
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
