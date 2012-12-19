/******************** include�� ********************/
#include <PA9.h>        // PAlibrary�� �������
#include "all_gfx.h"    // PAGfx�� �̿��� ������ �̹���,��������Ʈ�� ����ü�� ����ִ�.
#include "all_gfx.c"    // PAGfx�� �̿��� ������ �̹���,��������Ʈ�� ����ü�� ����ִ�.
#include "Startup.h"	// raw�������� ; data������ <�������ϸ�>.raw�� �ΰ�, <�������ϸ�>.raw
#include "trapcard.h"   // raw��������

/******************** ��ũ�� ����� ********************/
/* ��ũ�� �ε��� ��ũ�� */
#define UP_SCREEN 1
#define DOWN_SCREEN 0

/* ���׸�  '��' �ε��� ��ũ�� */
#define BG0   0
#define BG1   1
#define BG2   2
#define BG3   3

/* ��������Ʈ �ȷ�Ʈ �ε��� ��ũ�� */
#define NPAL  0 // �ѹ� ��������Ʈ �ȷ�Ʈ
#define IPAL  1 // ������ ��������Ʈ �ȷ�Ʈ

/* scr_Season�� ��ȯ�� */
#define GO_SHOP 0 // ����
#define GO_NEXT 1 // ������������

/* scr_Game�� ��ȯ�� */
#define GAMEOVER 0 // ����
#define GAMEPLAY 1 // ���

/* �б� ���� ��ũ��*/
#define SEASON_MAX 8 // ���б� ��
#define SEASON_1ST 1
#define SEASON_2ND 2
#define SEASON_3RD 3
#define SEASON_4TH 4
#define SEASON_5TH 5
#define SEASON_6TH 6
#define SEASON_7TH 7
#define SEASON_8TH 8

#define SIZE      6    /* ��� �迭�� ��,�� ������ */
#define NUM_OF_BLOCK 4 /* ��� ���� �ְ� ��( 0~4 ) */

/* ��� ���� ��ũ�� */
#define BLK_CASH  0 // �����
#define BLK_HWANG 1
#define BLK_CHANG 2
#define BLK_MIN   3
#define BLK_DONG  4

/* boolean�� ��ũ�� */
#define BOOL        int
#define TRUE		1
#define	FALSE		0

/* ������ �˰��� ��� ��ũ�� */
/* destroy�迭���� ��� */
#define KILL        -1		// ������ĺ�
#define MONEYPLUS   -2		// ������ĺ�
/* blocks�迭���� ��� */
#define TARGET		1000	// ���� ��� ǥ��

/* ������ ���� */
#define ITEM_BOMB_ROW	1	// ���� �� �� ����
#define ITEM_BOMB_COL	2	// ���� �� �� ����
#define ITEM_BOMB_SMALL	3	// ���� ��� ���� 9ĭ ����
#define ITEM_BOMB_BIG	4	// ���� ��� ���� 25ĭ ����
#define ITEM_TIME		5	// �ð� ����
#define ITEM_RELOCATE	6	// ��� ��� ���ġ (���� �ݿ��� �ȵ�)
/* ������ ���� */
#define ITEM_PRICE_ROW      2000
#define ITEM_PRICE_COL      2000
#define ITEM_PRICE_SMALL    4000
#define ITEM_PRICE_BIG      6000
#define ITEM_PRICE_TIME     6000
#define ITEM_PRICE_RELOCATE 6000

/* ������  ��ũ�� */
#define USER_ITEM_MAX 5        // ����ڰ� ������ �� �ִ� �ִ� ������ ����
#define NUM_OF_ITEMTYPE 6      // ������ ������ ��.
#define ITEM_FIRST_SPRITE 36   // �����۽�������Ʈ�� ���� ��������Ʈ �ε���.


// �������� ����ü ����
struct userInfo {	// ������ ����Ǵ� ���� ���������� ����.
	int userMoney;  // ���� ��.
	int userScore;  // ���� ���ھ�.
	int userStage;	// ���� ��������.
	int userItem[USER_ITEM_MAX]; // ���� ������ �迭.
	int activeBlockCol;	// Ȱ��ȭ�� ����� col�ε���
	int activeBlockRow; // Ȱ��ȭ�� ����� row�ε���
	int activeItem;		// Ȱ��ȭ�� �������� �ε���
};


/******************** �Լ� ����� ********************/
/* �⺻���� PAlibrary�� ȭ�� �ʱ�ȭ �Լ� */
void UnLoad_Screen();

/* �������� �ʱ�ȭ */
void user_init();

/* ���º� �Լ� */
void scr_Main();      // ����
int scr_Game();       // ����  _ return GAMEOVER or GAMEPLAY
int scr_Season();     // �б��� _ return GO_SHOP or GO_NEXT
void scr_Shop();      // ����
void scr_Gameover();  // ����

/* �����ġ�ν� */
int gettouchcol(int x); // ��ġ �Է��� �̷���� ���� ��� �� �ε����� ��ȯ.
int gettouchrow(int y); // ��ġ �Է��� �̷���� ���� ��� �� �ε����� ��ȯ.

/* ȭ�� ���� �Լ� */
void stage_init(int ustage); // ���������� ����.
void block_init();           // ����� �ʱ�ȭ.
void block_reload();         // ����� ����.
void item_init();            // ������ �ʱ�ȭ �� ����.

/* ��� �Լ� */
BOOL isNearBlock(int row, int col, int oldrow, int oldcol); // �� �࿭�� ���� �����¿� �� Ư����ġ�� ������ ������ TRUE�� ����, �ƴϸ� FALSE�� ����.
void swapBlock(int row, int col, int oldrow, int oldcol);   // �� �࿭�� ���� ����Ű��, �� ����� ���� �ٲ۴�.
void swapsprite(int row, int col, int oldrow, int oldcol,int oldsprite,int newsprite); // �� �࿭�� ���� ����Ű��, �� ��������Ʈ�� ��ġ�� �ٲ۴�.
BOOL blocksCrack(int combo); /* ���� ��ϱ׷� üũ */
int checkRow(int row);/* �� üũ */
int checkCol(int col);/* �� üũ */

/* ������ �Լ� */
int itemgettouchrow(int y);                        // ��ġ �Է��� �̷���� ���� ������ �ε����� ��ȯ.
void useOneClickItem(int item);                    // ��ǥ ������ �ʿ���� ������ ���.
BOOL useTwoClickItem(int item, int row, int col);  // ��ǥ ������ �ʿ��� ������ ���.
int getitemindex();                                // ����ִ� ������� ������ �迭 �ε����� ��ȯ.


/******************** �������� ����� ********************/
static int stageScore;   //�������������� �Ѿ �� �ִ� ���� ��
static int stageTime;	 //�������� �� �ð��� ���������� �ٸ�

