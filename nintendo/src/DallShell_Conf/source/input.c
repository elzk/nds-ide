/*
 * input.c
 *
 *  Created on: 2010. 7. 6.
 *      Author: ��ǻ�Ͱ��а�
 */

#include "dall_conf.h"
#include "input.h"
#include "graphic.h"
#define COUNT 30

const char* addr_text;
char text[33]; // keyboard input
int text_chn; // keyboard input of channel

int key_behavior(Pads key) {
	if(keysDown() & KEY_UP) return DALLKEY_UP;//key.Held.Up) return DALLKEY_UP;
	else if(keysDown() & KEY_DOWN) return DALLKEY_DOWN;
	else if(keysDown() & KEY_LEFT) return DALLKEY_LEFT;
	else if(keysDown() & KEY_RIGHT) return DALLKEY_RIGHT;
	else if(keysDown() & KEY_A) return DALLKEY_A;
	else if(keysDown() & KEY_B) return DALLKEY_B;
	else if(keysDown() & KEY_X) return DALLKEY_X;
	else if(keysDown() & KEY_Y) return DALLKEY_Y;
	else if(keysDown() & KEY_L) return DALLKEY_L;
	else if(keysDown() & KEY_R) return DALLKEY_R;
	else if(keysDown() & KEY_START) return DALLKEY_START;
	else if(keysDown() & KEY_SELECT) return DALLKEY_SELECT;
	return 0;
}

u64 current_flag = 0x0000000F;    // stylus�� ���� ���, ���� ����� �� �� �ִ� ui_item flag (bitmap)
extern int current_focus;     // ���� focus�� �ִ� ui item

struct ui_info *ui_pushed = NULL;

void ui_loop(void)
{
    int i=0;
    Pads key;
   // int cnt=0;

    redraw_screen(); // ù��° ȭ���� �׸�
loop:

    if (REG_KEYINPUT) {
        switch (key_behavior(key)) {

            // ȭ��ǥ key 4�� ó��
            case DALLKEY_UP :
            	if(current_flag == 0xF02) {
            		selected--;
            	}
            	break;
            case DALLKEY_DOWN :
            	if(current_flag == 0xF02) {
            		if(selected < (count-1)) selected++;
            	}
            	//
            	break; //... // upkey ��Ű��
            case DALLKEY_LEFT :
            	iprintf("LEFT");
            	break; //... // upkey ��Ű��
            case DALLKEY_RIGHT :
            	iprintf("RIGHT");
            	break; //... // upkey ��Ű��
                //...

            // ���� (A) key ó�� - style�� ������ �Ͱ� ����
            case DALLKEY_A :

            	if(current_flag == 0xF02) {
            	iprintf("A");

            	}
            	if(current_flag == 0xF20) {
            		iprintf("AA\n");
            		ui_pushed = ui + 4;
            		                goto selected;
            	}
            	break;
            case DALLKEY_B :
            	iprintf("B button");
            	break;
            case DALLKEY_X :
            	break;
            case DALLKEY_Y :
            	break;
            case DALLKEY_L :
            	break;
            case DALLKEY_R :
            	break;
            case DALLKEY_START :
            	break;
            case DALLKEY_SELECT :
            	break;
                // ���� �޴� ���
            default: break;// �Ⱦ��� Ű�� ���Ȱų� ���� �߸��� ���
        }
    }


    if (!Stylus.Released)
    	goto loop;


	for (i = 0; i < COUNT; i++) {
        if (!(current_flag & ui[i].flag)){
        	continue;   // ��, ���� ȭ�鿡�� �Ⱥ��̴� ��ư�̸� skip
        }
        //�ش� ui�� ��������Ʈ ������ ���õȴٸ�
		if(Stylus.X > ui[i].x && Stylus.X < (ui[i].x + ui[i].img_widht) &&
		   Stylus.Y > ui[i].y && Stylus.Y < (ui[i].y + ui[i].img_height))
		{
			current_flag = ui[i].flag;
			load_sprite(i,ui[i].file_name_Pal,ui[i].file_name_Sprite);
			break;
		}
   }

   if (i == COUNT) // ��ư�� ���� �������� touch�� ��
       goto loop; // ����

   if (Stylus.Released) { // focus�� �ٲ�
	   ui_pushed = ui + i;
	   goto selected;
selected:
       current_flag = ui_pushed->next_flag;   // �� ��ư�� ���� ��, ȭ�鿡�� ó���� ��ư��� bitmap ��ü

       if(ui_pushed->redraw)
           redraw_screen();
       if(ui_pushed->bgflag)
    	   load_background(ui_pushed->bg);
       if (ui_pushed->action) 				   // �Լ��� ��ϵ� ��쿡��
           ui_pushed->action();                // �� ��ư�� ���õǸ� ����
       else if(ui_pushed->another_action)
    	   ui_pushed->another_action();

   }
   goto loop;
}

