/*
*  display.c:
* ȭ�鿡 ���� ������ ���
* ��, Ŀ��, �ý��� �޽���, ����â, �ڿ� ���� ���
* io.c�� �ִ� �Լ����� �����
*/

#include "display.h"
#include "io.h"

// ����� ������� �»��(topleft) ��ǥ
const POSITION resource_pos = { 0, 0 };
const POSITION map_pos = { 1, 0 };  //map�� �����ϴ� ��ġ(�ֻ��)
const POSITION message_pos = { MAP_HEIGHT + 1 , 0 }; //message â ���� �ϴ� ��ġ
const POSITION situation_pos = { 1, MAP_WIDTH }; //situation�� �����ϴ� ��ġ(map��)
const POSITION order_pos = { MAP_HEIGHT+1,MAP_WIDTH}; //order�� �����ϴ� ��ġ(map�밢��)

char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],int map_color[MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);
void display_situation_text(CURSOR);
extern int map_color[MAP_HEIGHT][MAP_WIDTH];

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

void display_resource(RESOURCE resource) {
	set_color(COLOR_RESOURCE);  //���ڻ� �ٲٴ� �Լ�
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
					dest[i][j] = src[k][i][j];  //������ ������ �ʰ� ���� ���̾ ������ ������
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

// frontbuf[][]���� Ŀ�� ��ġ�� ���ڸ� ���� �ٲ㼭 �״�� �ٽ� ���
void display_cursor(CURSOR cursor) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	char ch = frontbuf[prev.row][prev.column];
	printc(padd(map_pos, prev), ch, map_color[prev.row][prev.column]);  //�÷� ����Ʈ�� Ŀ���� ����

	ch = frontbuf[curr.row][curr.column];
	printc(padd(map_pos, curr), ch, COLOR_CURSOR);  //���� Ŀ�� ��ġ�� ǥ��
}

