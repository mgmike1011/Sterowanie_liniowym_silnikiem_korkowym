/*
 * A4988_Drive.c
 *
 *  Created on: March 2022
 *      Author: Agnieszka Piórkowska, Miłosz Gajewski
 */
#include "A4988_Drive.h"
#include "math.h"
#include "main.h"
#include "stm32f7xx_hal.h"
void Set_Resolution_A4988(A4988_Drive* drive, int resolution){
	//
	//	Setting the resolution of the driver.
	//	@param resolution: Resolution.
	//	@param drive: Pointer to structure.
	//	@return: none
	//
	switch(resolution){
		case FULL_STEP:
			HAL_GPIO_WritePin(*&(drive->PORT_MS1), drive->PIN_MS1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(*&(drive->PORT_MS2), drive->PIN_MS2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(*&(drive->PORT_MS3), drive->PIN_MS3, GPIO_PIN_RESET);
			break;
		case HALF_STEP:
			HAL_GPIO_WritePin(*&(drive->PORT_MS1), drive->PIN_MS1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(*&(drive->PORT_MS2), drive->PIN_MS2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(*&(drive->PORT_MS3), drive->PIN_MS3, GPIO_PIN_RESET);
			break;
		case QUARTER_STEP:
			HAL_GPIO_WritePin(*&(drive->PORT_MS1), drive->PIN_MS1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(*&(drive->PORT_MS2), drive->PIN_MS2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(*&(drive->PORT_MS3), drive->PIN_MS3, GPIO_PIN_RESET);
			break;
		case ONE_EIGHTH_STEP:
			HAL_GPIO_WritePin(*&(drive->PORT_MS1), drive->PIN_MS1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(*&(drive->PORT_MS2), drive->PIN_MS2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(*&(drive->PORT_MS3), drive->PIN_MS3, GPIO_PIN_RESET);
			break;
		case ONE_SIXTEENTH_STEP:
			HAL_GPIO_WritePin(*&(drive->PORT_MS1), drive->PIN_MS1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(*&(drive->PORT_MS2), drive->PIN_MS2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(*&(drive->PORT_MS3), drive->PIN_MS3, GPIO_PIN_SET);
			break;
		default:
			HAL_GPIO_WritePin(*&(drive->PORT_MS1), drive->PIN_MS1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(*&(drive->PORT_MS2), drive->PIN_MS2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(*&(drive->PORT_MS3), drive->PIN_MS3, GPIO_PIN_SET);
	}
}
void Set_Direction_A4988(A4988_Drive* drive, int direction){
	//
	//	Setting the direction of rotation.
	//	@param direction: 1 - Right, 2 - Left, default - Right.
	//	@param drive: Pointer to structure.
	//	@return: none
	//
	switch(direction){
		case 1:
			HAL_GPIO_WritePin(*&(drive->PORT_DIR), drive->PIN_DIR, GPIO_PIN_SET);
			break;
		case 2:
			HAL_GPIO_WritePin(*&(drive->PORT_DIR), drive->PIN_DIR, GPIO_PIN_RESET);
			break;
		default:
			HAL_GPIO_WritePin(*&(drive->PORT_DIR), drive->PIN_DIR, GPIO_PIN_SET);
	}
}
void Enable_A4988(A4988_Drive* drive, int signal){
	//
	//	Enable controler.
	//	@param signal: ENABLE_DRIVE - 1, DISABLE_DRIVE - 2, default - disable.
	//	@param drive: Pointer to structure.
	//	@return: none
	//
	switch(signal){
		case ENABLE_DRIVE:
			HAL_GPIO_WritePin(*&(drive->PORT_ENABLE), drive->PIN_ENABLE, GPIO_PIN_RESET);
			break;
		case DISABLE_DRIVE:
			HAL_GPIO_WritePin(*&(drive->PORT_ENABLE), drive->PIN_ENABLE, GPIO_PIN_SET);
			break;
		default:
			HAL_GPIO_WritePin(*&(drive->PORT_ENABLE), drive->PIN_ENABLE, GPIO_PIN_SET);
	}
}
void Reset_A4988(A4988_Drive* drive, int reset){
	//
	//	Reset controler.
	//	@param reset: ENABLE_DRIVE - 1, DISABLE_DRIVE - 2, default - disable.
	//	@param drive: Pointer to structure.
	//	@return: none
	//
	switch(reset){
		case ENABLE_DRIVE:
			HAL_GPIO_WritePin(*&(drive->PORT_RESET), drive->PIN_RESET, GPIO_PIN_SET);
			break;
		case DISABLE_DRIVE:
			HAL_GPIO_WritePin(*&(drive->PORT_RESET), drive->PIN_RESET, GPIO_PIN_RESET);
			break;
		default:
			HAL_GPIO_WritePin(*&(drive->PORT_RESET), drive->PIN_RESET, GPIO_PIN_RESET);
	}
}
void Sleep_A4988(A4988_Drive* drive, int sleep){
	//
	//	Enable sleep mode.
	//	@param sleep: ENABLE_DRIVE - 1, DISABLE_DRIVE - 2, default - disable.
	//	@param drive: Pointer to structure.
	//	@return: none
	//
	switch(sleep){
		case ENABLE_DRIVE:
			HAL_GPIO_WritePin(*&(drive->PORT_SLEEP), drive->PIN_SLEEP, GPIO_PIN_SET);
			break;
		case DISABLE_DRIVE:
			HAL_GPIO_WritePin(*&(drive->PORT_SLEEP), drive->PIN_SLEEP, GPIO_PIN_RESET);
			break;
		default:
			HAL_GPIO_WritePin(*&(drive->PORT_SLEEP), drive->PIN_SLEEP, GPIO_PIN_RESET);
	}
}
int Calculate_Steps_A4988(A4988_Drive* drive, float angle, int resolution){
	//
	// Calculate steps steps needed to rotate by a given angle.
	// @param angle: Angle.
	// @param resolution: Set resolution.
	// @param drive: Pointer to structure.
	// @return: Number of steps.
	//
	int steps = 0;
	float r_s,x_s;
	r_s = 360/(drive->STEPS);
	x_s = r_s/resolution;
	steps = round(angle/x_s);
	return steps;
}