/*
 * Exp_Sample.c
 *
 *  Created on: 2011. 11. 9.
 *      Author: Minsuk Lee
 */

// Free RTOS Headers
// See AI reference Document in http://www.freertos.org/
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>

#include <nds.h>
#include <sevencore_io.h>

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define COLOR_RED       RGB(31,  0,  0) /* Bright Red  	*/
#define COLOR_WHITE     RGB(31, 31, 31) /* Bright White */
#define COLOR_BLACK     RGB( 0,  0,  0)
#define COLOR_GREEN     RGB( 0, 31, 0)

#define BOX_WIDTH	16
#define BOX_HEIGHT	16
#define MAX_X		(SCREEN_WIDTH / BOX_WIDTH)
#define MAX_Y		(SCREEN_HEIGHT / BOX_HEIGHT)

// define added 11/10/2011
#define BG_GFX			((u16*)0x6000000)
#define SCREEN_WIDTH	256

struct parameters{
	char *taskname;
	int direction;
	int basePoint;
	u32 color;
	int delay;
};

// fucntion added 11/10/2011
void key_init(void) {
	int i;
	u8 key;

	for (i = 0; i < MAX_KEY_LOG; i++)
		xQueueReceive(KeyQueue, &key, 0);
}

int kbhit(void) {
	u8 key;
	int ret = xQueuePeek(KeyQueue, &key, 0);
	return (ret == pdPASS);
}
u8 getkey(void) {
	u8 key;
	xQueueReceive(KeyQueue, &key, portMAX_DELAY);
	//while (pdPASS != xQueueReceive(KeyQueue, &key, 0))
	//		vTaskDelay(MSEC2TICK(5));
	return key;
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
Exp_Sample(void)
{
    u8 key, old_key = -1;

    while (1) {
 		if (!kbhit()) {
 	 		if (NDS_SWITCH() & KEY_START)
 				break;
 			vTaskDelay(30);
 			continue;
 		}
 		key=getkey();
 		if(key==old_key)
 			continue;
 		if(old_key>=0)
 			draw_my_box(old_key,8, COLOR_BLACK);
 		draw_my_box(key,8,COLOR_RED);
 		old_key=key;
    }
    draw_my_box(old_key,8,COLOR_BLACK);
		// Erase the Previous Box
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed
}
void Exp_Homework(struct parameters *p){
u8 key=0,old_key=-1,key_move=-1;

while(1){
	if(p->direction){
		draw_my_box(old_key,p->basePoint,COLOR_BLACK);
		draw_my_box(key,p->basePoint,p->color);
		vTaskDelay(300);
		draw_my_box(key,p->basePoint,COLOR_BLACK);
		old_key=key;

		if(key==15 || key==0)
			key_move *=-1;
		if(key<15 || key>=0)
					key +=key_move;
	}
	else{
		draw_my_box(p->basePoint,old_key,COLOR_BLACK);
		draw_my_box(p->basePoint,key,p->color);
		vTaskDelay(300);
		draw_my_box(p->basePoint,key,COLOR_BLACK);
		old_key=key;
		if(key==11||key==0)
			key_move*=-1;
		if(key<11||key>=0)
					key+=key_move;
	}
	vTaskDelay(p->delay);
}
}