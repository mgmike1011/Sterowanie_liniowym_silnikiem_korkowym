// Inicjalizacja
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

volatile uint16_t Set_distance = 0;
Init_A4988(&motor);
// Sterowanie
Set_Resolution_A4988(&motor, HALF_STEP);
Rotate_A4988(&motor, 180);
// Wywołanie zatrzymania
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	// Sterowanie w pętli otwartej - callback od sterownika silnika
	if(htim == motor.TIM_COUNTER_SLAVE){
		HAL_TIM_PWM_Stop(motor.TIM_STEP, motor.TIM_STEP_CHANNEL);
	}
}