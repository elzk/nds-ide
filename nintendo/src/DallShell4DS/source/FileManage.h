/*
 * FileManage.h
 *
 *  Created on: 2011. 7. 24.
 *      Author: Jaewon Lee
 */

#ifndef FILEMANAGE_H_
#define FILEMANAGE_H_

#include <nds/ndstypes.h>
#include <sys/dir.h>

struct dirent *pent; // dirent ����ü ���� ����
struct stat st; // stat ����ü ���� ����


typedef struct {
	char label[256]; // ���� ��
	s8 ext; //0=folder 1=unknown 2=music 3=image 4=text 5=game
	bool switched;
} items;

items list[256];// ���ϸ���Ʈ ����
int position, files; // ���� ��ġ, ���� ��
int number_of_files; // ������ ����
char *file_tsize;
char cur_dir[1024]; // ���� ���丮
s8 alive; // ������ ǥ�� ���� ����
int folders; // ���� ���� ����
bool sel_created; // ���õ� ���� ǥ��
s8 selected; // ���� ��ġ �Լ�
DIR* pdir;
//DIR_ITER* dir;
int textheight;
int mode; // File Mode
int re;
int rv;


/******** File Manage Function *******/
void fileInit();
void fileListing(bool change);
void clearlist();
void createitem(char *filename, s8 ext);
void updatefiles ();
void getextension(int i);
void godown();
void goup();
void checkitems();
int checkDownloadDevice();
void clearitems(bool pad);
void gotolastdir();
void checkFileMenu();
void addFolder();
void deleteFolderAndFiles();
int deleteDirTree( const char *aDirPath) ;
void initFileAndDirectory();
void moveDirectoryAndFiles();

#endif /* FILEMANAGE_H_ */


