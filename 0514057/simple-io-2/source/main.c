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

static portTASK_FUNCTION(homework_1, pvParameters);
static portTASK_FUNCTION(homework_2, pvParameters);

void InitDebug(void);

int
main(void)
{
	InitDebug();
	init_virtual_io(ENABLE_LED);	// Enable Virtual IO Devices
	init_printf();					// Initialize Bottom Screen for printf()

	xTaskCreate(homework_1,
					     (const signed char * const)"homework_1",
					     2048,
					     (void *)NULL,
					     tskIDLE_PRIORITY + 2,
					     NULL);
	xTaskCreate(homework_2,
					     (const signed char * const)"homework_2",
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
portTASK_FUNCTION(homework_1, pvParameters)
{
	u16 sw,bsw=KEY_UP;
	int led=8,i;
	long temp=2.33124;
	while (1) {
		sw = NDS_SWITCH();
		writeb_virtual_io(BARLED1, led);
		if(sw!=bsw){
			if ((sw & KEY_R) && led!=1)
				led=led/2;
			if ((sw & KEY_L) && led!=128)
				led=led*2;
		}
		bsw=sw;
		for(i=0;i<10000;i++)
			temp=temp/0.3413;

		vTaskDelay(MSEC2TICK(1));
	}
}

static
portTASK_FUNCTION(homework_2, pvParameters)
{
	u8 barled = 1;
	int t = 0;
	portTickType xLastWakeTime = xTaskGetTickCount();

	while (1) {
		printf(" %d",t);
		t++;
		writeb_virtual_io(BARLED2, barled);
		if(barled!=128)
			barled=barled*2;
		else
			barled=1;
		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(500));
	}
}