struct userInfo uinfo;   // ����� ���� ��ü ����.

int blocks[SIZE][SIZE];				// ��� �迭
int destroy[SIZE][SIZE];			// �ı��� ��� ���� �迭
int Blksprite[SIZE][SIZE];          // ��� ��������Ʈ �迭
static int stageTime = 60;		    // ��������  ���ѽð�
static int restTime = 60;		    // ���� �ð�

BOOL newGame=1;                     // ���ο� ���ӿ���
BOOL isplaying = FALSE;             // ������� ���࿩��


//Main...
int main(void){
    /* PALibrary�ʱ�ȭ(����!PALibrary���� ����ϴ� �����鵵 �ʱ�ȭ�ȴ�.) */
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL

	// ���� ȯ�� �ʱ�ȭ
	AS_Init(AS_MODE_SURROUND | AS_MODE_16CH);
	AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND);

	// ���� ���.AS_SoundDefaultPlay((u8*)<�������ϸ�>, (u32)<�������ϸ�>_size, 127, 64, <���� ����>, 0);
	AS_SoundDefaultPlay((u8*)Startup, (u32)Startup_size, 127, 64, false, 0);

	/* ���α׷� ���� */
	while(1){ // Infinite loop


		user_init();                            // �������� �ʱ�ȭ

		scr_Main();			                    // ����
		while ( scr_Game() != GAMEOVER ) {		// ����
			while ( scr_Season() == GO_SHOP ) 	// �б���ȯ�б���
				scr_Shop();                     // ����
		}
		scr_Gameover();                         // ����

		PA_WaitForVBL();	                    // PALibraby �Լ� _ ȭ������� 60fps�� ����ȭ�Ѵ�.
	}

	UnLoad_Screen();
	return 0;
}

/* ���Ӹ��� */
void scr_Main() {
    /* ȭ���ʱ�ȭ  �� ���� */
	UnLoad_Screen();
	PA_Init();
	PA_InitVBL();
	PA_EasyBgLoad(UP_SCREEN, BG3, Title);			// ��� ���÷���-Ÿ��Ʋȭ�� ����.
	PA_EasyBgLoad(DOWN_SCREEN, BG3, Title_Down);	// �ϴ� ���÷���-Ÿ��Ʋȭ�� ����.

	while (1) {
		if ( Stylus.Newpress ) {
			return;                                 // ��ġ �Է��� ������ scr_Main�� ��������.
		}
		PA_WaitForVBL();
	}
}

