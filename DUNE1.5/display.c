/*
*  display.c:
* 화면에 게임 정보를 출력
* 맵, 커서, 시스템 메시지, 정보창, 자원 상태 등등
* io.c에 있는 함수들을 사용함
*/

#include "display.h"
#include "io.h"

// 출력할 내용들의 좌상단(topleft) 좌표
const POSITION resource_pos = { 0, 0 };
const POSITION map_pos = { 1, 0 };  //map이 시작하는 위치(최상단)
const POSITION message_pos = { MAP_HEIGHT + 1 , 0 }; //message 창 실행 하는 위치
const POSITION situation_pos = { 1, MAP_WIDTH }; //situation이 시작하는 위치(map옆)
const POSITION order_pos = { MAP_HEIGHT+1,MAP_WIDTH}; //order가 시작하는 위치(map대각선)

char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],int map_color[MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);
void display_4cursor();
void display_situation_text(CURSOR);
void remove_4cursor(CURSOR);
extern int map_color[MAP_HEIGHT][MAP_WIDTH];	
extern CURSOR cursor;

void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	int map_color[MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor
)
{
	display_resource(resource);
	display_map(map,map_color);
	display_cursor(cursor);
	set_color(COLOR_DEFAULT);
}
void display2(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	int map_color[MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor
)
{
	display_resource(resource);
	display_map(map, map_color);
	display_4cursor(cursor);
	set_color(COLOR_DEFAULT);
}

void display_resource(RESOURCE resource) {
	set_color(COLOR_RESOURCE);  //글자색 바꾸는 함수
	gotoxy(resource_pos);
	printf("spice = %d/%d, population=%d/%d\n",
		resource.spice, resource.spice_max,
		resource.population, resource.population_max
	);
}

// subfunction of draw_map()
void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			for (int k = 0; k < N_LAYER; k++) {
				if (src[k][i][j] >= 0) {
					dest[i][j] = src[k][i][j];  //지형을 없애지 않고 상위 레이어에 유닛을 보여줌
				}
			}
		}
	}
}

void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],int map_color[MAP_HEIGHT][MAP_WIDTH]) {
	project(map, backbuf);

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (frontbuf[i][j] != backbuf[i][j]) {
				POSITION pos = { i, j };
				printc(padd(map_pos, pos), backbuf[i][j], map_color[i][j]);
			}
			frontbuf[i][j] = backbuf[i][j];
		}
	}
}

// frontbuf[][]에서 커서 위치의 문자를 색만 바꿔서 그대로 다시 출력
void display_cursor(CURSOR cursor) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	char ch = frontbuf[prev.row][prev.column];
	printc(padd(map_pos, prev), ch, map_color[prev.row][prev.column]);  //컬러 디폴트로 커서를 지움

	ch = frontbuf[curr.row][curr.column];
	printc(padd(map_pos, curr), ch, COLOR_CURSOR);  //현재 커서 위치를 표시
}

void display_4cursor() {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	// 이전 커서 지우기 (2x2)
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			char ch = frontbuf[prev.row + i][prev.column + j];
			printc(padd(map_pos, (POSITION) { prev.row + i, prev.column + j }),
				ch, map_color[prev.row + i][prev.column + j]);
		}
	}

	// 새로운 화면 커서(2x2)
	if (curr.row == 16) {
		cursor.current.row = 15;
		curr.row = 15;
	}
	if (curr.column == 58) {
		cursor.current.column = 57;
		curr.column = 57;
	}

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			char ch = frontbuf[curr.row + i][curr.column + j];
			printc(padd(map_pos, (POSITION) { curr.row + i, curr.column + j }),
				ch, COLOR_CURSOR);
		}
	}
}
void remove_4cursor(CURSOR cursor) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	// 이전 커서 지우기 (2x2)
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			char ch = frontbuf[curr.row + i][curr.column + j];
			printc(padd(map_pos, (POSITION) { curr.row + i, curr.column + j }),
				ch, map_color[curr.row + i][curr.column + j]); 
		}
	}
}
