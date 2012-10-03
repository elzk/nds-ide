/*
 * Exp_Homework.c
 *
 *  Created on: 2011. 9. 22.
 *      Author: Minsuk Lee
 */

#include <stdio.h>			// C-Standard Header
#include <time.h>
#include <stdlib.h>

#include "FreeRTOS.h"		// Free RTOS Headers
#include "task.h"

#include <nds.h>			// NDS / Sevencore Board Headers
#include <sevencore_io.h>
#include "realio.h"

// LED Bar Left-and-Right (BARLED 1)
// LEFT key - going left, RIGHT key - going right
void
Exp_1_Homework_A(void)
{
	u16 sw;
	u16 button;
	button = 1;
	u8  key_pressed = TRUE;

	int count = 0;
	int num = 1;
	int i;

	while (1) {
		sw = NDS_SWITCH();
		writeb_virtual_io(BARLED1, button);
		writeb_virtual_io(BARLED2, 0);
		key_pressed = !key_pressed;

		if ((key_pressed == FALSE) && (sw & KEY_LEFT)) {
			if(button < 128){
				key_pressed = !key_pressed;
				button = button * 2;
				writeb_virtual_io(BARLED1, button);
			}
		}else if ((key_pressed == FALSE) && (sw & KEY_RIGHT)) {
			if(button > 1){
				key_pressed = !key_pressed;
				button = button / 2;
				writeb_virtual_io(BARLED1, button);
			}
			writeb_virtual_io(BARLED1, button);
		}

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}

	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed
}

// LED Bar Left-and-Right & Round (BARLED 1 and BARLED 2)
// L key - going left, R key - going right
void
Exp_1_Homework_B(void)
{

}
