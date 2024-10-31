/*
*  display.c:
* 화면에 게임 정보를 출력
* 맵, 커서, 시스템 메시지, 정보창, 자원 상태 등등
* io.c에 있는 함수들을 사용함
*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common.h"

// 표시할 색상 정의. 대충 맞춰 뒀는데, 취향껏 추가하거나 변경하기
#define COLOR_CLEAN		0
#define COLOR_DEFAULT	15   //검은 배경 + 하얀 색
#define COLOR_CURSOR	240  //하얀 배경 + 검정 색   (7*16)+0
#define COLOR_RESOURCE  112  //회색 배경 + 검정 색   (7*16)+0
#define COLOR_BASE_RED  79   //진한 빨간 배경 + 하얀 색   (4*16)+15
#define COLOR_BASE_BLUE  31  //진한 파란 배경 + 하얀 색   (1*16)+15
#define COLOR_ROCK  143		 //진한 회색 배경 + 하얀 색  (8*16)+15
#define COLOR_SPICE 207		 //주황 배경 + 검정 색	(6*16)+0
#define COLOR_PLATE 15		 //검정 배경 + 하얀 색	(0*16)+15
#define COLOR_SANDWARM 224	 //진한 노랑 배경 + 하얀 색	(6*16)+15
#define COLOR_SCREEN 112	 //회색 배경 + 검정 색

// 지금은 자원, 맵, 커서만 표시
// 앞으로 화면에 표시할 내용들 여기에 추가하기
void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	int map_color[MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor
);
#endif