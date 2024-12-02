#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "common.h"
#include "io.h"
#include "display.h"
#include <string.h>

//�Ϻ����� �ൿ ����
//+�����̱�, ��Ȯ�ϱ� ����


void map_making(void);
void map_coloring(void);
void message_making(void);
void situation_making(void);
void order_making(void);
void clean_situation(CURSOR cursor);
void clean_order(CURSOR cursor);
void view_situation(CURSOR cursor);
void view_order(CURSOR cursor);
void view_system(CURSOR cursor);
void intro(void);
void outro(void);
void cursor_move(DIRECTION dir);
void sample_obj_move(void);
void dest_sandwarm(void);
void spice_making(void);
void base_order(void);
void build_order(void);
void making_plate(void);
void plate_order(void);
void dormitory_order(void);
void garage_order(void);
void barracks_order(void);
void shelter_order(void);
void factory_order(void);
void arena_order(void);
void add_struct_map(void);
void making_buildings_AT(int);
void making_buildings_HC(int);
void making_harvest(int x, int y);
void move_harvest(void);
void reset_memories(void);
void harvest_harvest(void);
void harvesting(void);
POSITION sandwarm_position(int);
POSITION harvest_position(int);

/* ================= control =================== */
int sys_clock = 0;  	// system-wide clock(ms)
int click_start_timer = 0; //Ÿ�̸� �����Ͽ� �ð��� ��
int double_click = 90;  //90�̳��� �ݺ� Ŭ���� �뽬�ϵ��� �ϱ� ����
int spice_time = 15000;  //������� ù �����̽� ���� �ð�
int order_on = 1;  //��ɽ���ġ [ 0 : ������� / 1 : �Ǽ� ��� (�̶� B�� ���� �� ����) / 2 : �Ǽ� �޴� (�̶� P,G,D,B,S ���ð���
								//3 : plate ��ġ ���� / 4 : dormitory ��ġ ���� / 5 : garage ��ġ ���� / 6 : barracks / 7: shelter / 8 : arena / 9 : factory / 10 : �Ϻ���Ʈ �̵���� ]
int move_on = 0;	// 0 : ��Ȱ��ȭ 1 : �̵� 2 : ��Ȯ
int m_harvest = 0;	// ������ �Ϻ����͸� �����Ҷ�
int h_harvest = 0;  // ��Ȯ�� �Ϻ����� �����Ҷ�
int change_cursor = -1; //Ŀ�� ���� ���� [ 0 : Ŀ�� 4ĭ ���� ]
int B_cnt = 0; //BŰ�� �ι� �Է��� �� barracks �Ǽ�
int buildings_cnt_AT = 0;
int buildings_cnt_HC = 0;
int units_cnt_AT = 0;  //AT���� ����
int units_cnt_HC = 0;
bool timer_on = 0;  //Ÿ�̸� Ű�� ����
CURSOR dash_cursor = { {0,0},{0,0} }; //�뽬�� ���� �� �뽬 �� �� ��ġ�� ���� -> �� �� ������ ������ �ʱ� ����
KEY last_key=k_none;
CURSOR cursor = { { 1, 1 }, {1, 1} };
POSITION destination = { 0, 0 };  //������ ������ ������� ��������
build buildings_AT[100] = { 0 };
build buildings_HC[100] = { 0 };
unit units_HC[100] = { 0 };
unit units_AT[100] = { 0 };