/* ���� */
int scr_Game() {

	// ��� �ڵ鸵 ������...
	BOOL blockchanged = TRUE;
	int col = -1;
	int row = -1;
	int i, j;
	int combo = 1;
	int newNum = 0;
	int ScoreCount = 0;
	int moneyCount = 0;

	// ���� ������ �ε��� ����
	int select = -1;

	/* Ÿ�̸� ����_��. */
	int lastcount = 0;      // ���� ��.
	int currentcount = 0;   // ���� ��.

	/* busy-waiting ī��Ʈ ����*/
	int waitcount = 0;

	/* PALibrary �ʱ�ȭ �� ȭ�� �ʱ�ȭ,����*/
	UnLoad_Screen();
	PA_Init();
	PA_InitVBL();
    /* �ؽ�Ʈ ��� ȯ�� �ʱ�ȭ */
	PA_InitText(UP_SCREEN, BG2);
	PA_SetTextTileCol(UP_SCREEN, TEXT_BLACK);
    /* ���ϴ� ���÷��� ��� */
	PA_EasyBgLoad(UP_SCREEN, BG3, Title);		// Ÿ��Ʋ ���÷���
	PA_EasyBgLoad(DOWN_SCREEN, BG3, Play_Down);	// �÷��� ���÷���
	/* ��������Ʈ �ȷ�Ʈ ���� */
	PA_LoadSpritePal(DOWN_SCREEN, NPAL, (void*)number_Pal); // ��� ��������Ʈ �ȷ�Ʈ.
	PA_LoadSpritePal(DOWN_SCREEN, IPAL, (void*)item_Pal);   // ������ ��������Ʈ �ȷ�Ʈ.

	stage_init(uinfo.userStage); // �������� ����
	block_init();                // ��� �� ��� ��� �ʱ�ȭ
	item_init();                 // ������ �� ������ ��� ����

	/* ���� ���� ��� */
	PA_OutputText(UP_SCREEN, 10, 2, "Stage : %d", uinfo.userStage);
	PA_OutputText(UP_SCREEN, 10, 5, "Score : %d", uinfo.userScore);
	PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
	PA_OutputText(UP_SCREEN, 5, 15, "Mission Score  : %d", stageScore);

	/* ī���� �ʱ�ȭ �� ���� */
	PA_VBLCountersReset(); // ī���� �ʱ�ȭ.
	PA_VBLCounterStart(0); // ī���� ����.

	/* ������� �÷��� */
	if ( isplaying==FALSE ) {
		AS_SoundDefaultPlay((u8*)trapcard, (u32)trapcard_size, 127, 64, true, 0);
		isplaying = TRUE;
	}

	while (1) {
		/* ī���� ���� ���� ���� ���� �ð�(restTime)�� ����,���� */
		currentcount = (PA_VBLCounter[0]/60);
		if ( lastcount!=currentcount ) {
			restTime = restTime - ( currentcount-lastcount );
			lastcount = currentcount;
		}
		/* ����� ������ ��� */
		PA_ClearTextBg(UP_SCREEN);
		PA_OutputText(UP_SCREEN, 10, 2, "Stage : %d", uinfo.userStage);
		PA_OutputText(UP_SCREEN, 10, 5, "Score : %d", uinfo.userScore);
		PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
		PA_OutputText(UP_SCREEN, 5, 15, "Mission Score  : %d", stageScore);
		PA_OutputText(UP_SCREEN, 5, 17, "Mission Time  : %d", stageTime);
		PA_OutputText(UP_SCREEN, 5, 19, "Rest Time  : %d", restTime);

		/* �Է¿� ���� �ڵ鸵 */
		if( Stylus.Newpress ){
			/******************** PAUSE ********************/
			if ( (Stylus.X>=214&&Stylus.X<=241)&&(Stylus.Y>=164&&Stylus.Y<=172) ) {// ȭ��� pause ��ġ.
				/* PALibrary �ʱ�ȭ �� ȭ���ʱ�ȭ,���� */
				PA_Init();
				PA_InitVBL();
				UnLoad_Screen();
				PA_EasyBgLoad(UP_SCREEN, BG3, Title);
				PA_EasyBgLoad(DOWN_SCREEN, BG3, Pause_Down);	// Pauseȭ�� �ϴ� ȭ�� ���÷���

				/* ���õ� ��� ���� �ʱ�ȭ */
				uinfo.activeBlockRow = -1;
				uinfo.activeBlockCol = -1;
				col = -1;
				row = -1;
				select = -1;
				uinfo.activeItem = -1;

				while( 1 ){

					if ( Stylus.Newpress ) { // ��ġ  �� �̷������ PAUSE���¸� ���
						/* PALibrary �ʱ�ȭ �� ȭ���ʱ�ȭ,���� */
						UnLoad_Screen();
						PA_InitText(UP_SCREEN, BG2);
						PA_SetTextTileCol(UP_SCREEN, TEXT_BLACK);
						PA_EasyBgLoad(UP_SCREEN, BG3, Title);
						PA_EasyBgLoad(DOWN_SCREEN, BG3, Play_Down);	// Playȭ�� �ϴ� ȭ�� ���÷���
						/* ���� ���� ��� */
						PA_OutputText(UP_SCREEN, 10, 2, "Stage : %d", uinfo.userStage);
						PA_OutputText(UP_SCREEN, 10, 5, "Score : %d", uinfo.userScore);
						PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
						PA_OutputText(UP_SCREEN, 5, 15, "Mission Score  : %d", stageScore);

						/* ��� �� ������ ��� ���� */
						block_reload();
						PA_LoadSpritePal(DOWN_SCREEN, IPAL, (void*)item_Pal); //��������Ʈ �ȷ�Ʈ ����.
						item_init();

						/* ī���� �ʱ�ȭ */
						lastcount = 0;
						currentcount = 0;
						PA_VBLCountersReset();
						PA_VBLCounterStart(0); // ī���� ����.

						break;

					}

					PA_WaitForVBL();
				}

			/******************** EXIT ********************/
			} else if( (Stylus.X>=209&&Stylus.X<=240)&&(Stylus.Y>=175&&Stylus.Y<=183) ) {		//��������
				return GAMEOVER;

			/******************** BLOCK ********************/
			} else if ( (Stylus.X>=0&&Stylus.X<=192)&&(Stylus.Y>=0&&Stylus.Y<=192) ) {		//���ó��
				col = gettouchcol(Stylus.X);
				row = gettouchrow(Stylus.Y);

				/* �������� */
				if( (row==uinfo.activeBlockRow)&&(col==uinfo.activeBlockCol) ){

					uinfo.activeBlockRow = -1;
					uinfo.activeBlockCol = -1;
					PA_SetSpriteAnim( DOWN_SCREEN, Blksprite[row][col], (PA_GetSpriteAnimFrame(DOWN_SCREEN,Blksprite[row][col])-(NUM_OF_BLOCK+1)) ); // ��������Ʈ ����.
				}

				/* ��ġ���� */
				else if( ((uinfo.activeBlockRow != -1) && (uinfo.activeBlockCol != -1)) &&
						((row != uinfo.activeBlockRow) || (col != uinfo.activeBlockCol)) )
				{
					if ( isNearBlock(row, col, uinfo.activeBlockRow, uinfo.activeBlockCol) ) {
						PA_SetSpriteAnim(DOWN_SCREEN, Blksprite[uinfo.activeBlockRow][uinfo.activeBlockCol], PA_GetSpriteAnimFrame(DOWN_SCREEN,Blksprite[uinfo.activeBlockRow][uinfo.activeBlockCol])-5);
						swapBlock(row, col, uinfo.activeBlockRow, uinfo.activeBlockCol);		// ��� ��������Ʈ ��ġ ��ȯ.

						blockchanged = TRUE;
						uinfo.activeBlockRow = -1;
						uinfo.activeBlockCol = -1;
					} else {
						; // �ʿ�� ������ �������.
					}
				}

				/* ������ ������ ����� �������� ó��. */
				else if (select != -1) {
					blockchanged = useTwoClickItem(uinfo.userItem[uinfo.activeItem],row,col);			// Ȱ��ȭ�� ������ ���
					PA_SetSpriteAnim(DOWN_SCREEN, ITEM_FIRST_SPRITE+uinfo.activeItem, 0); // �����۶� ��� �׷��� ȿ��
					uinfo.userItem[uinfo.activeItem] = -1;						// �����۶��� ���
					uinfo.activeItem = -1;										// ��Ȱ��ȭ ó��
					uinfo.activeBlockCol = -1;									// ��� ��Ȱ��ȭ ó��
					uinfo.activeBlockRow = -1;
					select = -1;                                                // ���þ����� �ε��� �ʱ�ȭ
				}

				/* ��� ���� */
				else {
					uinfo.activeBlockRow = row;													//	��� Ȱ��ȭ (�׷��� ȿ���� �־�� ��)
					uinfo.activeBlockCol = col;
					PA_SetSpriteAnim( DOWN_SCREEN, Blksprite[row][col], (PA_GetSpriteAnimFrame(DOWN_SCREEN,Blksprite[row][col])+(NUM_OF_BLOCK+1)) );
				}

			/******************** ITEM ********************/
			} else if ( (Stylus.X>=224&&Stylus.X<=256)&&(Stylus.Y>=0&&Stylus.Y<=160) ) {
				/* ����� ���õǾ� ������, �ʱ�ȭ */
				if ( uinfo.activeBlockRow!=-1 ) {
					PA_SetSpriteAnim(DOWN_SCREEN, Blksprite[uinfo.activeBlockRow][uinfo.activeBlockCol], PA_GetSpriteAnimFrame(DOWN_SCREEN,Blksprite[uinfo.activeBlockRow][uinfo.activeBlockCol])-5);
					uinfo.activeBlockRow = -1;
					uinfo.activeBlockCol = -1;
				}
				/* ���� ������ �ε��� ���� */
				select = itemgettouchrow(Stylus.Y);

				if (uinfo.userItem[select] != -1) { // ��ġ�� �����۶��� �������� ������
					/* ������� */
					if (uinfo.activeItem == select)	{
						PA_SetSpriteAnim( DOWN_SCREEN, ITEM_FIRST_SPRITE+uinfo.activeItem, (PA_GetSpriteAnimFrame(DOWN_SCREEN,ITEM_FIRST_SPRITE+uinfo.activeItem)-NUM_OF_ITEMTYPE) ); // ��Ȱ��ȭ �׷��� ȿ��.
						uinfo.activeItem = -1;
					}
					/* ȿ�� ��� �ߵ� ������ ���� */
					else if ((uinfo.userItem[select] >= 5) && (uinfo.userItem[select] <= 6)) {	// ������ �������� ��� ���Ǵ� ���̸�!!��ĥ �� �����ϰ� �ٲ�� ��
						useOneClickItem(uinfo.userItem[select]);	// ������ ���
						PA_SetSpriteAnim(DOWN_SCREEN, ITEM_FIRST_SPRITE+select, 0); // �����۶��� ��� �׷��� ȿ��
						uinfo.userItem[select] = -1;				// �����۶��� ��� (�׷��� ȿ�� �ʿ�)
						uinfo.activeItem = -1;						// ��Ȱ��ȭ ó�� (�׷��� ȿ�� �ʿ�)
						uinfo.activeBlockCol = -1;					// ��� ��Ȱ��ȭ ó�� (�׷��� ȿ�� �ʿ�?)
						uinfo.activeBlockRow = -1;
						select = -1;
					/* ������� �ʿ� ������ ����*/
					} else	{
						if ( uinfo.activeItem!=-1 ) { // ������ ���õ� �������� ������ ��Ȱ��ȭ.
							PA_SetSpriteAnim(DOWN_SCREEN, ITEM_FIRST_SPRITE+uinfo.activeItem, PA_GetSpriteAnimFrame(DOWN_SCREEN,ITEM_FIRST_SPRITE+uinfo.activeItem)-NUM_OF_ITEMTYPE); // ��Ȱ��ȭ �׷��� ȿ��
						}
						uinfo.activeItem = select; // ���� ���� ������ Ȱ��ȭ
						PA_SetSpriteAnim( DOWN_SCREEN, ITEM_FIRST_SPRITE+uinfo.activeItem, (uinfo.userItem[uinfo.activeItem]+NUM_OF_ITEMTYPE) ); // �׷��� ȿ��
					}
				}

			}
		} // ~ ��ġó�� ��.


		/* ��� ��ġ����� ��� ���� */
		if ( blockchanged==TRUE ){
			/* ����� ��� ���� */
			while ( blocksCrack(combo) == TRUE ) {

				for (i=0; i<SIZE; i++) {		// ���� ��� ǥ��
					for (j=0; j<SIZE; j++) {
						if (destroy[i][j] == KILL) {
							blocks[i][j] = TARGET;
							ScoreCount++;
						}
						if (destroy[i][j] == MONEYPLUS) {
							blocks[i][j] = TARGET;
							moneyCount++;
						}
					}
				}

				uinfo.userMoney += 100 * moneyCount * combo;	// �� ���
				PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney); // ����� �� ����.

				// ��ϻ��ȿ���� ��.
				for (i=0; i<SIZE; i++) {		// ���ο� ��� ����
					for (j=0; j<SIZE; j++) {
						if (blocks[i][j]  == TARGET) {
							PA_StartSpriteAnimEx(DOWN_SCREEN, Blksprite[i][j], 10, 14, 15, ANIM_LOOP, 1);
						}
					}
				}

				// ����� ���ھ� ����.
				PA_OutputText(UP_SCREEN, 10, 5, "Score : %d", uinfo.userScore);

				// �ִϸ��̼� ȿ���� ������ ��ٸ�
				while (1) {
					waitcount++;
					if (waitcount==25) {
						waitcount=0;
						break;
					}
					PA_WaitForVBL();
				}

				/* ���ο� ��� �� ������ �Բ� ��������Ʈ ����. */
				for (i=0; i<SIZE; i++) {		// ���ο� ��� �� ����
					for (j=0; j<SIZE; j++) {
						if (blocks[i][j]  == TARGET) {
							newNum = PA_RandMax(NUM_OF_BLOCK);
							blocks[i][j] = newNum;
							PA_SetSpriteAnim(DOWN_SCREEN, Blksprite[i][j], blocks[i][j]);
						}
					}
				}
				// �޺� �ߵ��� �ð������� Ȯ���� �� �ִ� �ð��� ��.
				while (1) {
					waitcount++;
					if (waitcount==20) {
						waitcount=0;
						break;
					}
					PA_WaitForVBL();
				}

				/* ���� ���� �ʱ�ȭ */
				for(i=0; i<SIZE; i++) {
					for(j=0; j<SIZE; j++) {
						destroy[i][j] = 0; // destroy �迭 �ʱ�ȭ
					}
				}
				combo++;
				ScoreCount = 0;
				moneyCount = 0;
			}

			/* �޺� �� �����ġ���� ���� �ʱ�ȭ */
			combo = 1;
			blockchanged = FALSE;
		}

		if(uinfo.userScore >= stageScore){ //�ܰ迡 ������ ���ھ� ���� ������Ű�� ���� ��������
			(uinfo.userStage)++;
			return GAMEPLAY;
		}

		if( restTime == 0 ) {	  // �ð��Ҹ� ���� ���� ����
			return GAMEOVER;
		}

		PA_WaitForVBL();
	}
}

