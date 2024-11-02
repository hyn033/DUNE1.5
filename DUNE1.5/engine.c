#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "common.h"
#include "io.h"
#include "display.h"

// �߸� ������ ����� ������ ����


void map_making(void);
void map_coloring(void);
void message_making(void);
void situation_making(void);
void order_making(void);
void clean_situation(CURSOR cursor);
void view_situation(CURSOR cursor);
void view_order(CURSOR cursor);
void intro(void);
void outro(void);
void cursor_move(DIRECTION dir);
void sample_obj_move(void);
void dest_sandwarm(void);
POSITION sandwarm_position(int);

/* ================= control =================== */
int sys_clock = 0;		// system-wide clock(ms)
int click_start_timer = 0; //Ÿ�̸� ����
int double_click = 90;
bool timer_on = 0;
CURSOR dash_cursor = { {0,0},{0,0} }; //�뽬�� ���� �� �뽬 �� �� ��ġ�� ���� -> �� �� ������ ������ �ʱ� ����
KEY last_key=k_none;
CURSOR cursor = { { 1, 1 }, {1, 1} };
POSITION destination = { 0, 0 };  //������ ������ ������� ��������

/* ================= game data =================== */
char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };   //map ����
int map_color[MAP_HEIGHT][MAP_WIDTH] = { 0 };		//map �� ����
char text_situ[13][50] = {
	"���� [����] �̴�.", // text[0] �츮 ����
	"�� ���� ���� ???�̴�.", // text[1] �� ����
	"[�縷����] �̴�.", //text[2] �縷����
	"�ƹ� �͵� �� �� ���� ���δ�.", //text[3] �縷����
	"[����]�̴�.",  //text[4] ����
	"[�����̽� ������] �̴�.", //text[5] �����̽�
	"�����̽� ���差 : 8 ",  //text[6] �����̽�
	"������ [��] �̴�.",  //text[7] ��
	"�ڼ��� �� ���� ���� �� ����.", //text[8] �� ����
	"�ǹ��� ���� �� �ִ�.", //text[9] ����
	"[�Ϻ�����] �̴�.", //text[10] �Ϻ�����
	"[�����] �̴�.", //text[11] �Ϻ�����
	"�ٰ����� �ʴ� ���� ���ƺ��δ�.." //text[12] �Ϻ�����
};
char text_order[10][50] = {
	"H : �Ϻ����� ����", //text[0] ���� ��ɾ�
	"H : ��Ȯ�ϱ�",		//text[1] ��� ��ɾ�
	"M : �����̱�"		//text[2] 
};
// extern
extern char backbuf[MAP_HEIGHT][MAP_WIDTH]; 
extern const POSITION message_pos; //message�� �����ϴ� ��ġ(map�Ʒ�)
extern const POSITION situation_pos; //situation�� �����ϴ� ��ġ(map��)
extern const POSITION order_pos; //order�� �����ϴ� ��ġ(map�밢��)


RESOURCE resource = {
	.spice = 0,
	.spice_max = 0,
	.population = 0,
	.population_max = 0
};

OBJECT_SAMPLE sandW[2] = {
	{
	.pos = {3, 10},
	.dest = {0, 0},
	.repr = 'W',
	.speed = 700,
	.next_move_time = 300
	},
	{
	.pos = {13, 40},
	.dest = {0,0},
	.repr = 'W',
	.speed = 700,
	.next_move_time = 300
	}
};

/* ================= main() =================== */
int main(void) {
	srand((unsigned int)time(NULL));
	map_making();
	map_coloring();
	message_making();
	situation_making();
	order_making();
	//intro();
	display(resource, map, map_color, cursor);

	while (1) {
		// loop �� ������(��, TICK==10ms����) Ű �Է� Ȯ��
		KEY key = get_key();
		if (click_start_timer > double_click && timer_on) { //Ÿ�̸Ӱ� �����ְ� 70�ʸ� �Ѱ��� ��쿣 ��ĭ �����̱�
			cursor_move(ktod(last_key)); 
			timer_on = 0; 
			click_start_timer = 0; 
		}
		if (is_arrow_key(key)) { // Ű �Է��� ������ ó��
			if (!timer_on) { //Ÿ�̸Ӱ� ����������
				timer_on = 1;
				last_key = key;
			}
			else if (last_key == key) { //Ÿ�̸Ӱ� �����ְ� Ű�� ���� ���
				for (int i = 0; i < 6; i++) {
					cursor_move(ktod(key));
					if (i == 0)
						dash_cursor.previous = cursor.previous;
					timer_on = 0;
					click_start_timer = 0;
				}
				cursor.previous = dash_cursor.previous;
			}
		}
		else {
			// ����Ű ���� �Է�
			switch (key) {
			case k_quit: outro();
			case k_none: break;
			case k_undef:break;
			case k_space: 
				clean_situation(cursor);
				view_situation(cursor);
				view_order(cursor);
				break;
			case k_esc:
				clean_situation(cursor);
				break;
			default: break;
			}
		}
		// ���� ������Ʈ ����
		dest_sandwarm();
		sample_obj_move();
		// ȭ�� ���
		map_coloring();
		display(resource, map, map_color, cursor);
		
		Sleep(TICK);
		if (timer_on) //Ÿ�̸Ӱ� ������������ ī��Ʈ
			click_start_timer += 10;
		sys_clock += 10;
	}
}

