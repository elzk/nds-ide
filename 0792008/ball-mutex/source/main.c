#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

#include "card_spi.h"
#include "gdbStub.h"
#include "gdbStubAsm.h"

#define COLOR_RED       RGB(31,  0,  0)
#define COLOR_GREEN     RGB(0,  31,  0)
#define COLOR_BLACK     RGB(0,   0,  0)

#define BOX_WIDTH	16
#define BOX_HEIGHT	16
#define MIN_X      0
#define MIN_Y      0
#define MAX_X		(SCREEN_WIDTH / BOX_WIDTH)
#define MAX_Y		(SCREEN_HEIGHT / BOX_HEIGHT)

#define DIRECTION_UP		1
#define DIRECTION_DOWN		2
#define DIRECTION_LEFT		3
#define DIRECTION_RIGHT		4

#define NUM_ROW		3
#define NUM_COL		3
#define NUM_TASK	(NUM_ROW + NUM_COL)


struct parameters {
	char *taskname;		// Task Name
	int direction;		// Current Moving Direction
	int basePoint;		// Starting Position
	u32 color;			// Ball Color
	int delay;			// Task Delay
};

struct parameters Param[NUM_TASK] = {
		{ "1", DIRECTION_RIGHT, 3,  COLOR_RED,   350 },
		{ "2", DIRECTION_RIGHT, 6,  COLOR_RED,   300 },
		{ "3", DIRECTION_RIGHT, 9,  COLOR_RED,   320 },
		{ "4", DIRECTION_DOWN,  4,  COLOR_GREEN, 370 },
		{ "5", DIRECTION_DOWN,  8,  COLOR_GREEN, 400 },
		{ "6", DIRECTION_DOWN,  12, COLOR_GREEN, 330 }
};

static portTASK_FUNCTION(Ball_Task, pvParameters);
void InitDebug(void);
void vStartExpTasks(void);
void draw_my_box(int pos_x, int pos_y, u16 color);

// ���⿡ �ʿ��� �� ��ŭ�� Semaphore �ڵ� ����  <--------
xSemaphoreHandle xSemaphore = NULL;

int
main(void)
{
	InitDebug();
	//init_virtual_io(ENABLE_SW | ENABLE_MATRIX);	// Enable Virtual LED's on Top Screen
	init_printf();							// Initialize Bottom Screen for printf()

	vStartExpTasks();
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

void
draw_my_box(int pos_x, int pos_y, u16 color)
{
	int i, j;
	u32 *basePoint, pixel;

	pixel = (color << 16) + color;
	for (i = 0; i < BOX_HEIGHT; i++) {
		basePoint = (u32 *)BG_GFX +
				((((pos_y * BOX_HEIGHT) + i) * SCREEN_WIDTH) + pos_x * BOX_WIDTH) / 2;
		for (j = 0; j < (BOX_WIDTH / 2); j++)
			*basePoint++ = pixel;
	}
}
void
vStartExpTasks(void)
{
	struct parameters *p;
	int i;

	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	bgInit(3,BgType_Bmp16, BgSize_B16_256x256, 0, 0);

	for (i = 0, p = Param; i < NUM_TASK; i++, p++)
		xTaskCreate(Ball_Task, (const signed char *)(p->taskname), 1024, (void *)p, tskIDLE_PRIORITY + 5, NULL);

	// ���⿡ �ʿ��� �� ��ŭ Semaphore �ʱ�ȭ (vSemaphoreCreateBinary) <--------
	vSemaphoreCreateBinary( xSemaphore );

}

static portTASK_FUNCTION(Ball_Task, pvParameters) {
	struct parameters *p = (struct parameters *)pvParameters;
	int x, y, prevX, prevY;
	int direction = p->direction;
	int i;

	// 여기에 각 Ball의 초기위치 (x, y) 설정
	if(p->direction == DIRECTION_RIGHT) {
		x = MIN_X;
		y = p->basePoint;
	}
	else if(p->direction == DIRECTION_DOWN) {
		x = p->basePoint;
		y = MIN_Y;
	}

	while(1) {
		draw_my_box(prevX, prevY, COLOR_BLACK);
		draw_my_box(x, y, p->color);

		prevX = x; prevY = y;

		if(NDS_SWITCH() & KEY_R) {
			if(direction == DIRECTION_RIGHT || direction == DIRECTION_LEFT) {
				for(i=0; i<NUM_TASK; i++) {
					if(p[i].direction == DIRECTION_UP || p[i].direction == DIRECTION_DOWN) {
						if(x == p[i].basePoint) {
							xSemaphoreTake( xSemaphore, 0 );
						}
					}
				}
			}
			else {
				for(i=0; i<NUM_TASK; i++) {
					if(p[i].direction == DIRECTION_RIGHT || p[i].direction == DIRECTION_LEFT) {
						if(y == p[i].basePoint) {
							xSemaphoreTake( xSemaphore, 0 );
						}
					}
				}
			}
		}

		// 각 Ball이 교차점에 있으면, Semaphore Give <--------
		if(direction == DIRECTION_RIGHT || direction == DIRECTION_LEFT) {
			for(i=0; i<NUM_TASK; i++) {
				if(p[i].direction == DIRECTION_UP || p[i].direction == DIRECTION_DOWN) {
					if(x == p[i].basePoint) {
						xSemaphoreGive( xSemaphore );
					}
				}
			}
		}
		else if(direction == DIRECTION_UP || direction == DIRECTION_DOWN) {
			for(i=0; i<NUM_TASK; i++) {
				if(p[i].direction == DIRECTION_RIGHT || p[i].direction == DIRECTION_LEFT) {
					if(y == p[i].basePoint) {
						xSemaphoreGive( xSemaphore );
					}
				}
			}
		}

		if(direction == DIRECTION_RIGHT) {
			if(x < (MAX_X - 1)) {
				x++;
			}
			else {
				direction = DIRECTION_LEFT;
				x--;
			}
		}
		else if (direction == DIRECTION_LEFT) {
			if(x > MIN_X) {
				x--;
			}
			else {
				direction = DIRECTION_RIGHT;
				x++;
			}
		}
		else if (direction == DIRECTION_DOWN) {
			if(y < (MAX_Y -1)) {
				y++;
			}
			else {
				direction = DIRECTION_UP;
				y--;
			}
		}
		else {
			if(y > MIN_Y) {
				y--;
			}
			else {
				direction = DIRECTION_DOWN;
				y++;
			}
		}
		vTaskDelay(p->delay);
	}
}