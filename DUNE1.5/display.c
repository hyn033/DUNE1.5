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
const POSITION message_pos = { MAP_HEIGHT + 1 , 0 }; //message�� �����ϴ� ��ġ(map�Ʒ�)
const POSITION situation_pos = { 1,MAP_WIDTH + 1 };  //situation�� �����ϴ� ��ġ(map��)
const POSITION order_pos = { MAP_HEIGHT + 1 ,	MAP_WIDTH + 1 }; //order�� �����ϴ� ��ġ(map�밢��)

char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);
void display_message(char message[BOX_HEIGHT][MAP_WIDTH]);
void display_situation(char situation[MAP_HEIGHT][BOX_WIDTH]);
void display_order(char order[BOX_HEIGHT][BOX_WIDTH]);


void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor
)
{
	display_resource(resource);
	display_map(map);
	display_cursor(cursor);
	// display_object_info()
	// display_commands()
	// ...
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

void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	project(map, backbuf);

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (frontbuf[i][j] != backbuf[i][j]) {
				POSITION pos = { i, j };
				printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT);
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
	printc(padd(map_pos, prev), ch, COLOR_DEFAULT);  //�÷� ����Ʈ�� Ŀ���� ����

	ch = frontbuf[curr.row][curr.column];
	printc(padd(map_pos, curr), ch, COLOR_CURSOR);  //���� Ŀ�� ��ġ�� ǥ��
}

void display_message(char message[BOX_HEIGHT][MAP_WIDTH]) {
	for (int i = 0; i < BOX_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			POSITION pos = { i, j };
			printc(padd(message_pos, pos), message[i][j], COLOR_DEFAULT);
		}
	}
}

void display_situation(char situation[MAP_HEIGHT][BOX_WIDTH]) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < BOX_WIDTH; j++) {
			POSITION pos = { i, j };
			printc(padd(situation_pos, pos), situation[i][j], COLOR_DEFAULT);
		}
	}
}
void display_order(char order[BOX_HEIGHT][BOX_WIDTH]) {
	for (int i = 0; i < BOX_HEIGHT; i++) {
		for (int j = 0; j < BOX_WIDTH; j++) {
			POSITION pos = { i, j };
			printc(padd(order_pos, pos), order[i][j], COLOR_DEFAULT);
		}
	}
}