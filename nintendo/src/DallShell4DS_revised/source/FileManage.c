/*
 *  ShellFileManage.c
 *
 *  Created on: 2011. 7. 24.
 *  Author: Jaewon Lee
 *  version: 1.0
 *
 */

#include "Common.h"
#include "dallshell.h"
#include "FileManage.h"
#include "Graphic.h"

#define ADD         0
#define DELETE      1
#define MOVE        2
#define ALERT       3

int device_selector=0; // �ٿ�ε带 ���� Device Selector

/*
 * ���� �ʱ�ȭ  �Լ�
 */
void fileInit() {
	position = 0;
	selected = 0;
	files = 0;
	alive = 1;
	strcpy(cur_dir, "/");
	file_tsize = " ";
	sel_created = false;
	textheight = 6;
	mode = 0;
	re = 0;
}

/**
 * ���� ������ �Լ�
 * @param change ������ ���ϸ�, ���Ӱ� �ٲ� ���丮 �ּҷ� Open
 */
void fileListing(bool change) {
	// �ּҰ� ��������, cur_dir�� ���� �����ش�.
	if (change == true) {
		pdir = opendir(cur_dir); // diropen returns NULL and sets errno on failure.
	} else {
		pdir = NULL;
	}
	if (pdir == NULL)
	{
//		PA_OutputText(1, 0, 2, "Fat Error or running emu.");
	}
	// cur_dir�� �ּҰ� �����ϸ�, ������ �ø���,
	if (pdir == NULL) {;}
	else
	{
		while ((pent = readdir(pdir)) != NULL) {
			stat(pent->d_name, &st);
			if(!strcmp(pent->d_name,".") || !strcmp(pent->d_name,".."))
			{
				continue;
			}
			else
			{
				sprintf(list[files].label, "%s", pent->d_name);
				if (S_ISDIR(st.st_mode)) {
					list[files].ext = 0; // ���丮�̸�, Ȯ���ڸ� ��Ÿ���� ext�� 0,
				}
				else {
					getextension(files); // �׷��� ������ ������ Ȯ���ڸ� ������ �Լ��� ����
				}
				files++; // ���� ī��Ʈ�� ����
			}
		}
		closedir(pdir);
	}
}

/**
 * �빮�ڸ� �ҹ��ڷ� ǥ�����ִ� �Լ�
 * @param char[] �Է¹��� �빮�� char[] (���� �̻��)
 */
void lowercase(char string[]) {
	int i = 0;

	while (string[i]) {
		string[i] = tolower(string[i]);
		i++;
	}
	return;
}

/**
 * ������ Ȯ���ڿ� ���� ��ȯ ����� �����ִ� �Լ�
 * @param number ���� ī��Ʈ �ѹ�
 */
void getextension(int number) {
	//0=folder 1=unknown 2=sound 3=image 4=text 5=game 9=GoBackFolder
	char ext[3];
	int i;
	if(strcmp(list[number].label,".")==0) {
		list[number].ext = 9;
	}
	else if(strcmp(list[number].label, "..") == 0) {
		list[number].ext = 0;
	}
	else
	{
		i = strlen(list[number].label);

		ext[2] = list[number].label[i - 1];
		ext[1] = list[number].label[i - 2];
		ext[0] = list[number].label[i - 3];
		ext[3] = '\0';
		lowercase(ext);
		if (strcmp(ext, "mp3") == 0) {
			list[number].ext = 2;
		} else if (strcmp(ext, "wav") == 0) {
			list[number].ext = 2;
		} else if (strcmp(ext, "mid") == 0) {
			list[number].ext = 2;
		} else if (strcmp(ext, "snd") == 0) {
			list[number].ext = 2;
		} else if (strcmp(ext, "png") == 0) {
			list[number].ext = 3;
		} else if (strcmp(ext, "bmp") == 0) {
			list[number].ext = 3;
		} else if (strcmp(ext, "jpg") == 0) {
			list[number].ext = 3;
		} else if (strcmp(ext, "gif") == 0) {
			list[number].ext = 3;
		} else if (strcmp(ext, "tif") == 0) {
			list[number].ext = 3;
		} else if (strcmp(ext, "cfg") == 0) {
			list[number].ext = 4;
		} else if (strcmp(ext, "doc") == 0) {
			list[number].ext = 4;
		} else if (strcmp(ext, "rtf") == 0) {
			list[number].ext = 4;
		} else if (strcmp(ext, "ini") == 0) {
			list[number].ext = 4;
		} else if (strcmp(ext, "txt") == 0) {
			list[number].ext = 4;
		} else if (strcmp(ext, "nds") == 0) {
			list[number].ext = 5;
		} else if (strcmp(ext, "gba") == 0) {
			list[number].ext = 5;
		} else {
			list[number].ext = 1;
		}
	}
}