void redraw_screen()
{
	int i = 0;
    //clear_screen(); // ȭ���� �۰�
	UnLoad_Screen();
	for (i = 0; i < COUNT; i++) {
		if (!(current_flag & ui[i].flag)) {
			break;
		}
	}
    for (i = 0; i < COUNT; i++) {
        if (current_flag & ui[i].flag)
        	load_sprite(i,ui[i].file_name_Pal,ui[i].file_name_Sprite);
        if(ui_pushed->bgflag)
            load_background(ui_pushed->bg);
   }
}

int check_location(void) {
	if(Stylus.Released) {
	if(Stylus.Y >= 42 && Stylus.Y <= 48) {
		selected = 0;
		return selected;
	}
	if(Stylus.Y >= 49 && Stylus.Y <= 55) {
		selected = 1;
		return selected;
	}
	if(Stylus.Y >= 57 && Stylus.Y <= 62) {
		selected = 2;
		return selected;
	}
	if(Stylus.Y >= 64 && Stylus.Y <= 70) {
		selected = 3;
		return selected;
	}
	if(Stylus.Y >= 72 && Stylus.Y <= 78) {
		selected = 4;
		return selected;
	}
	if(Stylus.Y >= 81 && Stylus.Y <= 87) {
		selected = 5;
		return selected;
	}
	if(Stylus.Y >= 89 && Stylus.Y <= 96) {
		selected = 6;
		return selected;
	}
	if(Stylus.Y >= 97 && Stylus.Y <= 102) {
		selected = 7;
		return selected;
	}
	if(Stylus.Y >= 105 && Stylus.Y <= 111) {
		selected = 8;
		return selected;
	}
	if(Stylus.Y >= 112 && Stylus.Y <= 118) {
		selected = 9;
		return selected;
	}
	if(Stylus.Y >= 120 && Stylus.Y <= 126) {
		selected = 10;
		return selected;
	}
	if(Stylus.Y >= 128 && Stylus.Y <= 134) {
		selected = 11;
		return selected;
	}
	if(Stylus.Y >= 136 && Stylus.Y <= 142) {
		selected = 12;
		return selected;
	}
	if(Stylus.Y >= 143 && Stylus.Y <= 149) {
			selected = 13;
			return selected;
		}
	}
	return -1;
}

