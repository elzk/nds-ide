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

// PAGfxConverter Include
#include "includefile.h"

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

	//���� ����
	mmLoad(MOD_GUNSHOT);
	mmLoad(MOD_KOONG);
	mmLoad(MOD_YEAH);

	while(1){
		Screen scr;

		scr.Ready_Screen();		// ��ŸƮ�� ���������� ������.

		mmStart(MOD_YEAH, MM_PLAY_LOOP); // ������� ����

		while(1){
			scr.SCreen_Change();

			Stage Game;		// game ��ü ����.

			while(1){
				Game.Location_Update();		// ��� ��ü�� ��ǥ���� ���°� ����.
				Game.Image_Update();		// ��� ��ü �̹��� �ѷ���.

				if(Pad.Newpress.Start && Game.isGameOver == 1) break;	// ���ӵ��� ���������.

				PA_WaitForVBL();
			}
			if(Game.isGameOver == 1){		// ��������� �ʱ���·� ���ư�.
				break;
			}
			Game.~Stage();
			PA_WaitForVBL();
		}
		scr.~Screen();
		PA_WaitForVBL();
	}

	//return 0;
}