/* ================= �Լ� ���� =================== */
void intro(void) {
	printf("     _                       __      _____ \n");
	printf("    | |                     /  |    |  ___|\n");
	printf("  __| | _   _  _ __    ___  `| |    |___ ` \n");
	printf(" / _` || | | || '_  | / _ `  | |        ` |\n");
	printf("| (_| || |_| || | | ||  __/ _| |_ _  ___/ /\n");
	printf(" `__,_| `__,_||_| |_| |___| |___/(_)|____/ \n");
	Sleep(2000);
	system("cls");
}

void outro(void) {
	printf("exiting...\n");
	exit(0);
}

void map_making(void) {
	// layer 0(map[0])�� ���� ����
	for (int j = 0; j < MAP_WIDTH; j++) {
		map[0][0][j] = 'x';
		map[0][MAP_HEIGHT - 1][j] = 'x';
	}

	for (int i = 1; i < MAP_HEIGHT - 1; i++) {
		map[0][i][0] = 'x';
		map[0][i][MAP_WIDTH - 1] = 'x';
		for (int j = 1; j < MAP_WIDTH - 1; j++) {
			map[0][i][j] = ' ';
		}
	}

	// layer 1(map[1])�� ��� �α�(-1�� ä��) // ���� �߰�
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			map[1][i][j] = -1;
		}
	}

	//����
	map[0][1][58] = 'B';
	map[0][1][57] = 'B';
	map[0][2][58] = 'B';
	map[0][2][57] = 'B';
	map[0][15][1] = 'B';
	map[0][15][2] = 'B';
	map[0][16][1] = 'B';
	map[0][16][2] = 'B';
	//����
	map[0][1][56] = 'P';
	map[0][1][55] = 'P';
	map[0][2][56] = 'P';
	map[0][2][55] = 'P';
	map[0][15][3] = 'P';
	map[0][15][4] = 'P';
	map[0][16][3] = 'P';
	map[0][16][4] = 'P';
	//��
	map[0][3][22] = 'R';
	map[0][3][23] = 'R';
	map[0][4][22] = 'R';
	map[0][4][23] = 'R';
	map[0][12][32] = 'R';
	map[0][12][31] = 'R';
	map[0][13][32] = 'R';
	map[0][13][31] = 'R';
	map[0][14][52] = 'R';
	map[0][7][44] = 'R';
	map[0][10][12] = 'R';

	//�����̽� ������
	map[0][12][1] = 'S';
	map[0][5][58] = 'S';

	//�Ϻ�����
	map[1][14][1] = 'H';
	map[1][3][58] = 'H';

	//����� 
	map[1][sandW[0].pos.row][sandW[0].pos.column] = 'W';
	map[1][sandW[1].pos.row][sandW[1].pos.column] = 'W';
}

void map_coloring(void) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (map[0][i][j] == 'x') map_color[i][j] = COLOR_DEFAULT;
			else if (map[0][i][j] == 'B') {
				if (i == 1 || i == 2) map_color[i][j] = COLOR_BASE_RED;
				else map_color[i][j] = COLOR_BASE_BLUE;
			}
			else if (map[1][i][j] == 'H') {
				if (i == 3) map_color[i][j] = COLOR_BASE_RED;
				else map_color[i][j] = COLOR_BASE_BLUE;
			}
			else if (map[0][i][j] == 'R') map_color[i][j] = COLOR_ROCK;
			else if (map[0][i][j] == 'S') map_color[i][j] = COLOR_SPICE;
			else if (map[0][i][j] == 'P') map_color[i][j] = COLOR_PLATE;
			else if (map[1][i][j] == 'W') map_color[i][j] = COLOR_SANDWARM;
			else map_color[i][j] =COLOR_SCREEN;
		}
	}
}