/**
 * ȭ�鿡 ���ϵ��� ǥ�����ִ� �Լ�
 * @param filname ���ϸ� ext Ȯ����
 */
void createitem(char *filename, s8 ext) {
	PA_SetTextTileCol(0, TEXT_BLACK);

	char check[2];
	strncpy(check, filename, 1);
	// 6�� ������ �ѷ���
	if ((number_of_files < 5) & (ext != 10))
	{
		// ���� ���丮 �̵�
		if (strcmp(filename, "..") == 0)
		{
			PA_OutputSimpleText(0, 3,
					(number_of_files + 7) + (number_of_files * 2),
					"Go to Parent Directory");
			number_of_files++; //���� �� ����
		}

		// ���ϸ� ��
		else
		{
			if (rv >= 0) // ��ü�� ���� ���
			{
				int fileSize = st.st_size;
				fileSize = fileSize / 1024;

				if (S_ISDIR(st.st_mode)) {
					PA_OutputSimpleText(0, 3,
							(number_of_files + 7) + (number_of_files * 2),
							filename);
					PA_OutputSimpleText(0, 25,
							(number_of_files + 7) + (number_of_files * 2),
							"<DIR>");
					//PA_16cText(0, 25, (strlen(filename) > 36 ? 2 : 6)+ number_of_files + ((number_of_files + 1) * 20 )+30, 255, 255, filename, 10, 1, 1000);
				} else {
					if (fileSize <= 1023) {
						sprintf(file_tsize, "%dKB", fileSize);
						PA_OutputSimpleText(0, 3,
								(number_of_files + 7) + (number_of_files * 2),
								filename);
						PA_OutputSimpleText(0, 25,
								(number_of_files + 7) + (number_of_files * 2),
								file_tsize);
					} else if (fileSize >= 1024) {
						fileSize = fileSize / 1024;
						sprintf(file_tsize, "%dMB", fileSize);
						PA_OutputSimpleText(0, 3,
								(number_of_files + 7) + (number_of_files * 2),
								filename);
						PA_OutputSimpleText(0, 25,
								(number_of_files + 7) + (number_of_files * 2),
								file_tsize);
					}
				}
			}
			number_of_files++; //���� �� ����
		}
	}
}

// ������ �ҷ��ͼ� ��θ� ����, ���Ʒ��� ��Ŀ���� ������ �� ���� ���ΰ�ħ
void updatefiles() {
	int i;
	char *path = " ";

	//ó�� 5�� �ѷ��ٶ�
	if (files < 6) {
		position = 0;
		for (i = 0; i < files + 1; i++) {
			sprintf(path, "%s%s", cur_dir, list[i].label); // ��� ����.
			rv = stat(path, &st); // path�� st�� ����, �����Ұ�� 0�� ����������쿡�� -1
			createitem(list[i].label, list[i].ext);
		}
	}
	//5�� ���ϸ���Ʈ �ѷ��ְ� �������� �Ѿ��
	else {
		for (i = position; i <= (position + 6); i++) {
			sprintf(path, "%s%s", cur_dir, list[i].label); // ��� ����.
			rv = stat(path, &st); // path�� st�� ����, �����Ұ�� 0�� ����������쿡�� -1
			createitem(list[i].label, list[i].ext);
		}
	}
}

// �Ʒ������� ���� ������ �Ʒ������� ������
void godown() {
	if (sel_created == false) {
		PA_CreateSprite(0,0,(void*) selected_Sprite,OBJ_SIZE_8X8,1,0,10,55);
		sel_created = true;
		selected = 0;
	} else if (sel_created == true) {
		if (selected < number_of_files - 1) {
			if (selected < 4) {
				selected++;
			}
			PA_SetSpriteY(0, 0, 55+(selected)*21+6);
		} else if (selected == number_of_files - 1) {
			if (position < files - 5) {
				position++;
				clearitems(true);
				updatefiles();
			}
		}
	}
}

