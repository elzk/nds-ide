/*
 * graphic.h
 *
 *  Created on: 2010. 7. 6.
 *      Author: ��ǻ�Ͱ��а�
 */

#ifndef GRAPHIC_H_
#define GRAPHIC_H_

#define TRUE 1
#define FALSE 0

#define UP_SCREEN 1
#define DOWN_SCREEN 0

#define SPRITE_NUM 30 // ��������Ʈ ����

#include "all_gfx.h"

/* Image Processing Function */
void load_background(PA_BgStruct* bg);
void setsize_sprite();
void load_sprite(int sprite_num, const unsigned short file_name_Pal[],const unsigned char file_name_Sprite[]);
void del_sprite(int sprite_num);
void UnLoad_Screen();

struct ui_info {
    u64 flag;      // �� ��ư�� Ȱ��ȭ�� (�� ȭ�鿡 ǥ�õ�) ��Ȳ�� �˻��ϱ� ���� flag
    u64 next_flag; // �� ��ư�� ���� ��, ȭ���� �ٲ�ٸ�,..
                   // �� ȭ�鿡 ���̴� ��ư���� enable�� flag
    int redraw;    // �� ��ư�� ���� �� ȭ���� ���� �׷����� �ϸ�
                   // ��, ���ο� �޴� ������ �ٲ��, 1, �ƴϸ� 0
                   // next_flag != current_flag�̸� �Ƹ��� 1
    unsigned int x;
    unsigned int y;
    unsigned int img_widht;
    unsigned int img_height;

    PA_BgStruct* bg;
    int bgflag; // background flag

    const unsigned short* file_name_Pal;
    const unsigned char* file_name_Sprite;
    //char *image;           // ���� �� ��ư (tab) ��������Ʈ �̹����� ���� pointer
    char *image_focused; // �� ��ư�� focus�� ���� �� �̹���,

    int  (*action)(void);  // int�� �Լ� ������
    void (*another_action)(void); // action�� �ٸ� ������ ������ �ʿ��ϸ�
};

extern struct ui_info ui[SPRITE_NUM];

#endif /* GRAPHIC_H_ */