void key_inputString() {
	bool key_flag = true;
	PA_Init();
	PA_ResetBgSysScreen(0);

	PA_LoadDefaultText(0, 0);  // Initialise the text syste
	PA_LoadKeyboard(2, &keyboardcustom); // Load the keyboard on background 2...
	PA_KeyboardIn(20, 95); // This scrolls the keyboard from the bottom, until it's at the right position

	switch(current_flag)
	{
		case 1 : PA_OutputSimpleText(0, 7, 5, "Change SSID : "); break;
		case 2 : PA_OutputSimpleText(0, 7, 5, "Change IP Address : "); break;
		case 3 : PA_OutputSimpleText(0, 7, 5, "Change Gateway : "); break;
		case 4 : PA_OutputSimpleText(0, 7, 5, "Change SubnetMask : "); break;
		case 5 : PA_OutputSimpleText(0, 7, 5, "Change DNS1 : "); break;
		case 6 : PA_OutputSimpleText(0, 7, 5, "Change DNS2 : "); break;
		case 7 : PA_OutputSimpleText(0, 7, 5, "Change Channel : "); break;
	}
	memset(text,0,33);
	s32 nletter = 0; // Next letter to right. 0 since no letters are there yet
	char letter = 0; // New letter to write.

	// Infinite loop to keep the program running
	while(key_flag)
    {
	  	letter = PA_CheckKeyboard();

	  	if (letter > 31) { // there is a new letter
	  		text[nletter] = letter;
	  		nletter++;
	  	}
	  	else if(letter == PA_TAB){// TAB Pressed...
	  		u8 i;
	  		for (i = 0; i < 4; i++){ // put 4 spaces...
	  			text[nletter] = ' ';
	  			nletter++;
	  		}
	  	}
	  	else if ((letter == PA_BACKSPACE)&&nletter) { // Backspace pressed
	  		nletter--;
	  		text[nletter] = ' '; // Erase the last letter
	  	}
	  	else if (letter == '\n'){ // Enter pressed
	  		switch(current_flag)
	  		{
				case 1 : strcpy(ap.ssid, text);
						 ap.ssid_len = strlen(ap.ssid);
						 break;
				case 2 : Config.my_ip.s_addr = inet_addr(text); break;
				case 3 : Config.gateway.s_addr = inet_addr(text); break;
				case 4 : Config.subnet.s_addr = inet_addr(text); break;
				case 5 : Config.dns1.s_addr = inet_addr(text); break;
				case 6 : Config.dns2.s_addr = inet_addr(text); break;
				case 7 : text_chn = atoi(text);
						 ap.channel = text_chn; break; //?????
	  		}
	  		text[nletter] = letter;
	  		nletter++;
	  		key_flag = false;
	  	}
	    PA_OutputSimpleText(0, 8, 8, text); // Write the text
	    PA_WaitForVBL();
    }

	current_flag = 0x00000F0F;
	redraw_screen();
	PA_InitText(0,1);
	PA_SetTextTileCol(0,1);
	PA_OutputText(0,10,6,"%s\n", ap.ssid);
	PA_OutputText(0,10,8,"%d.%d.%d.%d\n",(int)(Config.my_ip.s_addr&255),(int)((Config.my_ip.s_addr>>8)&255),(int)((Config.my_ip.s_addr>>16)&255),(int)((Config.my_ip.s_addr>>24)&255));
	PA_OutputText(0,10,10,"%d.%d.%d.%d\n",(int)(Config.gateway.s_addr&255),(int)((Config.gateway.s_addr>>8)&255),(int)((Config.gateway.s_addr>>16)&255),(int)((Config.gateway.s_addr>>24)&255));
	PA_OutputText(0,10,12,"%d.%d.%d.%d\n",(int)(Config.subnet.s_addr&255),(int)((Config.subnet.s_addr>>8)&255),(int)((Config.subnet.s_addr>>16)&255),(int)((Config.subnet.s_addr>>24)&255));
	PA_OutputText(0,10,14,"%d.%d.%d.%d\n",(int)(int)(Config.dns1.s_addr&255),(int)((Config.dns1.s_addr>>8)&255),(int)((Config.dns1.s_addr>>16)&255),(int)((Config.dns1.s_addr>>24)&255));
	PA_OutputText(0,10,16,"%d.%d.%d.%d\n",(int)(int)(Config.dns2.s_addr&255),(int)((Config.dns2.s_addr>>8)&255),(int)((Config.dns2.s_addr>>16)&255),(int)((Config.dns2.s_addr>>24)&255));
	PA_OutputText(0,10,19,"%d\n", (int)ap.channel);
}
