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

#define MIN_LED 0x01
#define MAX_LED 0x80
#define LED1(x) writeb_virtual_io(BARLED1, x)
#define LED2(x) writeb_virtual_io(BARLED2, x)

static portTASK_FUNCTION(Sample_Task_1, pvParameters);
static portTASK_FUNCTION(Sample_Task_2, pvParameters);

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
	u8 led = MIN_LED;
	u8 keyR_pressed = FALSE;
	u8 keyL_pressed = FALSE;

	LED1(MIN_LED);

	while (1) {
		sw = NDS_SWITCH();

		if((sw & KEY_R) && (keyR_pressed == FALSE)) {
			keyR_pressed = TRUE;
			if(led > MIN_LED) led/=2;
			LED1(led);
			printf("R");
		}
		else if((sw & KEY_L) && (keyL_pressed == FALSE)) {
			keyL_pressed = TRUE;
			if(led < MAX_LED) led*=2;
			LED1(led);
			printf("L");
		}

		if((keyR_pressed == TRUE) && !(sw & KEY_R))
			keyR_pressed = FALSE;

		if((keyL_pressed == TRUE) && !(sw & KEY_L))
			keyL_pressed = FALSE;
	}
}

static
portTASK_FUNCTION(Homework_2, pvParameters)
{
	portTickType xLastWakeTime = xTaskGetTickCount();
	u8 led = MIN_LED;

	while (1) {
		LED2(led);
		printf(".");
		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(500));
		led = led < MAX_LED ? led*2 : MIN_LED;
	}
}
