/*
 * main.c
 *
 *  Created on: 2011. 9. 24.
 *      Author: Minsuk Lee
 */

#include <stdio.h>          // C-Standard Header
#include <time.h>
#include <stdlib.h>

#include <FreeRTOS.h>       // Free RTOS Headers
#include <task.h>
#include <queue.h>
#include <semphr.h>

#include <nds.h>            // NDS / Sevencore Board Headers
#include <sevencore_io.h>

#include "card_spi.h"
#include "gdbStub.h"
#include "gdbStubAsm.h"

static portTASK_FUNCTION(Homework_1, pvParameters);
static portTASK_FUNCTION(Homework_2, pvParameters);

void InitDebug(void);

int
main(void)
{
	InitDebug();
	init_virtual_io(ENABLE_LED);	// Enable Virtual IO Devices
	init_printf();					// Initialize Bottom Screen for printf()

	xTaskCreate(Homework_1,
					     (const signed char * const)"Homework_1",
					     2048,
					     (void *)NULL,
					     tskIDLE_PRIORITY + 2,
					     NULL);

	xTaskCreate(Homework_2,
					     (const signed char * const)"Homework_2",
					     2048,
					     (void *)NULL,
					     tskIDLE_PRIORITY + 1,
					     NULL);

	vTaskStartScheduler();		// Never returns
	while(1)
		;
	return 0;
}

void
InitDebug(void)
{
#ifdef DEBUG
	irqInit();
	initSpi();
	initDebug();
	BreakPoint();
#endif
}

static
portTASK_FUNCTION(Homework_1, pvParameters)
{
	u16 sw;

	u8 button;
	button = 0x01;
	u8  key_pressed = FALSE;

	while (1) {
		printf("1");
		sw = NDS_SWITCH();
		writeb_virtual_io(BARLED1, button);

		if ((key_pressed == FALSE) && (sw & KEY_L)) {
			if(button < 0x80){
				button = button << 1;
				key_pressed = TRUE;
				writeb_virtual_io(BARLED1, button);
			}
		}else if ((key_pressed == FALSE) && (sw & KEY_R)) {
			if(button > 0x01){
				button = button >> 1;
				key_pressed = TRUE;
				writeb_virtual_io(BARLED1, button);
			}
		}

		if (key_pressed == TRUE){
			if(!(sw & KEY_R) && !(sw & KEY_L))
				key_pressed = FALSE;
		}

		vTaskDelay(50);
	}
}

static
portTASK_FUNCTION(Homework_2, pvParameters)
{
	u8 barled = 0x01;
	portTickType xLastWakeTime = xTaskGetTickCount();

	writeb_virtual_io(BARLED2, barled);
	while (1) {
		printf("2");
		if(barled < 0x80 && barled > 0x00){
			barled = barled << 1;
			writeb_virtual_io(BARLED2, barled);
		}else{
			barled = 0x01;
			writeb_virtual_io(BARLED2, barled);
		}

		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(500));
	}
}