/* �б��� */
int scr_Season() {

	PA_Init();
	PA_InitVBL();
	UnLoad_Screen();
	PA_EasyBgLoad(UP_SCREEN, BG3, Title);
	PA_EasyBgLoad(DOWN_SCREEN, BG3, Season);	/* ���ὺũ�� ���*/

	while (1) {
		if ( Stylus.Newpress ) {
			// shop
			if ( (Stylus.X>=21&&Stylus.X<=140)&&(Stylus.Y>=42&&Stylus.Y<=75) ) {
				return GO_SHOP;
			// next
			} else if ( (Stylus.X>=116&&Stylus.X<=231)&&(Stylus.Y>=131&&Stylus.Y<=164) ) {
				return GO_NEXT;
			}
		}
		PA_WaitForVBL();
	}
}

/* ���� */
void scr_Shop() {
	int itemindex = -1;
	int selectItem = -1;

	PA_Init();
	PA_InitVBL();
	UnLoad_Screen();
	PA_InitText(UP_SCREEN, BG2);
	PA_SetTextTileCol(UP_SCREEN, TEXT_BLACK);
	PA_EasyBgLoad(UP_SCREEN, BG3, Title);
	PA_EasyBgLoad(DOWN_SCREEN, BG3, Shop);
	PA_OutputText(UP_SCREEN, 10, 2, "Stage : %d", uinfo.userStage);
	PA_OutputText(UP_SCREEN, 10, 5, "Score : %d", uinfo.userScore);
	PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
	PA_OutputText(UP_SCREEN, 5, 15, "Mission Score  : %d", stageScore);
	/* ����� �����۶� ��� */
	PA_LoadSpritePal(DOWN_SCREEN, IPAL, (void*)item_Pal); //��������Ʈ �ȷ�Ʈ ����.
	item_init();

	while (1) {
		/* ������ ���� */
		if ( Stylus.Newpress ) {
			// Row Bomb
			if ( (Stylus.X>=21&&Stylus.X<=85)&&(Stylus.Y>=25&&Stylus.Y<=46) ) {
				selectItem = ITEM_BOMB_ROW;
			// Col Bomb
			} else if ( (Stylus.X>=21&&Stylus.X<=85)&&(Stylus.Y>=48&&Stylus.Y<=69) ) {
				selectItem = ITEM_BOMB_COL;
			// Small Bomb
			} else if ( (Stylus.X>=21&&Stylus.X<=85)&&(Stylus.Y>=73&&Stylus.Y<=92) ) {
				selectItem = ITEM_BOMB_SMALL;
			// Big Bomb
			} else if ( (Stylus.X>=21&&Stylus.X<=85)&&(Stylus.Y>=96&&Stylus.Y<=116) ) {
				selectItem = ITEM_BOMB_BIG;
			// Time ++
			} else if ( (Stylus.X>=21&&Stylus.X<=85)&&(Stylus.Y>=120&&Stylus.Y<=142) ) {
				selectItem = ITEM_TIME;
			// New Blocks
			} else if ( (Stylus.X>=21&&Stylus.X<=85)&&(Stylus.Y>=144&&Stylus.Y<=164) ) {
				selectItem = ITEM_RELOCATE;
			// Exit
			} else if ( (Stylus.X>=218&&Stylus.X<=242)&&(Stylus.Y>=171&&Stylus.Y<=181) ) {
				return;
			}

			itemindex = getitemindex(); // �� ������ ���� �ε����� ���Ѵ�.

			if ( itemindex!=-1 ) {			// �� ������ ���� �����ϸ�
				// ������ ����.
				switch ( selectItem ) {
				case ITEM_BOMB_ROW:
					if ( uinfo.userMoney < ITEM_PRICE_ROW ) { // ����ڰ� ����� ���� ������ ����������.
						; // �ʿ�� ������ �������.
						selectItem = -1; // ������ ���� ���.
					} else {
						uinfo.userMoney = uinfo.userMoney - ITEM_PRICE_ROW;
						PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
					}
					break;
				case ITEM_BOMB_COL:
					if ( uinfo.userMoney < ITEM_PRICE_COL ) { // ����ڰ� ����� ���� ������ ����������.
						; // �ʿ�� ������ �������.
						selectItem = -1; // ������ ���� ���.
					} else {
						uinfo.userMoney = uinfo.userMoney - ITEM_PRICE_COL;
						PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
					}
					break;
				case ITEM_BOMB_SMALL:
					if ( uinfo.userMoney < ITEM_PRICE_SMALL ) { // ����ڰ� ����� ���� ������ ����������.
						; // �ʿ�� ������ �������.
						selectItem = -1; // ������ ���� ���.
					} else {
						uinfo.userMoney = uinfo.userMoney - ITEM_PRICE_SMALL;
						PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
					}
					break;
				case ITEM_BOMB_BIG:
					if ( uinfo.userMoney < ITEM_PRICE_BIG ) { // ����ڰ� ����� ���� ������ ����������.
						; // �ʿ�� ������ �������.
						selectItem = -1; // ������ ���� ���.
					} else {
						uinfo.userMoney = uinfo.userMoney - ITEM_PRICE_BIG;
						PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
					}
					break;
				case ITEM_TIME:
					if ( uinfo.userMoney < ITEM_PRICE_TIME ) { // ����ڰ� ����� ���� ������ ����������.
						; // �ʿ�� ������ �������.
						selectItem = -1; // ������ ���� ���.
					} else {
						uinfo.userMoney = uinfo.userMoney - ITEM_PRICE_TIME;
						PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
					}
					break;
				case ITEM_RELOCATE:
					if ( uinfo.userMoney < ITEM_PRICE_RELOCATE ) { // ����ڰ� ����� ���� ������ ����������.
						; // �ʿ�� ������ �������.
						selectItem = -1; // ������ ���� ���.
					} else {
						uinfo.userMoney = uinfo.userMoney - ITEM_PRICE_RELOCATE;
					}
					break;
				default:
					break;
				}

				/* ������ ���� */
				if ( selectItem!=-1 ) {
					uinfo.userItem[itemindex] = selectItem;
					item_init();
					PA_ClearTextBg(UP_SCREEN);
					PA_OutputText(UP_SCREEN, 10, 2, "Stage : %d", uinfo.userStage);
					PA_OutputText(UP_SCREEN, 10, 5, "Score : %d", uinfo.userScore);
					PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
					PA_OutputText(UP_SCREEN, 5, 15, "Mission Score  : %d", stageScore);
					selectItem = -1;

				// �� ���� ������ ���.
				} else {
					; // �ʿ�� ������ �������.
				}

			// ������ �� ������ ������ ���.
			} else {
				; // �ʿ�� ������ �������.
			}
		}
		PA_WaitForVBL();
	}

}