void message_making(void) {
	for (int i = 0; i < BOX_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			POSITION pos = { i, j };
			padd(message_pos, pos);
			gotoxy(padd(message_pos, pos));
			if (i == 0 || i == BOX_HEIGHT - 1 || j == 0 || j == MAP_WIDTH - 1) {
				set_color(COLOR_DEFAULT);
				printf("*");
			}
			else
				printf("");
		}
	}
}

void situation_making(void) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < BOX_WIDTH; j++) {
			POSITION pos = { i, j };
			padd(situation_pos, pos);
			gotoxy(padd(situation_pos, pos));
			if (i == 0 || i == MAP_HEIGHT - 1 || j == 0 || j == BOX_WIDTH - 1) {
				set_color(COLOR_DEFAULT);
				printf("*");
			}
			else
				printf("");
		}
	}
}

void order_making(void) {
	for (int i = 0; i < BOX_HEIGHT; i++) {
		for (int j = 0; j < BOX_WIDTH; j++) {
			POSITION pos = { i, j };
			padd(order_pos, pos);
			gotoxy(padd(order_pos, pos));
			if (i == 0 || i == BOX_HEIGHT - 1 || j == 0 || j == BOX_WIDTH - 1) {
				set_color(COLOR_DEFAULT);
				printf("*");
			}
			else
				printf("");
		}
	}
}

// (�����ϴٸ�) ������ �������� Ŀ�� �̵�
void cursor_move(DIRECTION dir) {
	POSITION curr = cursor.current;
	POSITION new_pos = pmove(curr, dir);

	// validation check
	if (1 <= new_pos.row && new_pos.row <= MAP_HEIGHT - 2 && \
		1 <= new_pos.column && new_pos.column <= MAP_WIDTH - 2) {
		cursor.previous = cursor.current;
		cursor.current = new_pos;
	}
}

void clean_situation(CURSOR cursor) {
	for (int i = 5; i < 9; i++) {
		for (int j = 0; j < BOX_WIDTH; j++) {
			if (j != 0 && j != BOX_WIDTH - 1) {
				POSITION pos = { i, j };
				gotoxy(padd(situation_pos, pos));
				set_color(COLOR_CLEAN);
				printf(" ");
			}
		}
	}
	for (int i = 3; i < 6; i++) {
		for (int j = 0; j < BOX_WIDTH; j++) {
			if (j != 0 && j != BOX_WIDTH - 1) {
				POSITION pos = { i, j };
				gotoxy(padd(order_pos, pos));
				set_color(COLOR_CLEAN);
				printf(" ");
			}
		}
	}
}

void view_situation(CURSOR cursor) {
	int x = cursor.current.row;
	int y = cursor.current.column;
	POSITION pos = { 6, 5 };
	gotoxy(padd(situation_pos, pos));
	set_color(COLOR_DEFAULT);

	if (map[0][x][y] == 'B' && (y == 1 || y == 2)) printf("%s", text_situ[0]);
	else if (map[0][x][y] == 'B' && (x == 1 || x == 2)) {
		printf("%s", text_situ[1]);
		POSITION pos = { 8, 5 };
		gotoxy(padd(situation_pos, pos));
		printf("%s", text_situ[8]);
	}
	else if (map[1][x][y] == 'H') printf("%s", text_situ[10]);
	else if (map[1][x][y] == 'W') {
		printf("%s", text_situ[11]);
		POSITION pos = { 8, 5 };
		gotoxy(padd(situation_pos, pos));
		printf("%s", text_situ[12]);
	}
	else if (map[0][x][y] == ' ') {
		printf("%s", text_situ[2]);
		POSITION pos = { 8, 5 };
		gotoxy(padd(situation_pos, pos));
		printf("%s", text_situ[3]);
	}
	else if (map[0][x][y] == 'P') {
		printf("%s", text_situ[4]);
		POSITION pos = { 8, 5 };
		gotoxy(padd(situation_pos, pos));
		printf("%s", text_situ[9]);
	}
	else if (map[0][x][y] == 'S') {
		printf("%s", text_situ[5]);
		POSITION pos = { 8, 5 };
		gotoxy(padd(situation_pos, pos));
		printf("%s", text_situ[6]);
	}
	else if (map[0][x][y] == 'R') printf("%s", text_situ[7]);
}

