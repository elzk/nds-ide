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
					     tskIDLE_PRIORITY + 5,
					     NULL);
	xTaskCreate(Homework_2,
					     (const signed char * const)"Homework_2",
					     2048,
					     (void *)NULL,
					     tskIDLE_PRIORITY + 2,
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
		u8 i = 0x80; // LED 상태 값
		u8 key_pressed = FALSE; // 버튼이 눌러진 상태 확인

		writeb_virtual_io(BARLED1, 0); // 초기화
		writeb_virtual_io(BARLED2, 0); // 초기화

		while (1) {

			sw = NDS_SWITCH();

			// 손이 버튼에서 떼진 상태이고 왼쪽 방향키를 눌렀을 경우
			// 그리고 BARLED1 상태가 제일 왼쪽 LED가 켜지지 않은 경우
			if ((key_pressed == FALSE) && (sw & KEY_L) && (i < 0x80)) {
				key_pressed = TRUE;

				i = i << 1; // 왼쪽으로 LED 하나 이동

				writeb_virtual_io(BARLED1, i); // BARLED1 상태 변경 표시
			}

			// 손이 버튼에서 떼진 상태이고 오른쪽 방향키를 눌렀을 경우
			// 그리고 BARLED1 상태가 제일 오른쪽 LED가 켜지지 않은 경우
			else if ((key_pressed == FALSE) && (sw & KEY_R) && (i > 0x01)) {
				key_pressed = TRUE;

				i = i >> 1; // 오른쪽으로 LED 하나 이동

				writeb_virtual_io(BARLED1, i); // BARLED1 상태 변경 표시
			}

			// 손이 버튼에서 떼졌는지 확인
			if ((key_pressed == TRUE) && (!(sw & KEY_L)) && (!(sw & KEY_R)))
				key_pressed = FALSE;

			vTaskDelay(50);

		while (NDS_SWITCH() & KEY_START)
			vTaskDelay(10); // Wait while START KEY is being pressed
	}
}

static
portTASK_FUNCTION(Homework_2, pvParameters)
{
	u8 barled = 0;
	portTickType xLastWakeTime = xTaskGetTickCount();
	u16 a = 0x0001; // LED 상태 값

	while (1) {
		writeb_virtual_io(BARLED2, a);

		a = a << 1;
		if (a == 0x0080)
			a = 0x0001;

		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(500));
	}
}