// �������� ���� ������ �������� ������
void goup() {
	if (selected > 0) {
		PA_CreateSprite(0,0,(void*) selected_Sprite,OBJ_SIZE_8X8,1,0,10,55);
		selected--;
	}
	PA_SetSpriteY(0, 0, 55+(selected)*21+6);
	if ((position > 0) & (selected == 0)) {
		position--;
		clearitems(true);
		updatefiles();
	}
}

void clearitems(bool pad) {
	number_of_files = 0;
	PA_ClearTextBg(0);
	if (pad == false) {
		if (sel_created == true) {
			PA_DeleteSprite(0, 0);
			sel_created = false;
			selected = 0;
		}
	}
}

void clearlist() {
	int i;
	for (i = 0; i < 256; i++) {
		strcpy(list[i].label, " ");
		list[i].ext = 10;
		list[i].switched = false;
	}
	position = 0;
	files = 0;
	folders = 0;
}

void gotolastdir() {
	int i;
	i = (strlen(cur_dir) - 1);
	cur_dir[i] = ' ';
	i--;
	while (cur_dir[i] != '/') {
		cur_dir[i] = '\0';
		i--;
	}
}

int checkDownloadDevice() {
	if(Pad.Newpress.L)
	{
		if(device_selector!=0)
		{
			device_selector--;
			redrawSelectedDevice(device_selector);
		}

	}
	else if(Pad.Newpress.R)
	{
		if(device_selector!=2)
		{
			device_selector++;
			redrawSelectedDevice(device_selector);
		}
	}
	return device_selector;
}
void checkitems() {
	//s8 i;
	//char* Buffer;
	if (Pad.Newpress.Down) {
		while (Pad.Held.Down) {
			godown();
			PA_WaitForVBL();
			PA_WaitForVBL();
			PA_WaitForVBL();
			PA_WaitForVBL();
			PA_WaitForVBL();
			PA_WaitForVBL();
		}
	} else if ((Pad.Newpress.Up) & (sel_created == true)) {
		while (Pad.Held.Up) {
			goup();
			PA_WaitForVBL();
			PA_WaitForVBL();
			PA_WaitForVBL();
			PA_WaitForVBL();
			PA_WaitForVBL();
			PA_WaitForVBL();
		}
	}
	if (Pad.Newpress.A)
	{
		PA_SetTextTileCol(1,9);
//		PA_OutputText(1,2,2,"%d",selected+position);
//		PA_OutputText(1,2,10,"%d",list[selected+position].ext);
//		PA_OutputText(1,10,2,"%d,%d",strlen(list[selected+position].label),strlen(".."));
		if (list[selected + position].ext == 0) // ���� �������� ��,
		{
//			PA_OutputText(1,2,3,"%d",selected+position);
//			PA_OutputSimpleText(1,2,4,list[selected+position].label);

			if ((selected + position == 0) & (strcmp(list[selected + position].label,"..") == 0))
			{
//				PA_OutputSimpleText(1,2,6,list[selected+position].label);
				gotolastdir();
			}
			// 2. �Ϲ� ���丮�̸�, cur_dir�� ���� ���丮���� ������ �� �ּҷ� ��ü ��, ������Ʈ
			else
			{
				//   path :  /CREATEEX/
				sprintf(cur_dir, "%s%s/", cur_dir,
						list[selected + position].label);
//				PA_OutputSimpleText(1, 5, 5, cur_dir);
				clearlist();
				clearitems(false);
				fileListing(true);
				updatefiles();
			}
			clearlist();
			clearitems(false);
			fileListing(true);
			updatefiles();
		}
		// 3. �Ϲ� ������ ���, Ȯ���ڿ� ���� �з�
		else
		{
			char ext[3];
			int i;
			int isize = 0;
			char* Buffer;
			char temp[254];

			i = strlen(list[selected + position].label);

			// Ȯ���ڸ� �迭�� ����
			ext[2] = list[selected + position].label[i - 1];
			ext[1] = list[selected + position].label[i - 2];
			ext[0] = list[selected + position].label[i - 3];
			ext[3] = '\0';
			lowercase(ext);

			sprintf(temp, "%s%s", cur_dir, list[selected + position].label);

			// ������ Ȯ���ں� ����Ǵ� ���....
			if (strcmp(ext, "txt") == 0) {
				PA_InitText(0, 0);

				FILE* testRead = fopen(temp, "rb");
				fseek(testRead, 0, SEEK_END);
				isize = ftell(testRead);
				if (testRead == NULL)
//					PA_OutputText(1, 0, 2, "%c9Error");
				Buffer = (char*) malloc(isize + 1);

				fseek(testRead, 0, SEEK_SET);
				fread(Buffer, isize, 1, testRead);
				Buffer[isize] = '\0';
				fclose(testRead);

				PA_OutputText(0, 2, 3, "%c9%s",
						list[selected + position].label);
				PA_OutputText(0, 2, 5, "%c9%s", Buffer);
				clearitems(false);
				free(Buffer);
			} else if (strcmp(ext, "gif") == 0) {

			} else if (strcmp(ext, "nds") == 0) {

			}
		}
	}

	if (Pad.Newpress.B)
	{
		if(strcmp(cur_dir,"/"))
		{
			gotolastdir();
			clearlist();
			clearitems(false);
			fileListing(true);
			updatefiles();
		}
	}
}

