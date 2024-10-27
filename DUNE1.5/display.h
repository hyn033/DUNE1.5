/*
*  display.c:
* ȭ�鿡 ���� ������ ���
* ��, Ŀ��, �ý��� �޽���, ����â, �ڿ� ���� ���
* io.c�� �ִ� �Լ����� �����
*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common.h"

// ǥ���� ���� ����. ���� ���� �״µ�, ���ⲯ �߰��ϰų� �����ϱ�
#define COLOR_DEFAULT	15   //���� ��� + �Ͼ� �� (0*16)+15
#define COLOR_CURSOR	112  //ȸ�� ��� + ���� ��   (7*16)+0
#define COLOR_RESOURCE  112  //ȸ�� ��� + ���� ��   (7*16)+0

// ������ �ڿ�, ��, Ŀ���� ǥ��
// ������ ȭ�鿡 ǥ���� ����� ���⿡ �߰��ϱ�
void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor
);
void display_message(char message[BOX_HEIGHT][MAP_WIDTH]);
void display_situation(char situation[MAP_HEIGHT][BOX_WIDTH]);
void display_order(char order[BOX_HEIGHT][BOX_WIDTH]);
#endif