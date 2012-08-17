#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <virtual_io.h>
#include "up.h"
#include "down.h"

#include <PA9.h>       // Include for PA_Lib

#include <maxmod9.h> // Include Maxmod

// Include the soundbank
#include "soundbank_bin.h"
#include "soundbank.h"

#include "cs_api.h"
// Converted using PAGfx
#include "all_gfx.h"
#include "game.h"
#include "menu.h"

static portTASK_FUNCTION( testTask1, pvParameters );
	
int main(void)
{
	xTaskCreate( testTask1,
					     (const signed char * const)"testTask1",
					     2048,
					     (  void * ) NULL,
					     tskIDLE_PRIORITY + 1,
					     NULL);

	vTaskStartScheduler();
	while(1);

	return 0;
}


static portTASK_FUNCTION( testTask1, pvParameters )
{
	// input here.

	// PAlib �ʱ�ȭ
	PA_Init();
	// PAlib VBL Reference �ʱ�ȭ
	PA_InitVBL();

	while(true)
	{
		// ���� ���ȭ�� ���
		PA_LoadBackground(UP_SCREEN, BG2, &bg_main_up);
		PA_LoadBackground(DOWN_SCREEN, BG2, &bg_main_down);

		int selnum=0;
		ret_next(); // ��ġ�� �ƹ����� ������ ���� ���·��� ����
		PA_WaitForVBL();
		PA_LoadBackground(DOWN_SCREEN, BG2, &bg_menu); // �޴�ȭ�� ���

		selnum = sel_menu();

		if(selnum == 1){
			// �޴�1 - ���ӼҰ� (�̱���)
		}
		else if(selnum == 2){
			// �޴�2 - Single Player
			init_singleplay(); // 1player �ʱ�ȭ
			singleplay_start(); // 1player ���ӽ���
		}
		else if(selnum == 3){
			// �޴�3 - Multi Player
		}
		else
			;
		PA_WaitForVBL();
	}
}