void checkFileMenu() {
	// ���ϸ޴��� ���� Touch�Է��� ä���Ͽ�,
	if (PA_SpriteTouched(3)) {
		addFolder();
	}
	if (PA_SpriteTouched(2)) {
		moveDirectoryAndFiles();
	}
	if (PA_SpriteTouched(1)) {
		deleteFolderAndFiles();
	}
	if(Pad.Held.L && Pad.Held.Select) {
		initFileAndDirectory();
	}
}
/**
 * ���͸� �߰� �Լ�
 */
void addFolder() {
	s64 nletter = 0;
	char letter = 0;
	char text[256];
	bool overtext_pressed = false;
	bool sametext_pressed = false;
	bool enter_pressed = false;
	bool cancel = false;
	bool hasname = false;

	loadDialogBackground(ADD);
	PA_InitKeyboard(2); // �Է�Ű���� �ʱ�ȭ
	PA_SetBgPrio(0, 2, 0);
	PA_KeyboardIn(20, 95);

	memset(text,0,256);
	strcpy(text," ");
	PA_LoadDefaultText(0, 0);
	PA_SetTextTileCol(0, 1);
	PA_OutputSimpleText(0,5,8,"Name : ");
	PA_SetTextTileCol(0, 4);
	while (enter_pressed == false) {
		if (Pad.Newpress.B)
		{
			cancel = true;
		}
		letter = PA_CheckKeyboard();
		if (letter > 31)
		{
			text[nletter] = letter;
			nletter++;
		}
		else if (letter == PA_TAB)
		{
			u8 i;
			for (i = 0; i < 4; i++)
			{
				text[nletter] = ' ';
				nletter++;
			}
		}
		else if ((letter == PA_BACKSPACE) && nletter)
		{
			nletter--;
			text[nletter] = ' ';
		}
		// ���͸� ������,
		else if (letter == '\n')
		{
			int text_size=0;
			text_size = strlen(text);
			if (text_size > 0)
			{
				hasname = true;
				if(nletter > 13)
				{
					PA_KeyboardOut(); //
					PA_DeleteBg(0,1);
					loadDialogBackground(ALERT); //

					u32 nletters = 0; // Number of letter to output...
					u32 letter = 0; // Current letter written...

					while (letter == nletters){ // Do this until the function outputs all the text
						++nletters; // Next frame, one more letter...
						PA_SetTextTileCol(0,1);
						letter = PA_BoxText(0, 5, 10, 30, 40,
								"You can not enter \n more than 14 characters.", nletters);
						PA_WaitForVBL(); // You can set more WaitForVBL if you want to slow down the text output...
					}
					sleepTimer(2);
					PA_DeleteBg(0,0);
					PA_DeleteBg(0,1);

					PA_LoadDefaultText(0,0); //
					PA_SetTextTileCol(0,1);
					PA_OutputSimpleText(0,5,8,"Name : ");
					PA_SetTextTileCol(0, 4);
					loadDialogBackground(ADD);
					PA_KeyboardIn(20, 95); //
					overtext_pressed = true;
					if(overtext_pressed == true)
					{
						enter_pressed = false;
					}
				}
				else
				{
					int32_t i;
					char temp1[256], temp2[256];
					memset(temp1,0,256);
					memset(temp2,0,256);

					// ������ ���ڿ��� �ߺ��Ǵ����� Ȯ��
					if ((files <= 6) & (hasname))
					{
						position = 0;
						for (i = 0; i < files + 1; i++) {
							if (list[i].ext == 0) {
								//								sprintf(temp1, "%s", list[i].label);
								//								sprintf(temp2, "%s", text);

								if (stricmp(list[i].label, text) == 0)
								{
									sametext_pressed = true;
								}
							}
						}
						if (sametext_pressed == true)
						{
							PA_KeyboardOut(); //
							PA_DeleteBg(0,1);
							loadDialogBackground(ALERT); //

							u32 nletters = 0; // Number of letter to output...
							u32 letter = 0; // Current letter written...

							while (letter == nletters){ // Do this until the function outputs all the text
								++nletters; // Next frame, one more letter...
								PA_SetTextTileCol(0,1);
								letter = PA_BoxText(0, 5, 10, 30, 40,
										"You can not enter the\n same name.", nletters);
								PA_WaitForVBL(); // You can set more WaitForVBL if you want to slow down the text output...
							}
							sleepTimer(2);
							PA_DeleteBg(0,0);
							PA_DeleteBg(0,1);

							PA_LoadDefaultText(0,0); //
							PA_SetTextTileCol(0,1);
							PA_OutputSimpleText(0,5,8,"Name : ");
							PA_SetTextTileCol(0, 4);
							loadDialogBackground(ADD);
							PA_KeyboardIn(20, 95); //
							sametext_pressed=false;
						}
						else
						{
							enter_pressed = true;
						}
					}
					else if ((files > 6) & (hasname))
					{
						for (i = position; i <= (position + 6); i++)
						{
							if (list[i].ext == 0)
							{
								//								sprintf(temp1, "%s", list[i].label);
								//								sprintf(temp2, "%s", text);
								if (stricmp(list[i].label, text) == 0)
								{
									sametext_pressed = true;
								}
							}
						}
						if (sametext_pressed == true)
						{
							PA_KeyboardOut(); //
							PA_DeleteBg(0,1);
							loadDialogBackground(ALERT); //

							u32 nletters = 0; // Number of letter to output...
							u32 letter = 0; // Current letter written...

							while (letter == nletters){ // Do this until the function outputs all the text
								++nletters; // Next frame, one more letter...
								PA_SetTextTileCol(0,1);
								letter = PA_BoxText(0, 5, 10, 30, 40,
										"You can not enter the\n same name.", nletters);
								PA_WaitForVBL(); // You can set more WaitForVBL if you want to slow down the text output...
							}
							sleepTimer(2);
							PA_DeleteBg(0,0);
							PA_DeleteBg(0,1);

							PA_LoadDefaultText(0,0); //
							PA_SetTextTileCol(0,1);
							PA_OutputSimpleText(0,5,8,"Name : ");
							PA_SetTextTileCol(0, 4);
							loadDialogBackground(ADD);
							PA_KeyboardIn(20, 95); //
							sametext_pressed=false;
						}
						else
						{
							enter_pressed = true;
						}
					}
				}
			}
		}
		PA_WaitForVBL();
		PA_OutputSimpleText(0,12,8,text);
		if ((cancel == false) & ((enter_pressed == true))) {
			char fullpath[1024];
			sprintf(fullpath, "fat1:%s%s", cur_dir, text);
			if (mkdir(fullpath, 777) != 0) {
				//PA_WaitFor(Pad.Newpress.Anykey);
			} else {
				//PA_WaitFor(Pad.Newpress.Anykey);
			}
		} else if (cancel == true) {
			enter_pressed = true;
		}
	}
	PA_KeyboardOut();
	PA_DeleteBg(0,0);
	PA_DeleteBg(0,1);
	PA_LoadDefaultText(0, 2);
	clearitems(false);
	clearlist();
	fileListing(true);
	updatefiles();
}