/* ================= game data =================== */
char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };   //map ����
int map_color[MAP_HEIGHT][MAP_WIDTH] = { 0 };		//map �� ����
char text_situ[30][50] = {
	"���� [����] �̴�.", // text[0] �츮 ����
	"�� ���� ���� ???�̴�.", // text[1] �� ����
	"[�縷����] �̴�.", //text[2] �縷����
	"�ƹ� �͵� �� �� ���� ���δ�.", //text[3] �縷����
	"[����]�̴�.",  //text[4] ����
	"[�����̽� ������] �̴�.", //text[5] �����̽�
	"�����̽� ���差 : ",  //text[6] �����̽�
	"������ [��] �̴�.",  //text[7] ��
	"�ڼ��� �� ���� ���� �� ����.", //text[8] �� ����
	"�ǹ��� ���� �� �ִ�.", //text[9] ����
	"[�Ϻ�����] �̴�.", //text[10] �Ϻ�����
	"[�����] �̴�.", //text[11] �Ϻ�����
	"�ٰ����� �ʴ� ���� ���ƺ��δ�.", //text[12] �Ϻ�����
	"�ȶ��� �����̴�.", //text[13] ����
	"�α� ���� 10 �����Ѵ�.", //text[14] ����
	"���� â���̴�.", //text[15] â��
	"�����̽� ���差�� 10 �����Ѵ�.", //text[16] â��
	"������ ������ �� �ִ� �����̴�.", //text[17] ����
	"������ : 1", //text[18] ����
	"�������� ������ �� �ִ� ����ó�̴�.", //text[19] ����ó
	"������ : 5", //text[20] ����ó
	"ü�� : ", //text[21] �Ϻ����� ü��
	"���� �Ǽ� ��� : 1",  //text[22] ���� �Ǽ� ���
	"���� �Ǽ� ��� : 2",  //text[23] ���� �Ǽ� ���
	"â�� �Ǽ� ��� : 4",  //text[24] â�� �Ǽ� ���
	"���� �Ǽ� ��� : 4",  //text[25] ���� �Ǽ� ���
	"����ó �Ǽ� ��� : 5",  //text[26] ����ó �Ǽ� ���
};
char text_order[20][50] = {
	"H : �Ϻ����� ����", //text[0] ���� ��ɾ�
	"H : ��Ȯ�ϱ�",		//text[1] ��� ��ɾ�
	"M : �����̱�",		//text[2] 
	"B : �Ǽ��ϱ�",		//text[3] �Ǽ� ��ɾ� ( �⺻ ���� )
	"P : ���� �Ǽ�",	//text[4] �Ǽ� ��ɾ�
	"D : ���� �Ǽ�",	//text[5] �Ǽ� ��ɾ�
	"G : â�� �Ǽ�",	//text[6] �Ǽ� ��ɾ�
	"B : ���� �Ǽ�",	//text[7] �Ǽ� ��ɾ�
	"S : ����ó �Ǽ�",	//text[8] �Ǽ� ��ɾ�
	"Space : �Ǽ��ϱ�",	//text[9] �Ǽ� ��ɾ�
	"Space : Ŀ�� �̵� �� Ŭ��",	//text[10] �̵� ��ɾ�
	"Space : �����̽� �������� �����ּ���", //text[11] ��Ȯ ���
	"B : ���� ����", //text[12] ���� ���� ���
	"S : Ư������ ����", //text[12] Ư������ ���� ���
};
char text_system[30][50] = {
	"�Ϻ���Ʈ ����!", //text[0] �Ϻ���Ʈ ����
	"�����̽��� �����մϴ�.", //text[1] �����̽� ����
	"���̻� ������ �� �����ϴ�.", //text[2] �Ϻ���Ʈ �ڸ� ����
	"������� �Ϻ���Ʈ�� �Ծ����ϴ�!", //text[3] �Ϻ���Ʈ ����
	"������� �����̽��� �����߽��ϴ�!", //text[4] ����� �����̽� ����
	"������� �� �Ϻ���Ʈ�� �Ծ����ϴ�!", //text[5] �� �Ϻ���Ʈ ����
	"�� ������ �Ǽ��� �� �����ϴ�.",	  //text[6] ��ġ ��ġ �ȵɶ�
	"�Ǽ��� �Ϸ�Ǿ����ϴ�.",			  //text[7] ��ġ ��ġ �ɶ�
	"�α��� �ʹ� �����ϴ�.",			//text[8] �α����� ���� ��
	"��Ȯ ��......",			//text[9] �Ϻ���Ʈ ��Ȯ ��
	"�Ϻ���Ʈ�� ��Ȯ�� ���½��ϴ�!",			//text[10] �Ϻ���Ʈ ����
	"�Ϻ���Ʈ�� ����մϴ�.",			//text[11] �Ϻ���Ʈ ���
	"�Ϻ���Ʈ�� �̵��߽��ϴ�.",			//text[12] �Ϻ���Ʈ �̵�
	"�߸��� �������Դϴ�.",			//text[13] �Ϻ���Ʈ ������ �̻�
	"! �����̽� ���� �뷮 ���� !",			//text[14] �����̽� ���差�� �ʰ��Ҷ�

};
char system_view[7][58] = { 0 };
// extern
extern char backbuf[MAP_HEIGHT][MAP_WIDTH]; 
extern const POSITION message_pos; //message�� �����ϴ� ��ġ(map�Ʒ�)
extern const POSITION situation_pos; //situation�� �����ϴ� ��ġ(map��)
extern const POSITION order_pos; //order�� �����ϴ� ��ġ(map�밢��)

RESOURCE resource = {  //�ʱ� �Ϻ���Ʈ�� �ִ� 4�� ������
	.spice = 20,
	.spice_max = 20,
	.population = 5,
	.population_max = 20  //4*5 = 20
};

OBJECT_SAMPLE sandW[2] = {  //����� 2����
	{
	.pos = {3, 10},
	.dest = {0, 0},
	.repr = 'W',
	.speed = 3000,
	.next_move_time = 3000
	},
	{
	.pos = {13, 50},
	.dest = {0,0},
	.repr = 'W',
	.speed = 3000,
	.next_move_time = 3000
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
				if (move_on == 1) {
					clean_order(cursor);
					move_harvest();
				}
				if (move_on == 2) {
					clean_order(cursor);
					harvest_harvest();
				}
				making_plate();
				making_buildings_AT(order_on);
				making_buildings_HC(order_on);
				if(change_cursor==0)
					remove_4cursor(cursor); 
				clean_situation(cursor);
				clean_order(cursor);
				view_situation(cursor);
				view_order(cursor);
				break;
			case k_esc:
				remove_4cursor(cursor);
				clean_order(cursor);
				clean_situation(cursor);
				reset_memories();
				break;
			case k_H:
				base_order();
				if (order_on == -1)
					clean_order(cursor);
				if(move_on == 0)
					harvest_harvest();
				break;
			case k_B:
				B_cnt++;
				clean_situation(cursor);
				build_order();
				view_situation(cursor);
				if (B_cnt == 2) {
					if (order_on == 2) clean_order(cursor);
					barracks_order();
					reset_memories();
				}
				break;
			case k_P:
				if(order_on==2)
					clean_order(cursor);
				plate_order();
				reset_memories();
				break;
			case k_D:
				if(order_on==2)
					clean_order(cursor);
				dormitory_order();
				reset_memories();
				break;
			case k_G:
				if (order_on == 2)
					clean_order(cursor);
				garage_order();
				reset_memories();
				break;
			case k_S:
				if (order_on == 2)
					clean_order(cursor);
				shelter_order();
				reset_memories();
				break;
			case k_M:
				if (order_on == -1)
					clean_order(cursor);
				if (move_on == 0)
					move_harvest();
				break;
			default: break;
			}
		}
		// ���� ������Ʈ ����
		dest_sandwarm();
		sample_obj_move();
		harvesting();
		spice_making();
		// ȭ�� ���
		add_struct_map(); //����ü ������ map�� �ݿ�
		map_coloring();
		if(change_cursor == 0)
			display2(resource, map, map_color, cursor);
		else
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
	map[0][12][1] = '7';
	map[0][5][58] = '7';

	//�Ϻ�����
	units_AT[units_cnt_AT].onoff = 1;
	units_AT[units_cnt_AT].pos = (POSITION){ 14,1};
	units_AT[units_cnt_AT].home = (POSITION){ 14,1 };
	units_AT[units_cnt_AT].dest = (POSITION){ 14,1 };
	units_AT[units_cnt_AT].cost = 5;
	units_AT[units_cnt_AT].population = 5;
	units_AT[units_cnt_AT].next_move_time = 0;
	units_AT[units_cnt_AT].hp = 70;
	units_AT[units_cnt_AT].outlook = 0;
	units_AT[units_cnt_AT].ch = 'H';
	units_cnt_AT++;

	units_HC[units_cnt_HC].onoff = 1;
	units_HC[units_cnt_HC].pos = (POSITION){ 3,58};
	units_HC[units_cnt_HC].home = (POSITION){ 3,58 };
	units_HC[units_cnt_HC].dest = (POSITION){ 3,58 };
	units_HC[units_cnt_HC].cost = 5;
	units_HC[units_cnt_HC].population = 5;
	units_HC[units_cnt_HC].next_move_time = 0;
	units_HC[units_cnt_HC].hp = 70;
	units_HC[units_cnt_HC].outlook = 0;
	units_HC[units_cnt_HC].ch = 'H';
	units_cnt_HC++;

	//�����
	map[1][sandW[0].pos.row][sandW[0].pos.column] = 'W';
	map[1][sandW[1].pos.row][sandW[1].pos.column] = 'W';
}

