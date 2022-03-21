/*
 * A4988_Drive.h
 *
 *  Created on: March 2022
 *      Author: Agnieszka Piórkowska, Miłosz Gajewski
 */

#ifndef INC_A4988_DRIVE_H_
#define INC_A4988_DRIVE_H_
#include "main.h"
#include "stm32f7xx_hal.h"
//
// Define
//
// Direction
#define RIGHT_DIR 	1 	//ClockWise
#define LEFT_DIR 	2 	//CounterClockWise
// Resolution
#define FULL_STEP 			1	// 1/1
#define HALF_STEP	 		2 	// 1/2
#define QUARTER_STEP 		3 	// 1/4
#define ONE_EIGHTH_STEP 	4 	// 1/8
#define ONE_SIXTEENTH_STEP 	5 	// 1/16
// Enable/Disable
#define ENABLE_DRIVE 	1
#define DISABLE_DRIVE 	2
//
// Structure
//
typedef struct{
	// Motor parameters:
	int STEPS;
	char NAME[10];
	// Software parameters:
	// Direction PORT/PIN
	GPIO_TypeDef	*PORT_DIR;
	uint16_t		PIN_DIR;
	// Resolution PORT/PIN
	GPIO_TypeDef 	*PORT_MS1,
				 	*PORT_MS2,
					*PORT_MS3;
	uint16_t 		PIN_MS1,
					PIN_MS2,
					PIN_MS3;
	int 			RESOLUTION;
	// Enable PORT/PIN
	GPIO_TypeDef* 	PORT_ENABLE;
	uint16_t 		PIN_ENABLE;
	// Reset PORT/PIN
	GPIO_TypeDef* 	PORT_RESET;
	uint16_t 		PIN_RESET;
	//Sleep PORT/PIN
	GPIO_TypeDef* 	PORT_SLEEP;
	uint16_t 		PIN_SLEEP;
	// Timer
}A4988_Drive;
//
// Functions
//
void Set_Resolution_A4988(A4988_Drive* drive, int resolution);
void Set_Direction_A4988(A4988_Drive* drive, int direction);
void Enable_A4988(A4988_Drive* drive, int signal);
void Reset_A4988(A4988_Drive* drive, int reset);
void Sleep_A4988(A4988_Drive* drive, int sleep);
int Calculate_Steps_A4988(A4988_Drive* drive, float angle);
#endif /* INC_A4988_DRIVE_H_ */