// ���� ���丮�� �ö󰡱�
int deleteDirTree(const char *aDirPath) {
	int success = 1;
	int complete = 0;
	int32_t depth = 0;
	char next_dir_name[256];
	char orig_working_directory[MAXPATHLEN];

	getcwd( orig_working_directory, MAXPATHLEN);

	if ( chdir( aDirPath) != 0) {
		/* failed to get into the directory, cannot continue */
		return 0;
	}


	while ( !complete) {
		/* attempt to delete everything in the current directory */
		struct dirent *del_pent;
		DIR* dp = opendir( ".");

		if ( dp != NULL)
		{
			struct stat file_st;
			//			char name_buffer[256];

			while (!complete)
			{
				if ((del_pent = readdir(dp)) != NULL)
				{
					stat(del_pent->d_name, &file_st);
					/*
					 * avoid trying to delete "." and ".."
					 */
					if ( strcmp( del_pent->d_name, "..") != 0 && strcmp( del_pent->d_name, ".") != 0)
					{
						// unlink : �����̳� ���丮�� ����, ������ 0���� ���н� -1�� errno
						if ( unlink(del_pent->d_name) != 0)
						{
							if ( file_st.st_mode & S_IFDIR) {
								// ���丮�� ������� �ʾ�, unlink ������ �߻�
								if ( errno == EPERM) {
									/* the directory is not empty */
								}
								else {
									/* the delete failed for some other reason, oh dear */
									success = 0;
									complete = 1;
								}
							}
							else {
								/* failed to unlink a file, not alot can happen now */
								success = 0;
								complete = 1;
							}
						}
					}
				}
				else {
					/* completed the directory walk */
					break;
				}
			}
			closedir(dp);

			if ( !complete)
			{
				/* successfully deleted the contents of this directory
				 * so unlink it */
				chdir("..");

				if ( depth == 0)
				{
					if ( unlink(aDirPath) != 0)
					{
						/* should have worked */
						success = 0;
					}
					complete = 1;
				}
				else
				{
					depth -= 1;
				}

			}
		}
		else {
			/* time to cry */
			success = 0;
			complete = 1;
		}
	}

	chdir( orig_working_directory);

	return success;
}