void map_coloring(void) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (map[0][i][j] == 'x') map_color[i][j] = COLOR_DEFAULT;
			else if (map[1][i][j] == 'H') {
				if (i == 3) map_color[i][j] = COLOR_BASE_RED;
				else
					map_color[i][j] = COLOR_BASE_BLUE;
			}
			else if (map[0][i][j] == 'R') map_color[i][j] = COLOR_ROCK;
			else if ('1' <= map[0][i][j] && map[0][i][j] <= '9') map_color[i][j] = COLOR_SPICE;
			else if (map[0][i][j] == 'P')
				map_color[i][j] = COLOR_PLATE;
			else if (map[1][i][j] == 'W') map_color[i][j] = COLOR_SANDWARM;
			else if (map[0][i][j] == 'D') map_color[i][j] = COLOR_BUILD_AT;
			else if (map[0][i][j] == 'G') map_color[i][j] = COLOR_BUILD_AT;
			else if (map[0][i][j] == 'S') map_color[i][j] = COLOR_BUILD_AT;
			else if (map[0][i][j] == 'F') map_color[i][j] = COLOR_BUILD_HC;
			else if (map[0][i][j] == 'A') map_color[i][j] = COLOR_BUILD_HC;
			else if (map[0][i][j] == 'B') {
				if ((i == 1 || i == 2)&&(j==57||j==58)) map_color[i][j] = COLOR_BASE_RED;
				else if ((i == 15 || i == 16) && (j == 1 || j == 2)) map_color[i][j] = COLOR_BASE_BLUE;
				else map_color[i][j] = COLOR_BUILD_AT;
			}
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
			else printf("");
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
	POSITION pos = { 4, 4 };
	gotoxy(padd(order_pos, pos));
	set_color(COLOR_DEFAULT);
	printf("%s", text_order[3]);
}

// (�����ϴٸ�) ������ �������� Ŀ�� �̵�
void cursor_move(DIRECTION dir) {
	POSITION curr = cursor.current;
	POSITION new_pos = pmove(curr, dir);

	// validation check
	if (change_cursor == 0) {
		if (1 <= new_pos.row && new_pos.row <= MAP_HEIGHT - 3 && \
			1 <= new_pos.column && new_pos.column <= MAP_WIDTH - 3) {
			cursor.previous = cursor.current;
			cursor.current = new_pos;
		}
	}
	else {
		if (1 <= new_pos.row && new_pos.row <= MAP_HEIGHT - 2 && \
			1 <= new_pos.column && new_pos.column <= MAP_WIDTH - 2) {
			cursor.previous = cursor.current;
			cursor.current = new_pos;
		}
	}
}

void clean_situation(CURSOR cursor) {
	for (int i = 4; i < 14; i++) {
		for (int j = 0; j < BOX_WIDTH; j++) {
			if (j != 0 && j != BOX_WIDTH - 1) {
				POSITION pos = { i, j };
				gotoxy(padd(situation_pos, pos));
				set_color(COLOR_CLEAN);
				printf(" ");
			}
		}
	}
	POSITION pos = { 4, 4 };
	gotoxy(padd(order_pos, pos));
	set_color(COLOR_DEFAULT);
	printf("%s", text_order[3]);
	order_on = 1;
	change_cursor = -1;
}