/* ���� ���� */
void scr_Gameover()
{
	PA_Init();
	PA_InitVBL();
	UnLoad_Screen();
	PA_EasyBgLoad(UP_SCREEN, BG3, Title);
	PA_EasyBgLoad(DOWN_SCREEN, BG3, Gameover);	/* ���ὺũ�� ���*/

	while (1) {
		if ( Stylus.Newpress ) {
			newGame = TRUE; // ���ο� ����.
			return;
		}
		PA_WaitForVBL(); // while�� �ȿ� �� �־����. �߿�!
	}
}

/* �������� �� �ʱ�ȭ */
void stage_init(int ustage) {
	switch(ustage) {
		case SEASON_1ST:
			stageTime=300;					//�������� �ð� �ʱ�ȭ
			stageScore=1500;				//�������� ���ھ� �ʱ�ȭ
			break;
		case SEASON_2ND:
			stageTime=270;					//�������� �ð� �ʱ�ȭ
			stageScore=3000;				//�������� ���ھ� �ʱ�ȭ
			break;
		case SEASON_3RD:
			stageTime=240;					//�������� �ð� �ʱ�ȭ
			stageScore=5000;					//�������� ���ھ� �ʱ�ȭ
			break;
		case SEASON_4TH:
			stageTime=210;					//�������� �ð� �ʱ�ȭ
			stageScore=8000;					//�������� ���ھ� �ʱ�ȭ
			break;
		case SEASON_5TH:
			stageTime=180;					//�������� �ð� �ʱ�ȭ
			stageScore=12000;					//�������� ���ھ� �ʱ�ȭ
			break;
		case SEASON_6TH:
			stageTime=150;					//�������� �ð� �ʱ�ȭ
			stageScore=15000;					//�������� ���ھ� �ʱ�ȭ
			break;
		case SEASON_7TH:
			stageTime=120;					//�������� �ð� �ʱ�ȭ
			stageScore=20000;					//�������� ���ھ� �ʱ�ȭ
			break;
		case SEASON_8TH:
			stageTime=90;					//�������� �ð� �ʱ�ȭ
			stageScore=25000;					//�������� ���ھ� �ʱ�ȭ
			break;
	}

	restTime = stageTime; // �����ð� ����.
}

