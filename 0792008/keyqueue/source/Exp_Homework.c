#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

extern xQueueHandle KeyQueue;
#define MAX_KEY_LOG		10

void
key_init(void) // Queue 초기화
{
	int i;
	u8 key;

	for (i = 0; i < MAX_KEY_LOG; i++)
		xQueueReceive(KeyQueue, &key, 0);
}

int
kbhit(void) // Queue에 값은 그대로 두고 값만 return
// 키보드가 눌렸는지 알아냄
{
	u8 key;
	int ret = xQueuePeek(KeyQueue, &key, 0);
	return (ret == pdPASS);
}
u8
getkey(void)
{
	u8 key;

	xQueueReceive(KeyQueue, &key, portMAX_DELAY);
	//while (pdPASS != xQueueReceive(KeyQueue, &key, 0))
	// 기다리지 않고 바로 Queue 아이템 값 리턴
	//		vTaskDelay(MSEC2TICK(5));
	return key;
}

void
Exp_5_Homework_A(void)
{
	int i;
	int count = 0;
	u8 key;
	u8 key_save[NUM_7SEG_LED];

	key_init();

	while(1) {
		if(key = getkey()) {
			key_save[count] = key;
			for(i=0; i <= count; i++) {
				writeb_virtual_io(SEG7LED, 0x70 - (i << 4) + key_save[count - i]);
			}
			count++;

			if (count == NUM_7SEG_LED) {
				count = 7;
				for (i = 0; i < NUM_7SEG_LED; i++) {
					key_save[i] = key_save[i + 1];
					key_save[count] = key;
				}
			}
		}
		vTaskDelay(MSEC2TICK(5));
	}
}

void
Exp_5_Homework_B(void)
{
	key_init();
}

portTASK_FUNCTION(Key_Task, pvParameters)
{
	u8 key, scan = 0;
	u8 pressed = FALSE;

	while(1) {
		if(!pressed) {
			writeb_virtual_io(KEY_MATRIX, 0x80 >> scan);
			key = scan * 4;
			switch (readb_virtual_io(KEY_MATRIX)) {
			case 8 :  key += 1; break;
			case 4 :  key += 2; break;
			case 2 :  key += 3; break;
			case 1 :  key += 4; if (key == 16) key = 0; break;
			default : key = 255; break;
			}
			scan++;
			if (scan == 4)
				scan = 0;
			if (key < 16) {
				xQueueSend(KeyQueue, &key, 0);
				pressed = TRUE;
			}
		}

		if(pressed && !readb_virtual_io(KEY_MATRIX))
			pressed = FALSE;

		vTaskDelay(MSEC2TICK(30) );
	}
}
