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
portTickType start_time_h=0;
u16 barled;

#define NUM_STATE	9
#define NUM_INPUT	3

// Actions
struct state_machine_x {
	int check_timer;
	int next_state[NUM_INPUT];
	void (* action[NUM_INPUT])(void *p);
};
enum { SW_ON, SW_OFF, TO };
int ledFlag=0;
void Short_Click(void *p)
{
	printf("Short_Click\n");
	int i,led=128;
	if(ledFlag==16){
		return;
	}
	else if(ledFlag>7){
		for(i=0;i<(ledFlag-8);i++){
				led=(led>>1)+128;
		}
		writeb_virtual_io(BARLED1, 0xFF);
		writeb_virtual_io(BARLED2, led);
		ledFlag++;
	}
	else{
		for(i=0;i<ledFlag;i++){
			led=(led>>1)+128;
		}
		writeb_virtual_io(BARLED1, led);
		ledFlag++;
	}
}
void Long_Click(void *p)
{
	printf("Long_Click\n");
	writeb_virtual_io(BARLED1, 0xFF);
	writeb_virtual_io(BARLED2, 0x00);
	ledFlag=8;
}
void ShortShort_Click(void *p)
{
	printf("ShortShort_Click\n");
	int i,led=0xFF;
	if (ledFlag==0)
		return;
	ledFlag--;
	if (ledFlag==0){
		writeb_virtual_io(BARLED1, 0x00);
		writeb_virtual_io(BARLED2, 0x00);
	}
	else if(ledFlag>8){
		for(i=16;i>ledFlag;i--){
			led=(led<<1);
		}
		writeb_virtual_io(BARLED1, 0xFF);
		writeb_virtual_io(BARLED2, led);
	}
	else if(ledFlag<9){
		for(i=8;i>ledFlag;i--){
			led=(led<<1);
		}
		writeb_virtual_io(BARLED1, led);
		writeb_virtual_io(BARLED2, 0x00);
	}
}
void ShortLong_Click(void *p)
{
	printf("ShortLong_Click\n");
	writeb_virtual_io(BARLED1, 0xFC);
	writeb_virtual_io(BARLED2, 0x00);
	ledFlag=6;
}
void LongShort_Click(void *p)
{
	writeb_virtual_io(BARLED1, 0xFF);
	writeb_virtual_io(BARLED2, 0xFF);
	ledFlag=16;
	printf("LongShort_Click\n");
}
void LongLong_Click(void *p)
{
	writeb_virtual_io(BARLED1, 0x00);
	writeb_virtual_io(BARLED2, 0x00);
	ledFlag=0;
	printf("LongLong_Click\n");
}
void f_ts(void *p)
{
	start_time_h = xTaskGetTickCount();
}
struct state_machine_x SM[NUM_STATE] = {
	0, {1,0,0},{f_ts,NULL,NULL},
	1, {1,3,2},{NULL,f_ts,NULL},
	0, {2,6,0},{NULL,f_ts,NULL},
	1, {4,3,0},{f_ts,NULL,Short_Click},
	1, {4,0,5},{NULL,ShortShort_Click,NULL},
	0, {5,0,0},{NULL,ShortLong_Click,NULL},
	1, {7,6,0},{f_ts,NULL,Long_Click},
	1, {7,0,8}, {NULL,LongShort_Click,NULL},
	0, {8,0,0}, {NULL,LongLong_Click,NULL}
};



void Exp_3_Homework(void)
{
    int state;
    int input;

    printf("EXP_Homework\n");

    state = 0;							// Initial State 0 : All LED Off
    writeb_virtual_io(BARLED1, 0);
    writeb_virtual_io(BARLED2, 0);

	while (1) {
			/* Step 0: Generate Input Event */
			if (SM[state].check_timer) {
				if ((xTaskGetTickCount() - start_time_h) >= MSEC2TICK(200)) {
					input = TO;
					goto do_action;		// Input happens
				}
			}
			if (NDS_SWITCH() & KEY_A)
				input = SW_ON;
			else
				input = SW_OFF;

			/* Step 1: Do Action */
	do_action:
			if (SM[state].action[input])
				SM[state].action[input](NULL);

			/* Step 2: Set Next State */
			state = SM[state].next_state[input];

			if (NDS_SWITCH() & KEY_START)
				break;
			vTaskDelay(MSEC2TICK(50));
		}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10));		// Wait while START KEY is being pressed
}