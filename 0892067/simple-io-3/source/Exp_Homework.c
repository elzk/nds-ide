/*
 * Exp_Homework.c
 *
 *  Created on: 2011. 9. 22.
 *      Author: Minsuk Lee
 */

#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

portTickType short_timer;
u16 barled;

#define NUM_STATE	1
#define NUM_INPUT	1

// Actions

struct state_machine_x {
	int check_timer;
	int next_state[NUM_INPUT];
	void (* action[NUM_INPUT])(void *p);
};
enum { SW_ON, SW_OFF, TO };

struct state_machine_x SM[NUM_STATE] = {
	// Fill here
};

void
Exp_3_Homework(void)
{
    // variables

	while (1) {
		/* Step 0: Generate Input Event */

		/* Step 1: Do Action */

		/* Step 2: Set Next State */

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(50));
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10));		// Wait while START KEY is being pressed
}