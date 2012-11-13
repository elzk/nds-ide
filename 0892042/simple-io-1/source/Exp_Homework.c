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
void Exp_1_Homework_A(void) {
	u16 sw;
	u8 key_pressed1 = FALSE;
	u8 key_pressed2 = FALSE;
	int key_number = 1;

	writeb_virtual_io(BARLED1, key_number);

	while (1) {
		sw = NDS_SWITCH();
		if ((sw & KEY_LEFT) && (key_pressed1 == FALSE)) {
			key_pressed1 = TRUE;

			if (key_number < 128) {
				key_number = key_number * 2;
				writeb_virtual_io(BARLED1, key_number);
			}
		}

		if ((key_pressed1 == TRUE) && (!(sw & KEY_LEFT)))
			key_pressed1 = FALSE;

		if ((sw & KEY_RIGHT) && (key_pressed2 == FALSE)) {
			key_pressed2 = TRUE;

			if (key_number > 1) {
				key_number = key_number / 2;
				writeb_virtual_io(BARLED1, key_number);
			}
		}

		if ((key_pressed2 == TRUE) && (!(sw & KEY_RIGHT)))
			key_pressed2 = FALSE;

		/*
		 if (key_pressed == FALSE) {
		 if (((sw & KEY_LEFT) && (key_number < 127))) {

		 key_number = key_number * 2;
		 writeb_virtual_io(BARLED1, key_number);
		 }

		 if (((sw & KEY_RIGHT) && (key_number > 1))) {
		 key_pressed = TRUE;
		 key_number = key_number / 2;
		 writeb_virtual_io(BARLED1, key_number);
		 }
		 }

		 if (key_pressed == TRUE)
		 if(!(sw & KEY_RIGHT) || !(sw & KEY_LEFT))
		 key_pressed = FALSE;
		 */
		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed

}

// LED Bar Left-and-Right & Round (BARLED 1 and BARLED 2)
// L key - going left, R key - going right
void Exp_1_Homework_B(void) {

}