void deleteFolderAndFiles() {
	PA_LoadDefaultText(0,0);
	loadDialogBackground(DELETE);
	//���� ����
	if ((list[selected+position].ext >= 1) & (list[selected+position].ext <= 5))
	{
		u32 nletters = 0; // Number of letter to output...
		u32 letter = 0; // Current letter written...

		while (letter == nletters){ // Do this until the function outputs all the text
			++nletters; // Next frame, one more letter...
			PA_SetTextTileCol(0,4);
			letter = PA_BoxText(0, 5, 10, 30, 40, "Are you sure you want \n to delete? (A=Yes;others=no)", nletters);
			PA_SetTextTileCol(0, TEXT_BLACK);
			PA_WaitForVBL(); // You can set more WaitForVBL if you want to slow down the text output...
		}
		PA_WaitFor(Pad.Newpress.Anykey);

		if(Pad.Newpress.A)
		{
			PA_LoadDefaultText(0,2);
			char temp[2000]=" ";
			sprintf(temp, "%s%s", cur_dir, list[selected+position].label);
			remove(temp); // ���� ���� �Լ�
			free(temp); // �޸� ����
//			PA_OutputText(1,2,8,"Cur_dir : %s",cur_dir);
			clearlist();
			clearitems(false);
			fileListing(true);
			updatefiles();
			PA_DeleteBg(0,0);
			PA_DeleteBg(0,1);
		}
	}
	// ���� ����
	else
	{
		u32 nletters = 0; // Number of letter to output...
		u32 letter = 0; // Current letter written...

		while (letter == nletters){ // Do this until the function outputs all the text
			++nletters; // Next frame, one more letter...
			PA_SetTextTileCol(0,4);
			letter = PA_BoxText(0, 5, 10, 30, 40, "Are you sure you want \n to delete Directory & \n All Files? \n (A=Yes;others=no)", nletters);
			PA_SetTextTileCol(0, TEXT_BLACK);
			PA_WaitForVBL(); // You can set more WaitForVBL if you want to slow down the text output...
		}
		PA_WaitFor(Pad.Newpress.Anykey);

		if(Pad.Held.A)
		{
			char *fullpath=" ";
			sprintf(fullpath, "%s%s", cur_dir, list[selected+position].label);
			if (deleteDirTree(fullpath) == 1)
			{
//				PA_OutputSimpleText(1,0,12, "Folder and subfolders deleted!");
			}
			else
			{
				// ���� ���� ���� ���� ó��
//				PA_OutputSimpleText(1,0,12, "An error ocurred, sorry!");
			}
			PA_WaitFor(Pad.Newpress.Anykey);
			clearitems(false);
			clearlist();
			fileListing(true);
			updatefiles();
			PA_DeleteBg(0,0);
			PA_DeleteBg(0,1);
		}
	}
//	PA_OutputText(1,2,8,"Cur_dir : %s",cur_dir);
	PA_LoadDefaultText(0,2);
	clearlist();
	clearitems(false);
	fileListing(true);
	updatefiles();
	PA_DeleteBg(0,0);
	PA_DeleteBg(0,1);
}

