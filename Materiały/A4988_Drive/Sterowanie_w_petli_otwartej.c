void Rotate_mm_A4988(A4988_Drive* drive, float distance){
	float step_res_ = 360/(float)(drive->STEPS); // Obliczenie podstawowej rozdzielczości silnika 360/200=1.8stopnia
	float angle = (distance*step_res_)/((float)(drive->STEP_mm_RESOLUTION)); // Zamiana zadanego dystansu na kąt do obrotu
	int steps = 0; // Ilość krokó do wykonania - inicjalizacja
	float r_s,x_s;
	r_s = 360/((float)(drive->STEPS));
	x_s = r_s/((float)(drive->RESOLUTION)); // Obliczenie rozdzielczości silnika ze sterownikiem
	steps = (int)round(angle/x_s); // Ilość kroków do wykonania
	__HAL_TIM_SET_AUTORELOAD(drive->TIM_COUNTER_SLAVE,steps); // Ustawienie zliczania impulsów
	Sleep_A4988(drive, ENABLE_DRIVE); // Odblokowanie sterownika
	HAL_TIM_PWM_Start(drive->TIM_STEP, drive->TIM_STEP_CHANNEL); // Rozpoczęcie wysyłania impulsów
}