void clean_order(CURSOR cursor) {
	for (int i = 2; i < 7; i++) {
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
	if (order_on == 2) {
		int i = 0;
		int j = 0;
		while (i < 7) {
			POSITION pos = { 4 + j, 5 };
			gotoxy(padd(situation_pos, pos));
			printf("%s", text_situ[22 + i]);
			i++;
			j +=2;
		}
		return;
	}
	if (map[0][x][y] == 'B') {
		if((y == 1 || y == 2)&&(x ==15 || x ==16))
			printf("%s", text_situ[0]);
		else if ((y == 57 || y == 58) && (x == 1 || x == 2)) {
			printf("%s", text_situ[1]);
			POSITION pos = { 8, 5 };
			gotoxy(padd(situation_pos, pos));
			printf("%s", text_situ[8]);
		}
		else {
			printf("%s", text_situ[17]);
			POSITION pos = { 8, 5 };
			gotoxy(padd(situation_pos, pos));
			printf("%s", text_situ[18]);
		}
	}
	else if (map[1][x][y] == 'H') {
		printf("%s", text_situ[10]);
		if (y < 8) {
			int hp=0;
			for (int i = 0; i < units_cnt_AT; i++) {
				if (x == units_AT[i].pos.row && y == units_AT[i].pos.column)
					hp = units_AT[i].hp;
			}
			POSITION pos = { 8, 5 };
			gotoxy(padd(situation_pos, pos));
			printf("%s%d", text_situ[21],hp);
		}
	}
	else if (map[1][x][y] == 'W') {		//�����
		printf("%s", text_situ[11]);
		POSITION pos = { 8, 5 };
		gotoxy(padd(situation_pos, pos));
		printf("%s", text_situ[12]);
	}
	else if (map[0][x][y] == ' ') {		//�縷
		printf("%s", text_situ[2]);
		POSITION pos = { 8, 5 };
		gotoxy(padd(situation_pos, pos));
		printf("%s", text_situ[3]);
	}
	else if (map[0][x][y] == 'P') {		//����
		printf("%s", text_situ[4]);
		POSITION pos = { 8, 5 };
		gotoxy(padd(situation_pos, pos));
		printf("%s", text_situ[9]);
	}
	else if (map[0][x][y] == 'D') {		//����
		printf("%s", text_situ[13]);
		POSITION pos = { 8, 5 };
		gotoxy(padd(situation_pos, pos));
		printf("%s", text_situ[14]);
	}
	else if (map[0][x][y] == 'G') {  //â��
		printf("%s", text_situ[15]);
		POSITION pos = { 8, 5 };
		gotoxy(padd(situation_pos, pos));
		printf("%s", text_situ[16]);
	}
	else if (map[0][x][y] == 'S') {  //����ó
		printf("%s", text_situ[19]);
		POSITION pos = { 8, 5 };
		gotoxy(padd(situation_pos, pos));
		printf("%s", text_situ[20]);
	}
	else if ('1'<= map[0][x][y] && map[0][x][y] <= '9') {		//�����̽� ������
		printf("%s", text_situ[5]);
		POSITION pos = { 8, 5 };
		gotoxy(padd(situation_pos, pos));
		printf("%s%c",text_situ[6], map[0][x][y]);
	}
	else if (map[0][x][y] == 'R') printf("%s", text_situ[7]);
}

void view_order(CURSOR cursor) {
	order_on = 1;
	change_cursor = -1;
	int x = cursor.current.row;
	int y = cursor.current.column;
	POSITION pos = { 4, 4 };
	gotoxy(padd(order_pos, pos));
	set_color(COLOR_DEFAULT);

	if (map[0][x][y] == 'B' && (y == 1 || y == 2)) {
		printf("%s", text_order[0]);
		order_on = 0;
	}
	else if (map[1][x][y] == 'H' && y < 25) {
		printf("%s\t\t%s", text_order[1], text_order[2]);
		order_on = -1;
	}
	else {
		printf("%s", text_order[3]);
		order_on = 1;
	}
}

void view_system(CURSOR cursor) {
	POSITION m_pos = { 7,2 };
	for (int i = 1; i < 8; i++) {
		for (int j = 1; j < MAP_WIDTH; j++) {
			POSITION pos = { i, j };
			gotoxy(padd(message_pos, pos));
			set_color(COLOR_CLEAN);
			printf(" ");
		}
	}
	for (int i = 6; i >= 0; i--) {
		gotoxy(padd(message_pos, m_pos));
		set_color(COLOR_DEFAULT);
		printf("%s", system_view[i]);
		m_pos.row -= 1;
	}
}
/* ================= sample object movement =================== */
POSITION sandwarm_position(int i) {
	POSITION curr = { sandW[i].pos.row, sandW[i].pos.column};
	POSITION dest = { sandW[i].dest.row, sandW[i].dest.column};
	if (dest.row == 0 && dest.column == 0) {
		//������ ���� �� ������� ������
		int dir = (rand() % 3) + 1; 
		POSITION next_pos = pmove(curr, dir); 
		if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
			1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
			map[0][next_pos.row][next_pos.column] == ' ') return next_pos;
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
		if (map[0][next_pos.row][next_pos.column] != ' ') {
			if (dir == d_right || dir == d_left) dir = (diff.row >= 0) ? d_down : d_up;
			else dir = (diff.column >= 0) ? d_right : d_left;
			next_pos = pmove(curr, dir);
			return next_pos;
		}
		else if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
			1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
			map[0][next_pos.row][next_pos.column] == ' ') return next_pos;
		else return sandW[i].pos;  // ���ڸ�
	}
}