/* ��ġ �Է��� ����� �� �ε����� ��ȯ.*/
int gettouchcol(int x) {

	if ( (x>=0)&&(x<=32) ) {
		return 0;

	} else if ( (x>=33)&&(x<=64) ) {
		return 1;

	} else if ( (x>=65)&&(x<=96) ) {
		return 2;

	} else if ( (x>=97)&&(x<=128) ) {
		return 3;

	} else if ( (x>=129)&&(x<=160) ) {
		return 4;

	} else if ( (x>=161)&&(x<=192) ) {
		return 5;

	} else {
		return -1;
	}
}

/* ��ġ �Է��� ����� �� �ε����� ��ȯ.*/
int gettouchrow(int y) {

	if ( (y>=0)&&(y<=32) ) {
		return 0;

	} else if ( (y>=33)&&(y<=64) ) {
		return 1;

	} else if ( (y>=65)&&(y<=96) ) {
		return 2;

	} else if ( (y>=97)&&(y<=128) ) {
		return 3;

	} else if ( (y>=129)&&(y<=160) ) {
		return 4;

	} else if ( (y>=161)&&(y<=192) ) {
		return 5;

	} else {
		return -1;
	}
}

/* ȭ���ʱ�ȭ �Լ�. �����޸𸮸� �ʱ�ȭ�Ѵ�. */
void UnLoad_Screen()
{
	int i;
	for(i=0; i<2; ++i)
	{
		PA_ResetBgSysScreen(i);
		PA_ResetSpriteSysScreen(i);
	}
}

/* �� ����� �������� �Ǻ� */
BOOL isNearBlock(int row, int col, int oldrow, int oldcol)
{
	/* ����� ������ ���� �����¿� 4���� �� */
	if ( ((col == oldcol) && (row+1 == oldrow)) ||	// �Ʒ��� ����
		 ((col == oldcol) && (row-1 == oldrow)) ||	// ���� ����
		 ((col-1 == oldcol) && (row == oldrow)) ||	// ������ ����
		 ((col+1 == oldcol) && (row == oldrow)) )		// ���� ����
		return TRUE;
	else
		return FALSE;

}

/* �� ����� �� �ٲ�*/
void swapBlock(int row, int col, int oldrow, int oldcol)
{
	int temp;


	temp = blocks[row][col];
	blocks[row][col] = blocks[oldrow][oldcol];
	blocks[oldrow][oldcol] = temp;

	// ��� ��������Ʈ ��ġ ����
	swapsprite(row,col,oldrow,oldcol,Blksprite[oldrow][oldcol], Blksprite[row][col]);
}

/* �� ����� ��������Ʈ �ٲ�*/
void swapsprite(int row, int col, int oldrow, int oldcol,int oldsprite,int newsprite) {
	int temp_sprite; // ��������Ʈ �ѹ� �ӽ� ���庯��.
	int oldX = PA_GetSpriteX(DOWN_SCREEN,oldsprite);
	int oldY = PA_GetSpriteY(DOWN_SCREEN,oldsprite);
	int newX = PA_GetSpriteX(DOWN_SCREEN,newsprite);
	int newY = PA_GetSpriteY(DOWN_SCREEN,newsprite);

	// ��� ��������Ʈ �迭 ����.
	temp_sprite = Blksprite[row][col];
	Blksprite[row][col] = Blksprite[oldrow][oldcol];
	Blksprite[oldrow][oldcol] = temp_sprite;

	PA_SetSpriteXY(DOWN_SCREEN,oldsprite,newX,newY);
	PA_SetSpriteXY(DOWN_SCREEN,newsprite,oldX,oldY);
}

/* ������ �ʱ�ȭ �� ���� */
void item_init() {
	int i;
	int itemspriteindex = ITEM_FIRST_SPRITE;

	if ( newGame==TRUE ) {
		for ( i=0;i<(USER_ITEM_MAX);i++ ) {
			uinfo.userItem[i]=i+1; 			// ���� ������ ����.
			PA_CreateSprite(DOWN_SCREEN, itemspriteindex,(void*)item_Sprite, OBJ_SIZE_32X32,1,IPAL, 224, (i)*32);
			PA_SetSpriteAnim(DOWN_SCREEN, itemspriteindex, uinfo.userItem[i]); // ��Ϲ迭�� ���� ��������Ʈ�� �ݿ�.
			itemspriteindex++;
		}
		newGame = FALSE;
	} else {
		for ( i=0;i<(USER_ITEM_MAX);i++ ) {
			PA_CreateSprite(DOWN_SCREEN, itemspriteindex,(void*)item_Sprite, OBJ_SIZE_32X32,1,IPAL, 224, (i)*32);
			if ( uinfo.userItem[i]==-1 ) {
				PA_SetSpriteAnim(DOWN_SCREEN, itemspriteindex, 0); // ��Ϲ迭�� ���� ��������Ʈ�� �ݿ�.
			} else {
				PA_SetSpriteAnim(DOWN_SCREEN, itemspriteindex, uinfo.userItem[i]); // ��Ϲ迭�� ���� ��������Ʈ�� �ݿ�.
			}
			itemspriteindex++;
		}
	}
}

/* ��� �ʱ�ȭ */
void block_init()
{
	int i, j;
	int randnum;
	int k = 0; // ��������Ʈ �ѹ�.

	for ( i=0;i<SIZE;i++ ) {
		for ( j=0;j<SIZE;j++ ){
			randnum = PA_RandMax(NUM_OF_BLOCK);
			blocks[j][i] = randnum;
			PA_CreateSprite(DOWN_SCREEN, k,(void*)number_Sprite, OBJ_SIZE_32X32,1,NPAL, i*32, j*32);
			Blksprite[j][i] = k; // �ش� �����������Ʈ�����迭 ��  �ʱ�ȭ
			PA_SetSpriteAnim(DOWN_SCREEN, k, blocks[j][i]); // ��Ϲ迭�� ���� ��������Ʈ�� �ݿ�.
			k++;
		}
	}
}

/* ��� ��������Ʈ ����*/
void block_reload()
{
	int i, j;
	int k = 0; // ��������Ʈ �ѹ�.

	PA_LoadSpritePal(DOWN_SCREEN, NPAL, (void*)number_Pal); //��������Ʈ �ȷ�Ʈ ����.

	for ( i=0;i<SIZE;i++ ) {
		for ( j=0;j<SIZE;j++ ){
			PA_CreateSprite(DOWN_SCREEN, k,(void*)number_Sprite, OBJ_SIZE_32X32,1,NPAL, i*32, j*32);
			Blksprite[j][i] = k; // �ش� �����������Ʈ�����迭 ��  �ʱ�ȭ
			PA_SetSpriteAnim(DOWN_SCREEN, k, blocks[j][i]); // ��Ϲ迭�� ���� ��������Ʈ�� �ݿ�.
			k++;
		}
	}
}