/**
 * ���丮 �� ���� �̵� �Լ�
 */
void moveDirectoryAndFiles() {
	s64 nletter = 0;
	char letter = 0;
	char text[256];
	bool enter_pressed = false;
	bool cancel = false;
	bool hasname = false;

	loadDialogBackground(MOVE);
	PA_InitKeyboard(2); // �Է�Ű���� �ʱ�ȭ
	PA_SetBgPrio(0, 2, 0);
	PA_KeyboardIn(20, 95);

	memset(text,0,256);
	strcpy(text," ");
	PA_LoadDefaultText(0, 0);
	PA_SetTextTileCol(0, 1);
	PA_OutputSimpleText(0,5,8,"Name : ");
	PA_SetTextTileCol(0, 4);
	while (enter_pressed == false)
	{
		if (Pad.Newpress.B) {
			cancel = true;
		}
		letter = PA_CheckKeyboard();
		if (letter > 31) {
			text[nletter] = letter;
			nletter++;
		}
		else if (letter == PA_TAB)
		{
			u8 i;
			for (i = 0; i < 4; i++)
			{
				text[nletter] = ' ';
				nletter++;
			}
		}
		else if ((letter == PA_BACKSPACE) && nletter)
		{
			nletter--;
			text[nletter] = ' ';
		}
		else if (letter == '\n')
		{
			int text_size = strlen(text);
			if (strlen(text) > 0) {
				hasname = true;
			}
			char oldpath[1024];
			char newpath[1024];
			sprintf(oldpath, "%s%s", cur_dir, list[selected+position].label);
			sprintf(newpath, "%s%s", text, list[selected+position].label);
			if(((text[0]!='/') ||(text[text_size-1]!='/')))
			{
				PA_KeyboardOut(); //
				PA_DeleteBg(0,1);
				loadDialogBackground(ALERT); //

				u32 nletters = 0; // Number of letter to output...
				u32 letter = 0; // Current letter written...

				while (letter == nletters){ // Do this until the function outputs all the text
					++nletters; // Next frame, one more letter...
					PA_SetTextTileCol(0,1);
					letter = PA_BoxText(0, 5, 10, 30, 40,
							"Address format is /nincorrect, or the \ndirectory or file \ncan not be found.", nletters);
					PA_WaitForVBL(); // You can set more WaitForVBL if you want to slow down the text output...
				}
				sleepTimer(2);
				PA_DeleteBg(0,0);
				PA_DeleteBg(0,1);

				PA_LoadDefaultText(0,0); //
				PA_SetTextTileCol(0,1);
				PA_OutputSimpleText(0,5,8,"Name : ");
				PA_SetTextTileCol(0, 4);
				loadDialogBackground(ADD);
				PA_KeyboardIn(20, 95); //
				enter_pressed = false;
			}
			else
			{
				if(rename(oldpath,newpath)==-1)
				{
					PA_KeyboardOut(); //
					PA_DeleteBg(0,1);
					loadDialogBackground(ALERT); //

					u32 nletters = 0; // Number of letter to output...
					u32 letter = 0; // Current letter written...

					while (letter == nletters){ // Do this until the function outputs all the text
						++nletters; // Next frame, one more letter...
						PA_SetTextTileCol(0,1);
						letter = PA_BoxText(0, 5, 10, 30, 40,
								"Path is incorrect\n, or the directory\n or file can\n not be found.", nletters);
						PA_WaitForVBL(); // You can set more WaitForVBL if you want to slow down the text output...
					}
					sleepTimer(2);
					PA_DeleteBg(0,0);
					PA_DeleteBg(0,1);

					PA_LoadDefaultText(0,0); //
					PA_SetTextTileCol(0,1);
					PA_OutputSimpleText(0,5,8,"Name : ");
					PA_SetTextTileCol(0, 4);
					loadDialogBackground(ADD);
					PA_KeyboardIn(20, 95); //
					enter_pressed = false;
				}
				else
				{
					enter_pressed = true;
				}
			}
		}
		PA_WaitForVBL();
		PA_OutputSimpleText(0,12,8,text);
		if ((cancel == false) & ((enter_pressed == true))) {
			// ������
		}
		else if (cancel == true)
		{
			enter_pressed = true;
		}
	}
	PA_KeyboardOut();
	PA_DeleteBg(0,0);
	PA_DeleteBg(0,1);
	PA_LoadDefaultText(0, 2);
	clearitems(false);
	clearlist();
	fileListing(true);
	updatefiles();
}