void view_order(CURSOR cursor) {
	int x = cursor.current.row;
	int y = cursor.current.column;
	POSITION pos = { 4, 4 };
	gotoxy(padd(order_pos, pos));
	set_color(COLOR_DEFAULT);

	if(map[0][x][y] == 'B' && (y == 1 || y == 2)) printf("%s", text_order[0]);
	else if (map[1][x][y] == 'H' && (y == 1 || y == 2)) printf("%s\t\t%s", text_order[1],text_order[2]);
}
/* ================= sample object movement =================== */
POSITION sandwarm_position(int i) {
	POSITION curr = { sandW[i].pos.row, sandW[i].pos.column};
	POSITION dest = { sandW[i].dest.row, sandW[i].dest.column};
	if (dest.row == 0 && dest.column == 0) {
		//������ ���� �� ������� ������
		int key = (rand() % 5) + 1;
		POSITION next_pos = pmove(curr, key);
		if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
			1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
			map[1][next_pos.row][next_pos.column] < 0 && \
			map[0][next_pos.row][next_pos.column] != 'R' && \
			map[0][next_pos.row][next_pos.column] != 'B' && \
			map[0][next_pos.row][next_pos.column] != 'P') return next_pos;
		else return sandW[i].pos;  // ���ڸ�
	}
	else {
		//������ ���� �� ������� ������
		POSITION diff = psub(dest, curr);
		DIRECTION dir;
		if (abs(diff.row) >= abs(diff.column)) {
			dir = (diff.row >= 0) ? d_down : d_up;
		}
		else {
			dir = (diff.column >= 0) ? d_right : d_left;
		}
		POSITION next_pos = pmove(curr, dir);
		if (map[0][next_pos.row][next_pos.column] == 'R') {
			if (dir == d_right || dir == d_left) dir = (diff.row >= 0) ? d_down : d_up;
			else dir = (diff.column >= 0) ? d_right : d_left;
			next_pos = pmove(curr, dir);
			return next_pos;
		}
		else if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
			1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
			map[0][next_pos.row][next_pos.column] != 'R' && \
			map[0][next_pos.row][next_pos.column] != 'B' && \
			map[0][next_pos.row][next_pos.column] != 'P') return next_pos;
		else return sandW[i].pos;  // ���ڸ�
	}
}

void sample_obj_move(void) {
	for (int i = 0; i < 2; i++) {
		if (sys_clock <= sandW[i].next_move_time) {
			// ���� �ð��� �� ����
			return;
		}

		// ������Ʈ(�ǹ�, ���� ��)�� layer1(map[1])�� ����
		map[1][sandW[i].pos.row][sandW[i].pos.column] = -1;
		sandW[i].pos = sandwarm_position(i);
		map[1][sandW[i].pos.row][sandW[i].pos.column] = sandW[i].repr;

		sandW[i].next_move_time = sys_clock + sandW[i].speed;
	}
}

void dest_sandwarm(void) {
	sandW[0].dest.row = 0;
	sandW[0].dest.column = 0;
	sandW[1].dest.row = 0;
	sandW[1].dest.column = 0;
	POSITION compare_dest = { 0,0 };
	//�ʱ� ������ ����
	for (int x = 0; x < 2; x++) {
		for (int i = 0; i < MAP_HEIGHT; i++) {
			for (int j = 0; j < MAP_WIDTH; j++) {
				if (map[1][i][j] == 'H') {
					//�ʱ� ������ ( �Ÿ��� ������� �ʰ� ���� ���������� Ž���� ������)
					sandW[x].dest.row = i;
					sandW[x].dest.column = j;
				}
			}
		}
	}
	//�ʱ� ������ ���� ����� ���� ��Ž��
	for (int x = 0; x < 2; x++) {
		for (int i = 0; i < MAP_HEIGHT; i++) {
			for (int j = 0; j < MAP_WIDTH; j++) {
				if (map[1][i][j] == 'H') {
					//�ʱ� ������ ( �Ÿ��� ������� �ʰ� ���� ���������� Ž���� ������)
					compare_dest.row = i;
					compare_dest.column = j;
					if ((abs(sandW[x].pos.column - compare_dest.column)) < (abs(sandW[x].pos.column - sandW[x].dest.column))) {
						sandW[x].dest.row = compare_dest.row;
						sandW[x].dest.column = compare_dest.column;
					}
				}
			}
		}
	}
}