/* �� üũ */
int checkRow(int row)
{
	int base;
	int i, j;
	int match = 0;
	int count = 0;
	int checkcount = 0;

	for(i=0; i<(SIZE-2); i++) {
		match = 0;
		base = blocks[row][i];
		for(j=i+1; j<SIZE; j++) {
			if (base == blocks[row][j]) {
				match++;

				if (match == 2) {
					if(blocks[row][j] == BLK_CASH){
						destroy[row][j-2] = MONEYPLUS;
						destroy[row][j-1] = MONEYPLUS;
						destroy[row][j] = MONEYPLUS;
					}else{
						destroy[row][j-2] = KILL;
						destroy[row][j-1] = KILL;
						destroy[row][j] = KILL;
						checkcount++;
					}
				}

				if (match >= 3){
					if(blocks[row][j] == BLK_CASH){
						destroy[row][j] = MONEYPLUS;
					}else{
						destroy[row][j] = KILL;
					}
				}

			} else
				break;
		}
	}

	for(i=0; i<SIZE; i++) {
		if ((destroy[row][i] == KILL) && (checkcount != 0))
			count++;
	}

	return count;
}

/* �� üũ */
int checkCol(int col)
{
	int base;
	int i, j;
	int match = 0;
	int count = 0;
	int checkcount = 0;

	for(i=0; i<(SIZE-2); i++) {
		match = 0;
		base = blocks[i][col];
		for(j=i+1; j<SIZE; j++) {
			if (base == blocks[j][col]) {
				match++;

				if (match == 2) {
					if(blocks[j][col] == BLK_CASH){
						destroy[j-2][col] = MONEYPLUS;
						destroy[j-1][col] = MONEYPLUS;
						destroy[j][col] = MONEYPLUS;
					}
					else{
						destroy[j-2][col] = KILL;
						destroy[j-1][col] = KILL;
						destroy[j][col] = KILL;
						checkcount++;
					}
				}

				if (match >= 3){
					if(blocks[j][col] == BLK_CASH){
						destroy[j][col] = MONEYPLUS;
					}else{
						destroy[j][col] = KILL;
					}
				}

			} else
				break;
		}
	}
	for(i=0; i<SIZE; i++) {
		if ((destroy[i][col] == KILL) && (checkcount != 0))
			count++;
	}

	return count;
}

/* ���� ��ϱ׷� üũ */
BOOL blocksCrack(int combo) {
	int i, j, temp;

	for(i=0; i<SIZE; i++){
		/* �� ó�� */
		temp = checkRow(i);
		if (temp == 3)										// ��� ��� �� ��ŭ ���� �ݿ�
			uinfo.userScore += 50 * combo;
		else if (temp == 4)
			uinfo.userScore += 70 * combo;
		else if (temp == 5)
			uinfo.userScore += 100 * combo;
		else if (temp == 6)
			uinfo.userScore += 200 * combo;

		/* �� ó�� */
		temp = checkCol(i);
		if (temp == 3)										// ��� ��� �� ��ŭ ���� �ݿ�
			uinfo.userScore += 50 * combo;
		else if (temp == 4)
			uinfo.userScore += 70 * combo;
		else if (temp == 5)
			uinfo.userScore += 100 * combo;
		else if (temp == 6)
			uinfo.userScore += 200 * combo;
	}

	for (i=0; i<SIZE; i++) {
		for (j=0; j<SIZE; j++) {
			if (destroy[i][j] == KILL) {
				return TRUE;
			}
			else if (destroy[i][j] == MONEYPLUS) {
				return TRUE;
			}
		}
	}
	return FALSE;
}


/* ���õ� �����۶� �ε��� ��ȯ */
int itemgettouchrow(int y) {


	if ( (y>=0)&&(y<=32) ) {
		return 0;

	} else if ( (y>=33)&&(y<=64) ) {
		return 1;

	} else if ( (y>=65)&&(y<=96) ) {
		return 2;

	} else if ( (y>=97)&&(y<=128) ) {
		return 3;

	} else if ( (y>=129)&&(y<=160) ) {
		return 4;

	} else {
		return -1;
	}
}

/* ��� �ߵ� ������ */
void useOneClickItem(int item)
{
	int i, j;

	switch (item) {
	case ITEM_TIME:
		restTime = restTime + 20;		// 20�� ����
		if (restTime > stageTime)
			restTime = stageTime;

		break;

	case ITEM_RELOCATE:					// ��� ���ġ
		for (i=0; i<6; i++) {
			for (j=0; j<6; j++) {
				blocks[i][j] = PA_RandMax(NUM_OF_BLOCK);	// ���ο� ��� �� ����
				PA_SetSpriteAnim(DOWN_SCREEN, Blksprite[i][j], blocks[i][j]); // ��Ϲ迭�� ���� ��������Ʈ�� �ݿ�.
			}
		}
		break;

	default :
		return;
	}
}

