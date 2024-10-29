#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "common.h"
#include "io.h"
#include "display.h"

// �ǹ�&����, Ŀ�� �� �߰�

void map_making(void);
void map_coloring(void);
void message_making(void);
void situation_making(void);
void order_making(void);
void intro(void);
void outro(void);
void cursor_move(DIRECTION dir);
void sample_obj_move(void);
POSITION sample_obj_next_position(void);


/* ================= control =================== */
int sys_clock = 0;		// system-wide clock(ms)
int click_start_timer = 0; //Ÿ�̸� ����
int double_click = 90;
bool timer_on = 0;
CURSOR dash_cursor = { {0,0},{0,0} }; //�뽬�� ���� �� �뽬 �� �� ��ġ�� ���� -> �� �� ������ ������ �ʱ� ����
KEY last_key=k_none;
CURSOR cursor = { { 1, 1 }, {1, 1} };


/* ================= game data =================== */
char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };   //map ����
char message[BOX_HEIGHT][MAP_WIDTH] = { 0 };		//�޼��� â 
char situation[MAP_HEIGHT][BOX_WIDTH] = { 0 };		//����â 
char order[BOX_HEIGHT][BOX_WIDTH] = { 0 };			//���â
int map_color[MAP_HEIGHT][MAP_WIDTH] = { 0 };		//map �� ����

RESOURCE resource = {
	.spice = 0,
	.spice_max = 0,
	.population = 0,
	.population_max = 0
};

OBJECT_SAMPLE obj = {
	.pos = {1, 1},
	.dest = {MAP_HEIGHT - 2, MAP_WIDTH - 2},
	.repr = 'o',
	.speed = 300,
	.next_move_time = 300
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
	display_message(message);
	display_situation(situation);
	display_order(order);
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
			case k_none:
			case k_undef:
			default: break;
			}
		}
		// ���� ������Ʈ ����
		sample_obj_move();
		// ȭ�� ���
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
	//����� 			
	map[1][3][10] = 'W';
	map[1][13][40] = 'W';
	//�Ϻ�����
	map[1][14][1] = 'H';
	map[1][3][58] = 'H';
	// object sample
	map[1][obj.pos.row][obj.pos.column] = 'o';
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
	// layer 0(map[0])�� ���� ����
	for (int i = 0; i < MAP_WIDTH; i++) {
		message[0][i] = '*';
		message[BOX_HEIGHT - 1][i] = '*';
	}//�ڽ��� ��, �Ʒ��κ� ä���

	for (int i = 1; i < BOX_HEIGHT - 1; i++) {
		message[i][0] = '*';
		message[i][MAP_WIDTH - 1] = '*';
		for (int j = 1; j < MAP_WIDTH - 1; j++) {
			message[i][j] = ' ';
		}//��� ���� ä���
	}//�ڽ��� ���� ä���
}

void situation_making(void) {
	// layer 0(map[0])�� ���� ����
	for (int i = 0; i < BOX_WIDTH; i++) {
		situation[0][i] = '*';
		situation[MAP_HEIGHT - 1][i] = '*';
	}//�ڽ��� ��, �Ʒ��κ� ä���

	for (int i = 1; i < MAP_HEIGHT - 1; i++) {
		situation[i][0] = '*';
		situation[i][BOX_WIDTH - 1] = '*';
		for (int j = 1; j < BOX_WIDTH - 1; j++) {
			situation[i][j] = ' ';
		}//��� ���� ä���
	}//�ڽ��� ���� ä���
}

void order_making(void) {
	// layer 0(map[0])�� ���� ����
	for (int i = 0; i < BOX_WIDTH; i++) {
		order[0][i] = '*';
		order[BOX_HEIGHT - 1][i] = '*';
	}//�ڽ��� ��, �Ʒ��κ� ä���

	for (int i = 1; i < BOX_HEIGHT - 1; i++) {
		order[i][0] = '*';
		order[i][BOX_WIDTH - 1] = '*';
		for (int j = 1; j < BOX_WIDTH - 1; j++) {
			order[i][j] = ' ';
		}//��� ���� ä���
	}//�ڽ��� ���� ä���
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

/* ================= sample object movement =================== */
POSITION sample_obj_next_position(void) {
	// ���� ��ġ�� �������� ���ؼ� �̵� ���� ����	
	POSITION diff = psub(obj.dest, obj.pos);
	DIRECTION dir;

	// ������ ����. ������ �ܼ��� ���� �ڸ��� �պ�
	if (diff.row == 0 && diff.column == 0) {
		if (obj.dest.row == 1 && obj.dest.column == 1) {
			// topleft --> bottomright�� ������ ����
			POSITION new_dest = { MAP_HEIGHT - 2, MAP_WIDTH - 2 };
			obj.dest = new_dest;
		}
		else {
			// bottomright --> topleft�� ������ ����
			POSITION new_dest = { 1, 1 };
			obj.dest = new_dest;
		}
		return obj.pos;
	}

	// ������, ������ �Ÿ��� ���ؼ� �� �� �� ������ �̵�
	if (abs(diff.row) >= abs(diff.column)) {
		dir = (diff.row >= 0) ? d_down : d_up;
	}
	else {
		dir = (diff.column >= 0) ? d_right : d_left;
	}

	// validation check
	// next_pos�� ���� ����� �ʰ�, (������ ������)��ֹ��� �ε����� ������ ���� ��ġ�� �̵�
	// ������ �浹 �� �ƹ��͵� �� �ϴµ�, ���߿��� ��ֹ��� ���ذ��ų� ���� ������ �ϰų�... ���
	POSITION next_pos = pmove(obj.pos, dir);
	if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
		1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
		map[1][next_pos.row][next_pos.column] < 0) {

		return next_pos;
	}
	else {
		return obj.pos;  // ���ڸ�
	}
}

void sample_obj_move(void) {
	if (sys_clock <= obj.next_move_time) {
		// ���� �ð��� �� ����
		return;
	}

	// ������Ʈ(�ǹ�, ���� ��)�� layer1(map[1])�� ����
	map[1][obj.pos.row][obj.pos.column] = -1;
	obj.pos = sample_obj_next_position();
	map[1][obj.pos.row][obj.pos.column] = obj.repr;

	obj.next_move_time = sys_clock + obj.speed;
}