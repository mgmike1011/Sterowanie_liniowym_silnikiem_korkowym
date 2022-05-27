volatile uint16_t Set_distance = 0; // Zadana odległość
// Włączenie sterowania
if (control == PETLA_ZAMKNIETA){
	HAL_TIM_Base_Start_IT(&htim4); // Włączenie pomiaru z częstotliwością 5Hz
	Mesure_distance = readRangeSingleMillimeters_VL6180X(&sensor); // Pomiar początkowy
	HAL_TIM_Base_Stop_IT(motor.TIM_COUNTER_SLAVE); // Wyłączenie zliczania impulsów
	HAL_TIM_PWM_Start(motor.TIM_STEP, motor.TIM_STEP_CHANNEL); // Obracanie
}
// Praca w przerwaniu
if(htim->Instance == TIM4){ // Wykonanie pomiaru z zadaną częstotliwością
	Mesure_distance = readRangeSingleMillimeters_VL6180X(&sensor); // Wykonanie pomiaru
	if(Mesure_distance > Set_distance){ // Sprawdzenie odległości
		HAL_TIM_PWM_Stop(motor.TIM_STEP, motor.TIM_STEP_CHANNEL); // Zatrzymanie pracy silnika
	}
}