/**
 * ���ϰ� ���丮 ������ ó�� ���·� �ʱ�ȭ �ϴ� �Լ�
 * @param string[]
 */
void initFileAndDirectory() {
	PA_LoadDefaultText(0,0);
	loadDialogBackground(DELETE);
	u32 nletters = 0; // Number of letter to output...
	u32 letter = 0; // Current letter written...

	while (letter == nletters){ // Do this until the function outputs all the text
		++nletters; // Next frame, one more letter...
		PA_SetTextTileCol(0,4);
		letter = PA_BoxText(0, 5, 10, 30, 40, "Warning! Are you sure\n want to intialization?\n (A=Yes;others=no)", nletters);
		PA_SetTextTileCol(0, TEXT_BLACK);
		PA_WaitForVBL(); // You can set more WaitForVBL if you want to slow down the text output...
	}
	PA_WaitFor(Pad.Newpress.Anykey);

	if(Pad.Newpress.A)
	{
		int i;
		char temp[1024];
		strcpy(temp,cur_dir);
		PA_LoadDefaultText(0,2);
		for (i = 0; i < number_of_files; i++) {
			if(!stricmp(list[i].label,"_SYSTEM_")||
					!stricmp(list[i].label,"MOONSHL")||
					!stricmp(list[i].label,"WIFIDOWN.NDS")||
					!stricmp(list[i].label,"_DS_MENU.SYS")||
					!stricmp(list[i].label,"_DS_MENU.DAT")||
					!stricmp(list[i].label,"DallShell4DS_110724.NDS")||
					!stricmp(list[i].label,"DallShell4DS_110724.SAV")||
					!stricmp(list[i].label,"DallShell_Conf.TXT")||
					!stricmp(list[i].label,"DallShell_Conf.NDS"))
			{
				;
			}
			else
			{
				if((list[i].ext >= 1) & (list[i].ext <= 5))
				{
					//strcat(temp,list[i].label);
					sprintf(temp, "%s%s", cur_dir, list[i].label);
					remove(temp);
				}
				else if(list[i].ext == 0)
				{
					//strcat(temp,list[i].label);
					sprintf(temp, "%s%s", cur_dir, list[i].label);
					deleteDirTree(temp);
				}
			}
		}
		PA_LoadDefaultText(0,0);
		u32 nletters = 0; // Number of letter to output...
		u32 letter = 0; // Current letter written...

		while (letter == nletters){ // Do this until the function outputs all the text
			++nletters; // Next frame, one more letter...
			PA_SetTextTileCol(0,4);
			letter = PA_BoxText(0, 5, 10, 30, 40, "Please reboot your DS.\nThe power is turned off.", nletters);
			PA_SetTextTileCol(0, TEXT_BLACK);
			PA_WaitForVBL(); // You can set more WaitForVBL if you want to slow down the text output...
		}
		sleepTimer(4);
		PA_PowerOff();
	}
	else
	{
		PA_LoadDefaultText(0, 2);
		fileInit();
		clearlist();
		clearitems(false);
		fileListing(true);
		updatefiles();
		PA_DeleteBg(0,0);
		PA_DeleteBg(0,1);
	}
}

void sleepTimer(int sec)
{
	int i=0;
	for(i=0;i<sec*100;i++)
	{
		PA_WaitForVBL();
	}
}

