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
#define COLOR_DEFAULT	15   //���� ��� + �Ͼ� ��
#define COLOR_CURSOR	240  //�Ͼ� ��� + ���� ��   (7*16)+0
#define COLOR_RESOURCE  112  //ȸ�� ��� + ���� ��   (7*16)+0
#define COLOR_BASE_RED  79   //���� ���� ��� + �Ͼ� ��   (4*16)+15
#define COLOR_BASE_BLUE  31  //���� �Ķ� ��� + �Ͼ� ��   (1*16)+15
#define COLOR_ROCK  143		 //���� ȸ�� ��� + �Ͼ� ��  (8*16)+15
#define COLOR_SPICE 207		 //��Ȳ ��� + ���� ��	(6*16)+0
#define COLOR_PLATE 15		 //���� ��� + �Ͼ� ��	(0*16)+15
#define COLOR_SANDWARM 111	 //���� ��� ��� + �Ͼ� ��	(6*16)+15
#define COLOR_SCREEN 112	 //ȸ�� ��� + ���� ��

// ������ �ڿ�, ��, Ŀ���� ǥ��
// ������ ȭ�鿡 ǥ���� ����� ���⿡ �߰��ϱ�
void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	int map_color[MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor
);
void display_message(char message[BOX_HEIGHT][MAP_WIDTH]);
void display_situation(char situation[MAP_HEIGHT][BOX_WIDTH]);
void display_order(char order[BOX_HEIGHT][BOX_WIDTH]);
void map_coloring(int map_color[MAP_HEIGHT][MAP_WIDTH]);
#endif