/* ��� ���� �䱸 ������  */
BOOL useTwoClickItem(int item, int row, int col)
{
	int i, j;
	int targetRow = row;
	int targetCol = col;
	int waitcount=0;

	/* ���õ� ����� �������� ȿ������ */
	switch (item) {
	/* ���� �� �� ���� */
	case ITEM_BOMB_ROW:
		// ���  ��� �׷��� ȿ��
		for (i=0; i<SIZE; i++) {
			PA_StartSpriteAnimEx(DOWN_SCREEN, Blksprite[targetRow][i], 10, 14, 15, ANIM_LOOP, 1);
		}
		// �����
		for (i=0; i<SIZE; i++) {
			if (blocks[targetRow][i] == BLK_CASH) {			// �� ����� �����ϸ� �� ȹ��
				uinfo.userMoney += 100;
			}
		}
		uinfo.userScore += 200;								// ���� ���.
		// ����� ���ھ� ����.
		PA_OutputText(UP_SCREEN, 10, 5, "Score : %d", uinfo.userScore);
		PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);

		// �ִϸ��̼� ȿ���� ������ ��ٸ�
		while (1) {
			waitcount++;
			if (waitcount==25) {
				waitcount=0;
				break;
			}
			PA_WaitForVBL();
		}
		// ���ο� �� ������ �Բ� ��������Ʈ ����.
		for (i=0; i<SIZE; i++) {		// ���ο� ��� ����
			blocks[targetRow][i] = PA_RandMax(NUM_OF_BLOCK);
			PA_SetSpriteAnim(DOWN_SCREEN, Blksprite[targetRow][i], blocks[targetRow][i]);
		}

		// �޺� �ߵ��� �ð������� Ȯ���� �� �ִ� �ð��� ��.
		while (1) {
			waitcount++;
			if (waitcount==20) {
				waitcount=0;
				break;
			}
			PA_WaitForVBL();
		}

		break;

	/* ���� �� �� ���� */
	case ITEM_BOMB_COL:
		for (i=0; i<SIZE; i++) {	// ��� ��� �׷��� ȿ��
			PA_StartSpriteAnimEx(DOWN_SCREEN, Blksprite[i][targetCol], 10, 14, 15, ANIM_LOOP, 1);
		}
		for (i=0; i<SIZE; i++) {
			if (blocks[i][targetCol] == BLK_CASH) {			// �� ����� �����ϸ� �� ȹ��
				uinfo.userMoney += 100;
			}
		}
		uinfo.userScore += 200;								// ���� ���
		PA_OutputText(UP_SCREEN, 10, 5, "Score : %d", uinfo.userScore);
		PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);

		// �ִϸ��̼� ȿ���� ������ ��ٸ�
		while (1) {
			waitcount++;
			if (waitcount==25) {
				waitcount=0;
				break;
			}
			PA_WaitForVBL();
		}

		// ���ο� �� ������ �Բ� ��������Ʈ ����.
		for (i=0; i<SIZE; i++) {		// ���ο� ��� ����
			blocks[i][targetCol] = PA_RandMax(NUM_OF_BLOCK);
			PA_SetSpriteAnim(DOWN_SCREEN, Blksprite[i][targetCol], blocks[i][targetCol]);
		}
		// �޺� �ߵ��� �ð������� Ȯ���� �� �ִ� �ð��� ��.
		while (1) {
			waitcount++;
			if (waitcount==20) {
				waitcount=0;
				break;
			}
			PA_WaitForVBL();
		}
		break;

	/* 9ĭ ����	*/
	case ITEM_BOMB_SMALL:
		for (i=targetRow-1; i<targetRow+2; i++) {
			for (j=targetCol-1; j<targetCol+2; j++) {
				if (i>=0 && j>=0 && i<SIZE && j<SIZE) {
					PA_StartSpriteAnimEx(DOWN_SCREEN, Blksprite[i][j], 10, 14, 15, ANIM_LOOP, 1);
				}
			}
		}
		for (i=targetRow-1; i<targetRow+2; i++) {
			for (j=targetCol-1; j<targetCol+2; j++) {
				if (i>=0 && j>=0 && i<SIZE && j<SIZE) {
					if (blocks[targetRow][i] == BLK_CASH)	// �� ����� �����ϸ� �� ȹ��
						uinfo.userMoney += 100;
				}
			}
		}
		uinfo.userScore += 300;								// ���� ���
		PA_OutputText(UP_SCREEN, 10, 5, "Score : %d", uinfo.userScore);
		PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
		// �ִϸ��̼� ȿ���� ������ ��ٸ�
		while (1) {
			waitcount++;
			if (waitcount==25) {
				waitcount=0;
				break;
			}
			PA_WaitForVBL();
		}
		// ���ο� �� ������ �Բ� ��������Ʈ ����.
		for (i=targetRow-1; i<targetRow+2; i++) {
			for (j=targetCol-1; j<targetCol+2; j++) {
				if (i>=0 && j>=0 && i<SIZE && j<SIZE) {
					blocks[i][j] = PA_RandMax(NUM_OF_BLOCK);
					PA_SetSpriteAnim(DOWN_SCREEN, Blksprite[i][j], blocks[i][j]);
				}
			}
		}
		// �޺� �ߵ��� �ð������� Ȯ���� �� �ִ� �ð��� ��.
		while (1) {
			waitcount++;
			if (waitcount==20) {
				waitcount=0;
				break;
			}
			PA_WaitForVBL();
		}
		break;

	/* 25ĭ ���� */
	case ITEM_BOMB_BIG:
		// �׷��� ȿ��.
		for (i=targetRow-2; i<targetRow+3; i++) {
			for (j=targetCol-2; j<targetCol+3; j++) {
				if (i>=0 && j>=0 && i<SIZE && j<SIZE) {
					PA_StartSpriteAnimEx(DOWN_SCREEN, Blksprite[i][j], 10, 14, 15, ANIM_LOOP, 1);
				}
			}
		}
		for (i=targetRow-2; i<targetRow+3; i++) {
			for (j=targetCol-2; j<targetCol+3; j++) {
				if (i>=0 && j>=0 && i<SIZE && j<SIZE) {
					if (blocks[targetRow][i] == BLK_CASH) {	// �� ����� �����ϸ� �� ȹ��
						uinfo.userMoney += 100;
					}
				}
			}
		}
		uinfo.userScore += 500;								// ���� ���
		PA_OutputText(UP_SCREEN, 10, 5, "Score : %d", uinfo.userScore);
		PA_OutputText(UP_SCREEN, 10, 7, "Cash  : %d", uinfo.userMoney);
		// �ִϸ��̼� ȿ���� ������ ��ٸ�
		while (1) {
			waitcount++;
			if (waitcount==25) {
				waitcount=0;
				break;
			}
			PA_WaitForVBL();
		}
		// ���ο� �� ������ �Բ� ��������Ʈ ����.
		for (i=targetRow-2; i<targetRow+3; i++) {
			for (j=targetCol-2; j<targetCol+3; j++) {
				if (i>=0 && j>=0 && i<SIZE && j<SIZE) {
					blocks[i][j] = PA_RandMax(NUM_OF_BLOCK);
					PA_SetSpriteAnim(DOWN_SCREEN, Blksprite[i][j], blocks[i][j]);
				}
			}
		}
		// �޺� �ߵ��� �ð������� Ȯ���� �� �ִ� �ð��� ��.
		while (1) {
			waitcount++;
			if (waitcount==20) {
				waitcount=0;
				break;
			}
			PA_WaitForVBL();
		}


		break;

	/* ��ȿ�� �������� �ƴ� �� */
	default:
		return FALSE;
	}

	return TRUE;
}

int getitemindex() {
	int i;

	for ( i=0;i<(USER_ITEM_MAX);i++ ) {
		if ( uinfo.userItem[i]==-1 ) {
			return i;
		}
	}

	return -1;
}

/* ����������ʱ�ȭ */
void user_init() {
	uinfo.userMoney=0;
	uinfo.userScore=0;
	uinfo.userStage=1;
	uinfo.userItem[0]=-1;
	uinfo.userItem[1]=-1;
	uinfo.userItem[2]=-1;
	uinfo.userItem[3]=-1;
	uinfo.userItem[4]=-1;
	uinfo.activeBlockCol = -1;
	uinfo.activeBlockRow = -1;
	uinfo.activeItem = -1;
}