void sample_obj_move(void) {
	for (int i = 0; i < 2; i++) {
		//������ �ð��̸� �����̱� �ƴϸ� �ѱ��
		if (sys_clock >= sandW[i].next_move_time) {
		// ������Ʈ(�ǹ�, ���� ��)�� layer1(map[1])�� ����
			map[1][sandW[i].pos.row][sandW[i].pos.column] = -1;
			sandW[i].pos = sandwarm_position(i);
			if (map[1][sandW[i].pos.row][sandW[i].pos.column] == 'H') {
				strcpy_s(system_view[0], 58, system_view[1]);
				strcpy_s(system_view[1], 58, system_view[2]);
				strcpy_s(system_view[2], 58, system_view[3]);
				strcpy_s(system_view[3], 58, system_view[4]);
				strcpy_s(system_view[4], 58, system_view[5]);
				strcpy_s(system_view[5], 58, system_view[6]);
				for (int j = 0; j < units_cnt_AT; j++) {
					if (sandW[i].pos.row == units_AT[j].pos.row && sandW[i].pos.column == units_AT[j].pos.column) {
						units_AT[j].onoff = 0;
						resource.population -= 5;
						snprintf(system_view[6], 58, text_system[3]);
					}
				}
				for (int j = 0; j < units_cnt_HC; j++) {
					if (sandW[i].pos.row == units_HC[j].pos.row && sandW[i].pos.column == units_HC[j].pos.column) {
						units_HC[j].onoff = 0;
						snprintf(system_view[6], 58, text_system[5]);
					}
				}
				view_system(cursor);
			}
			map[1][sandW[i].pos.row][sandW[i].pos.column] = sandW[i].repr;

			sandW[i].next_move_time = sys_clock + sandW[i].speed;
		}
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
					//�� ������ �Ÿ� Ž��
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

void spice_making(void) {
	for (int i = 0; i < 2; i++) {
		if (sys_clock <= spice_time) {
			// ���� �ð��� �� ����
			return;
		}
		// ������Ʈ(�ǹ�, ���� ��)�� layer1(map[1])�� ����
		int num_spice = (rand() % 9) + 1;
		char ch = num_spice + '0';
		set_color(COLOR_SPICE);
		map[0][sandW[i].pos.row][sandW[i].pos.column] = ch;
	}
	spice_time = sys_clock + 50000;
	strcpy_s(system_view[0], 58, system_view[1]);
	strcpy_s(system_view[1], 58, system_view[2]);
	strcpy_s(system_view[2], 58, system_view[3]);
	strcpy_s(system_view[3], 58, system_view[4]);
	strcpy_s(system_view[4], 58, system_view[5]);
	strcpy_s(system_view[5], 58, system_view[6]);
	snprintf(system_view[6], 58, text_system[4]);
	view_system(cursor);
}

void base_order(void) {
	//���� ������ �����ϴ��� Ȯ��
	int x = cursor.current.row;
	int y = cursor.current.column;
	if (map[0][x][y] == 'B' && order_on == 0) {
		strcpy_s(system_view[0], 58, system_view[1]);
		strcpy_s(system_view[1], 58, system_view[2]);
		strcpy_s(system_view[2], 58, system_view[3]);
		strcpy_s(system_view[3], 58, system_view[4]);
		strcpy_s(system_view[4], 58, system_view[5]);
		strcpy_s(system_view[5], 58, system_view[6]);
		POSITION pos = { x,y };
		if (resource.spice >= 5 && (resource.population + 5)<= resource.population_max) {
			if (map[1][14][1] != 'H' || map[1][14][2] != 'H' || map[1][14][3] != 'H' || map[1][14][4] != 'H') { //�Ϻ���Ʈ �ڸ��� �ִ� 4��
				if (map[1][14][1] != 'H') making_harvest(14, 1);
				else if (map[1][14][2] != 'H') making_harvest(14, 2);
				else if (map[1][14][3] != 'H') making_harvest(14, 3);
				else if (map[1][14][4] != 'H') making_harvest(14, 4);
				snprintf(system_view[6], 58, text_system[0]);
			}
			else snprintf(system_view[6], 58, text_system[2]);
		}
		else if (resource.spice < 5) snprintf(system_view[6], 58, text_system[1]);
		else snprintf(system_view[6], 58, text_system[8]);
		view_system(cursor);
	}
}

void build_order(void) {
	if (order_on == 1) {
		for (int i = 1; i < 6; i++) {
			POSITION pos = { i + 1, 4 };
			gotoxy(padd(order_pos, pos));
			set_color(COLOR_DEFAULT);
			printf("%s", text_order[4 + i-1]);
		}
		order_on = 2;
	}
}

void plate_order(void) {
	if (order_on == 2) {
		int x = cursor.current.row;
		int y = cursor.current.column;
		POSITION pos = { 4, 4 };
		gotoxy(padd(order_pos, pos));
		set_color(COLOR_DEFAULT);
		printf("%s", text_order[9]);
		order_on = 3;
		change_cursor = 0;
	}
}

void making_plate(void) {
	//order_on �� 3�϶� �����ϵ���
	if (order_on == 3 && resource.spice >= 1) {
		strcpy_s(system_view[0], 58, system_view[1]);
		strcpy_s(system_view[1], 58, system_view[2]);
		strcpy_s(system_view[2], 58, system_view[3]);
		strcpy_s(system_view[3], 58, system_view[4]);
		strcpy_s(system_view[4], 58, system_view[5]);
		strcpy_s(system_view[5], 58, system_view[6]);
		int x = cursor.current.row;
		int y = cursor.current.column;
		//Ŀ�� ���� 4ĭ�� �ƹ� �ǹ��� �������� ��ġ�ϵ���
		if (map[0][x][y] == ' ' && map[0][x + 1][y] == ' ' && map[0][x][y + 1] == ' ' && map[0][x + 1][y + 1] == ' ') {
			map[0][x][y] = 'P';
			map[0][x + 1][y] = 'P';
			map[0][x][y + 1] = 'P';
			map[0][x + 1][y + 1] = 'P';
			resource.spice -= 1;
			snprintf(system_view[6], 58, text_system[7]);
		}
		else snprintf(system_view[6], 58, text_system[6]);
		view_system(cursor);
	}
}

void dormitory_order(void) {
	if (order_on == 2) {
		int x = cursor.current.row;
		int y = cursor.current.column;
		POSITION pos = { 4, 4 };
		gotoxy(padd(order_pos, pos));
		set_color(COLOR_DEFAULT);
		printf("%s", text_order[9]);
		order_on = 4;
		change_cursor = 0;
	}
}

void garage_order(void) {
	if (order_on == 2) {
		int x = cursor.current.row;
		int y = cursor.current.column;
		POSITION pos = { 4, 4 };
		gotoxy(padd(order_pos, pos));
		set_color(COLOR_DEFAULT);
		printf("%s", text_order[9]);
		order_on = 5;
		change_cursor = 0;
	}
}

void barracks_order(void) {
	if (order_on == 2) {
		int x = cursor.current.row;
		int y = cursor.current.column;
		POSITION pos = { 4, 4 };
		gotoxy(padd(order_pos, pos));
		set_color(COLOR_DEFAULT);
		printf("%s", text_order[9]);
		order_on = 6;
		change_cursor = 0;
	}
}

void shelter_order(void) {
	if (order_on == 2) {
		int x = cursor.current.row;
		int y = cursor.current.column;
		POSITION pos = { 4, 4 };
		gotoxy(padd(order_pos, pos));
		set_color(COLOR_DEFAULT);
		printf("%s", text_order[9]);
		order_on = 7;
		change_cursor = 0;
	}
}

void arena_order(void) {
	if (order_on == 2) {
		int x = cursor.current.row;
		int y = cursor.current.column;
		POSITION pos = { 4, 4 };
		gotoxy(padd(order_pos, pos));
		set_color(COLOR_DEFAULT);
		printf("%s", text_order[9]);
		order_on = 8;
		change_cursor = 0;
	}
}

void factory_order(void) {
	if (order_on == 2) {
		int x = cursor.current.row;
		int y = cursor.current.column;
		POSITION pos = { 4, 4 };
		gotoxy(padd(order_pos, pos));
		set_color(COLOR_DEFAULT);
		printf("%s", text_order[9]);
		order_on = 9;
		change_cursor = 0;
	}
}

void add_struct_map(void) {
	for (int i = 0; i < buildings_cnt_AT; i++) {
		if (buildings_AT[i].onoff == 1) {
			map[0][buildings_AT[i].pos1.row][buildings_AT[i].pos1.column] = buildings_AT[i].ch;
			map[0][buildings_AT[i].pos2.row][buildings_AT[i].pos2.column] = buildings_AT[i].ch;
			map[0][buildings_AT[i].pos3.row][buildings_AT[i].pos3.column] = buildings_AT[i].ch;
			map[0][buildings_AT[i].pos4.row][buildings_AT[i].pos4.column] = buildings_AT[i].ch;
		}
	}
	for (int i = 0; i < buildings_cnt_HC; i++) {
		if (buildings_HC[i].onoff == 1) {
			map[0][buildings_HC[i].pos1.row][buildings_HC[i].pos1.column] = buildings_HC[i].ch;
			map[0][buildings_HC[i].pos2.row][buildings_HC[i].pos2.column] = buildings_HC[i].ch;
			map[0][buildings_HC[i].pos3.row][buildings_HC[i].pos3.column] = buildings_HC[i].ch;
			map[0][buildings_HC[i].pos4.row][buildings_HC[i].pos4.column] = buildings_HC[i].ch;
		}
	}
	for (int i = 0; i < units_cnt_AT; i++) {
		if (units_AT[i].onoff == 1) {
			map[1][units_AT[i].pos.row][units_AT[i].pos.column] = units_AT[i].ch;
		}
	}
	for (int i = 0; i < units_cnt_HC; i++) {
		if (units_HC[i].onoff == 1) {
			map[1][units_HC[i].pos.row][units_HC[i].pos.column] = units_HC[i].ch;
		}
	}
}

void making_buildings_AT(int order) {
	//order_on �� 4�϶� �����ϵ���
	int hp, cost;
	char ch;
	unsigned char color;
	if (order == 4 && resource.spice >= 2 ) {
		hp = 10;
		ch = 'D';
		color = COLOR_BUILD_AT;
		cost = 2;
	}
	else if (order == 5 && resource.spice >= 4) {
		hp = 10;
		ch = 'G';
		color = COLOR_BUILD_AT;
		cost = 4;
	}
	else if (order == 6 && resource.spice >= 4) {
		hp = 20;
		ch = 'B';
		color = COLOR_BUILD_AT;
		cost = 4;
	}
	else if (order == 7 && resource.spice >= 5) {
		hp = 30;
		ch = 'S';
		color = COLOR_BUILD_AT;
		cost = 5;
	}
	else return;  //order�� 4~7�� �ƴϸ� ���� x

	strcpy_s(system_view[0], 58, system_view[1]);
	strcpy_s(system_view[1], 58, system_view[2]);
	strcpy_s(system_view[2], 58, system_view[3]);
	strcpy_s(system_view[3], 58, system_view[4]);
	strcpy_s(system_view[4], 58, system_view[5]);
	strcpy_s(system_view[5], 58, system_view[6]);
	int x = cursor.current.row;
	int y = cursor.current.column;
	//Ŀ�� ���� 4ĭ�� P�� ������
	if (map[0][x][y] == 'P' && map[0][x + 1][y] == 'P' && map[0][x][y + 1] == 'P' && map[0][x + 1][y + 1] == 'P') {
		buildings_AT[buildings_cnt_AT].onoff = 1;
		buildings_AT[buildings_cnt_AT].pos1 = (POSITION){ x,y };
		buildings_AT[buildings_cnt_AT].pos2 = (POSITION){ x + 1,y };
		buildings_AT[buildings_cnt_AT].pos3 = (POSITION){ x,y + 1 };
		buildings_AT[buildings_cnt_AT].pos4 = (POSITION){ x + 1,y + 1 };
		buildings_AT[buildings_cnt_AT].hp = hp;
		buildings_AT[buildings_cnt_AT].ch = ch;
		buildings_AT[buildings_cnt_AT].color = color;
		buildings_AT[buildings_cnt_AT].cost = cost;
		buildings_cnt_AT++;
		resource.spice -= cost;
		snprintf(system_view[6], 58, text_system[7]);
		if(order == 4) resource.population_max += 10;
		if(order == 5) resource.spice_max += 10;
	}
	else snprintf(system_view[6], 58, text_system[6]);
	view_system(cursor);
}

void making_buildings_HC(int order) {
	//order_on �� 4�϶� �����ϵ���
	int hp, cost;
	char ch;
	unsigned char color;
	if (order == 8 && resource.spice >= 3) {
		hp = 10;
		ch = 'A';
		color = COLOR_BUILD_HC;
		cost = 3;
	}
	else if (order == 9 && resource.spice >= 5) {
		hp = 10;
		ch = 'F';
		color = COLOR_BUILD_HC;
		cost = 5;
	}
	else return;  //order�� 8~9�� �ƴϸ� ���� x
	int x = cursor.current.row;
	int y = cursor.current.column;
	//Ŀ�� ���� 4ĭ�� P�� ������
	if (map[0][x][y] == 'P' && map[0][x + 1][y] == 'P' && map[0][x][y + 1] == 'P' && map[0][x + 1][y + 1] == 'P') {
		buildings_HC[buildings_cnt_HC].onoff = 1;
		buildings_HC[buildings_cnt_HC].pos1 = (POSITION){ x,y };
		buildings_HC[buildings_cnt_HC].pos2 = (POSITION){ x + 1,y };
		buildings_HC[buildings_cnt_HC].pos3 = (POSITION){ x,y + 1 };
		buildings_HC[buildings_cnt_HC].pos4 = (POSITION){ x + 1,y + 1 };
		buildings_HC[buildings_cnt_HC].hp = hp;
		buildings_HC[buildings_cnt_HC].ch = ch;
		buildings_HC[buildings_cnt_HC].color = color;
		buildings_HC[buildings_cnt_HC].cost = cost;
		buildings_cnt_HC++;
		resource.spice -= cost;
	}
}

void making_harvest(int x, int y) {
	units_AT[units_cnt_AT].onoff = 1;
	units_AT[units_cnt_AT].pos = (POSITION){ x,y };
	units_AT[units_cnt_AT].home = (POSITION){ x,y };
	units_AT[units_cnt_AT].dest = (POSITION){ x,y };
	units_AT[units_cnt_AT].cost = 5;
	units_AT[units_cnt_AT].population = 5;
	units_AT[units_cnt_AT].next_move_time = 0;
	units_AT[units_cnt_AT].hp = 70;
	units_AT[units_cnt_AT].outlook = 0;
	units_AT[units_cnt_AT].get = 0;
	units_AT[units_cnt_AT].ch = 'H';
	resource.spice -= units_AT[units_cnt_AT].cost;
	resource.population += units_AT[units_cnt_AT].population;
	units_cnt_AT++;
}

void move_harvest(void) {
	int x = cursor.current.row;
	int y = cursor.current.column;
	if (map[1][x][y] == 'H' && order_on == -1) {
		POSITION pos = { 4, 4 };
		gotoxy(padd(order_pos, pos));
		set_color(COLOR_DEFAULT);
		printf("%s", text_order[10]);
		for (int i = 0; i < units_cnt_AT; i++) {
			if (units_AT[i].onoff == 1) {
				POSITION hpos = units_AT[i].pos;
				if (y == hpos.column && x == hpos.row)
					m_harvest = i;
			}
		}
		move_on = 1;
	}
	else if (order_on == -1 && move_on == 1) {
		POSITION curr = cursor.current;
		POSITION next_pos = { curr.row, curr.column };
		if ((abs(units_AT[m_harvest].pos.row - next_pos.row) == 1 || abs(units_AT[m_harvest].pos.column - next_pos.column) == 1)&& map[0][next_pos.row][next_pos.column] == ' ') {
			map[1][units_AT[m_harvest].pos.row][units_AT[m_harvest].pos.column] =' ';
			units_AT[m_harvest].pos.row = next_pos.row;
			units_AT[m_harvest].pos.column = next_pos.column;
			strcpy_s(system_view[0], 58, system_view[1]);
			strcpy_s(system_view[1], 58, system_view[2]);
			strcpy_s(system_view[2], 58, system_view[3]);
			strcpy_s(system_view[3], 58, system_view[4]);
			strcpy_s(system_view[4], 58, system_view[5]);
			strcpy_s(system_view[5], 58, system_view[6]);
			snprintf(system_view[6], 58, text_system[12]);
			view_system(cursor);
		}
		move_on = 0;
		order_on = 1;
	}
}

void reset_memories(void) {
	B_cnt = 0;
	move_on = 0;
}

void harvest_harvest(void) {
	int x = cursor.current.row;
	int y = cursor.current.column;
	if (map[1][x][y] == 'H' && order_on == -1) {
		POSITION pos = { 4, 4 };
		gotoxy(padd(order_pos, pos));
		set_color(COLOR_DEFAULT);
		printf("%s", text_order[11]);
		move_on = 2;
		for (int i = 0; i < units_cnt_AT; i++) {
			if (units_AT[i].onoff == 1) {
				POSITION hpos = units_AT[i].pos;
				if (y == hpos.column && x == hpos.row)
					h_harvest = i;
			}
		}
	}
	else if (move_on == 2 && order_on == -1) {
		POSITION curr = cursor.current;
		POSITION next_pos = { curr.row, curr.column };
		if (('1' <= map[0][next_pos.row][next_pos.column] && map[0][next_pos.row][next_pos.column] <= '9' )&& map[0][next_pos.row][next_pos.column] != ' ') {
			units_AT[h_harvest].dest.row = next_pos.row;
			units_AT[h_harvest].dest.column = next_pos.column;
			units_AT[h_harvest].next_move_time = 2000;
			strcpy_s(system_view[0], 58, system_view[1]);
			strcpy_s(system_view[1], 58, system_view[2]);
			strcpy_s(system_view[2], 58, system_view[3]);
			strcpy_s(system_view[3], 58, system_view[4]);
			strcpy_s(system_view[4], 58, system_view[5]);
			strcpy_s(system_view[5], 58, system_view[6]);
			snprintf(system_view[6], 58, text_system[11]);
			view_system(cursor);
			move_on = 0;
		}
		else {
			strcpy_s(system_view[0], 58, system_view[1]);
			strcpy_s(system_view[1], 58, system_view[2]);
			strcpy_s(system_view[2], 58, system_view[3]);
			strcpy_s(system_view[3], 58, system_view[4]);
			strcpy_s(system_view[4], 58, system_view[5]);
			strcpy_s(system_view[5], 58, system_view[6]);
			snprintf(system_view[6], 58, text_system[13]);
			view_system(cursor);
			move_on = 0;
		}
	}
}

POSITION harvest_position(int i) {
	POSITION curr = { units_AT[i].pos.row, units_AT[i].pos.column };
	POSITION dest = { units_AT[i].dest.row, units_AT[i].dest.column };
	POSITION home = { units_AT[i].home.row, units_AT[i].home.column };
	POSITION diff = psub(dest, curr);

	//���� ��Ȯ ���� ����
	if ((units_AT[i].home.row == units_AT[i].dest.row && units_AT[i].home.column == units_AT[i].dest.column) && curr.row == home.row && curr.column == home.column) {
		units_AT[i].next_move_time = 0;
		return units_AT[i].pos;
	}

	//������ ��Ȯ�ϱ� ��
	if (units_AT[i].get == 0) {
		diff = psub(dest, curr);
		//�������� �������� ��
		if (curr.row == dest.row && curr.column == dest.column) {
			if (map[0][dest.row][dest.column] != ' ') {
				int ch = map[0][dest.row][dest.column] - '0';
				int get_spice = (rand() % 3) + 2;
				if (get_spice > ch)
					get_spice = ch;
				units_AT[i].get = get_spice;
				ch -= get_spice;
				if (ch >= 1)
					map[0][dest.row][dest.column] = ch + '0';
				else
					map[0][dest.row][dest.column] = ' ';
			}
			else {
				units_AT[i].get = 0;
			}
			diff = psub(home, curr);
		}
	}
	//������ ��Ȯ�� ��
	else if (units_AT[i].get != 0) {
		if (map[1][home.row][home.column] == 'H' && (home.row == 14 && home.column == 1)) {
			home = (POSITION){ units_AT[i].home.row, units_AT[i].home.column + 1 };
			units_AT[i].home.row = home.row;
			units_AT[i].home.column = home.column;
		}
		diff = psub(home, curr);
	}

	//��Ȯ�� �����̽��� x
	if (map[0][dest.row][dest.column] == ' ') {
		units_AT[i].dest.row = home.row;
		units_AT[i].dest.column = home.column;
		units_AT[i].home.row = home.row;
		units_AT[i].home.column = home.column;
	}


	DIRECTION dir;
	if (abs(diff.row) >= abs(diff.column)) {
		dir = (diff.row >= 0) ? d_down : d_up;
	}
	else {
		dir = (diff.column >= 0) ? d_right : d_left;
	}
	POSITION next_pos = pmove(curr, dir);
	//���� ����
	if (next_pos.row == home.row && next_pos.column == home.column) {
		if (resource.spice + units_AT[i].get <= resource.spice_max) {
			resource.spice += units_AT[i].get;
			units_AT[i].get = 0;
		}
		else {
			resource.spice = resource.spice_max;
			units_AT[i].get = 0;
			strcpy_s(system_view[0], 58, system_view[1]);
			strcpy_s(system_view[1], 58, system_view[2]);
			strcpy_s(system_view[2], 58, system_view[3]);
			strcpy_s(system_view[3], 58, system_view[4]);
			strcpy_s(system_view[4], 58, system_view[5]);
			strcpy_s(system_view[5], 58, system_view[6]);
			snprintf(system_view[6], 58, text_system[14]);
			view_system(cursor);
		}
		if (map[0][dest.row][dest.column] == ' ') {
			strcpy_s(system_view[0], 58, system_view[1]);
			strcpy_s(system_view[1], 58, system_view[2]);
			strcpy_s(system_view[2], 58, system_view[3]);
			strcpy_s(system_view[3], 58, system_view[4]);
			strcpy_s(system_view[4], 58, system_view[5]);
			strcpy_s(system_view[5], 58, system_view[6]);
			snprintf(system_view[6], 58, text_system[10]);
			view_system(cursor);
		}
	}

	if ((units_AT[i].get ==0 && ('1' <= map[0][next_pos.row][next_pos.column] && map[0][next_pos.row][next_pos.column] <= '9'))&& map[1][next_pos.row][next_pos.column]== -1) return next_pos;
	else if (map[1][next_pos.row][next_pos.column] != -1) {
		if (dir == d_right || dir == d_left) dir = (diff.row >= 0) ? d_down : d_up;
		else dir = (diff.column >= 0) ? d_right : d_left;
		next_pos = pmove(curr, dir);
		return next_pos;
	}
	else if (map[0][next_pos.row][next_pos.column] != ' ') {
		if (dir == d_right || dir == d_left) dir = (diff.row >= 0) ? d_down : d_up;
		else dir = (diff.column >= 0) ? d_right : d_left;
		next_pos = pmove(curr, dir);
		return next_pos;
	}
	else if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
		1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
		map[0][next_pos.row][next_pos.column] == ' ' && \
		map[1][next_pos.row][next_pos.column] == -1) {
		return next_pos;
	}
	else return units_AT[i].pos;  // ���ڸ�
}

void harvesting(void) {
	for (int i = 0; i < 100; i++) {
		if (sys_clock >= units_AT[i].next_move_time && units_AT[i].next_move_time != 0) {
			if (units_AT[i].onoff == 1) {
				map[1][units_AT[i].pos.row][units_AT[i].pos.column] = -1;
				units_AT[i].pos = harvest_position(i);
				map[1][units_AT[i].pos.row][units_AT[i].pos.column] = units_AT[i].ch;
				if (map[0][units_AT[i].pos.row][units_AT[i].pos.column] != ' ') {
					strcpy_s(system_view[0], 58, system_view[1]);
					strcpy_s(system_view[1], 58, system_view[2]);
					strcpy_s(system_view[2], 58, system_view[3]);
					strcpy_s(system_view[3], 58, system_view[4]);
					strcpy_s(system_view[4], 58, system_view[5]);
					strcpy_s(system_view[5], 58, system_view[6]);
					snprintf(system_view[6], 58, text_system[9]);
					view_system(cursor);
					units_AT[i].next_move_time = sys_clock + 3000;
				}
				else
					units_AT[i].next_move_time = sys_clock + 500;
			}
		}
	}
}