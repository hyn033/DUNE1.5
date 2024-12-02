#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "common.h"
#include "io.h"
#include "display.h"
#include <string.h>

//하베스터 행동 구현
//+움직이기, 수확하기 구현


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
int click_start_timer = 0; //타이머 시작하여 시간을 잼
int double_click = 90;  //90이내의 반복 클릭시 대쉬하도록 하기 위함
int spice_time = 15000;  //샌드웜의 첫 스파이스 배출 시간
int order_on = 1;  //명령스위치 [ 0 : 본진명령 / 1 : 건설 명령 (이때 B를 누를 수 있음) / 2 : 건설 메뉴 (이때 P,G,D,B,S 선택가능
								//3 : plate 설치 가능 / 4 : dormitory 설치 가능 / 5 : garage 설치 가능 / 6 : barracks / 7: shelter / 8 : arena / 9 : factory / 10 : 하베스트 이동명령 ]
int move_on = 0;	// 0 : 비활성화 1 : 이동 2 : 수확
int m_harvest = 0;	// 움직일 하베스터를 지목할때
int h_harvest = 0;  // 수확할 하베스터 지목할때
int change_cursor = -1; //커서 변경 변수 [ 0 : 커서 4칸 변경 ]
int B_cnt = 0; //B키를 두번 입력할 시 barracks 건설
int buildings_cnt_AT = 0;
int buildings_cnt_HC = 0;
int units_cnt_AT = 0;  //AT유닛 갯수
int units_cnt_HC = 0;
bool timer_on = 0;  //타이머 키는 변수
CURSOR dash_cursor = { {0,0},{0,0} }; //대쉬를 했을 때 대쉬 후 전 위치를 저장 -> 맵 상 흔적을 남기지 않기 위해
KEY last_key=k_none;
CURSOR cursor = { { 1, 1 }, {1, 1} };
POSITION destination = { 0, 0 };  //유닛이 있을때 샌드웜의 목적지임
build buildings_AT[100] = { 0 };
build buildings_HC[100] = { 0 };
unit units_HC[100] = { 0 };
unit units_AT[100] = { 0 };

/* ================= game data =================== */
char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };   //map 정보
int map_color[MAP_HEIGHT][MAP_WIDTH] = { 0 };		//map 색 정보
char text_situ[30][50] = {
	"나의 [본진] 이다.", // text[0] 우리 본진
	"이 곳은 적의 ???이다.", // text[1] 적 본진
	"[사막지형] 이다.", //text[2] 사막지형
	"아무 것도 할 수 없어 보인다.", //text[3] 사막지형
	"[장판]이다.",  //text[4] 장판
	"[스파이스 매장지] 이다.", //text[5] 스파이스
	"스파이스 매장량 : ",  //text[6] 스파이스
	"딱딱한 [돌] 이다.",  //text[7] 돌
	"자세히 알 수는 없을 것 같다.", //text[8] 적 본진
	"건물을 지을 수 있다.", //text[9] 장판
	"[하베스터] 이다.", //text[10] 하베스터
	"[샌드웜] 이다.", //text[11] 하베스터
	"다가가지 않는 것이 좋아보인다.", //text[12] 하베스터
	"안락한 숙소이다.", //text[13] 숙소
	"인구 수가 10 증가한다.", //text[14] 숙소
	"넓은 창고이다.", //text[15] 창고
	"스파이스 저장량이 10 증가한다.", //text[16] 창고
	"보병을 생산할 수 있는 병영이다.", //text[17] 병영
	"생산비용 : 1", //text[18] 병영
	"프레멘을 생산할 수 있는 은신처이다.", //text[19] 은신처
	"생산비용 : 5", //text[20] 은신처
	"체력 : ", //text[21] 하베스터 체력
	"장판 건설 비용 : 1",  //text[22] 장판 건설 비용
	"숙소 건설 비용 : 2",  //text[23] 숙소 건설 비용
	"창고 건설 비용 : 4",  //text[24] 창고 건설 비용
	"병영 건설 비용 : 4",  //text[25] 병영 건설 비용
	"은신처 건설 비용 : 5",  //text[26] 은신처 건설 비용
};
char text_order[20][50] = {
	"H : 하베스터 생산", //text[0] 본진 명령어
	"H : 수확하기",		//text[1] 농부 명령어
	"M : 움직이기",		//text[2] 
	"B : 건설하기",		//text[3] 건설 명령어 ( 기본 설정 )
	"P : 장판 건설",	//text[4] 건설 명령어
	"D : 숙소 건설",	//text[5] 건설 명령어
	"G : 창고 건설",	//text[6] 건설 명령어
	"B : 병영 건설",	//text[7] 건설 명령어
	"S : 은신처 건설",	//text[8] 건설 명령어
	"Space : 건설하기",	//text[9] 건설 명령어
	"Space : 커서 이동 후 클릭",	//text[10] 이동 명령어
	"Space : 스파이스 매장지를 정해주세요", //text[11] 수확 명령
	"B : 보병 생산", //text[12] 보병 생산 명령
	"S : 특수유닛 생산", //text[12] 특수유닛 생산 명령
};
char text_system[30][50] = {
	"하베스트 생성!", //text[0] 하베스트 생성
	"스파이스가 부족합니다.", //text[1] 스파이스 부족
	"더이상 생성할 수 없습니다.", //text[2] 하베스트 자리 부족
	"샌드웜이 하베스트를 먹었습니다!", //text[3] 하베스트 먹힘
	"샌드웜이 스파이스를 배출했습니다!", //text[4] 샌드웜 스파이스 배출
	"샌드웜이 적 하베스트를 먹었습니다!", //text[5] 적 하베스트 먹힘
	"이 곳에는 건설할 수 없습니다.",	  //text[6] 위치 설치 안될때
	"건설이 완료되었습니다.",			  //text[7] 위치 설치 될때
	"인구가 너무 많습니다.",			//text[8] 인구수가 많을 때
	"수확 중......",			//text[9] 하베스트 수확 중
	"하베스트가 수확을 끝냈습니다!",			//text[10] 하베스트 복귀
	"하베스트가 출발합니다.",			//text[11] 하베스트 출발
	"하베스트가 이동했습니다.",			//text[12] 하베스트 이동
	"잘못된 목적지입니다.",			//text[13] 하베스트 목적지 이상
	"! 스파이스 저장 용량 부족 !",			//text[14] 스파이스 저장량이 초과할때

};
char system_view[7][58] = { 0 };
// extern
extern char backbuf[MAP_HEIGHT][MAP_WIDTH]; 
extern const POSITION message_pos; //message가 시작하는 위치(map아래)
extern const POSITION situation_pos; //situation이 시작하는 위치(map옆)
extern const POSITION order_pos; //order가 시작하는 위치(map대각선)

RESOURCE resource = {  //초기 하베스트는 최대 4로 지정함
	.spice = 20,
	.spice_max = 20,
	.population = 5,
	.population_max = 20  //4*5 = 20
};

OBJECT_SAMPLE sandW[2] = {  //샌드웜 2마리
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
		// loop 돌 때마다(즉, TICK==10ms마다) 키 입력 확인
		KEY key = get_key();
		if (click_start_timer > double_click && timer_on) { //타이머가 켜져있고 70초를 넘겼을 경우엔 한칸 움직이기
			cursor_move(ktod(last_key)); 
			timer_on = 0; 
			click_start_timer = 0; 
		}
		if (is_arrow_key(key)) { // 키 입력이 있으면 처리
			if (!timer_on) { //타이머가 꺼져있으면
				timer_on = 1;
				last_key = key;
			}
			else if (last_key == key) { //타이머가 켜져있고 키가 같을 경우
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
			// 방향키 외의 입력
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
		// 샘플 오브젝트 동작
		dest_sandwarm();
		sample_obj_move();
		harvesting();
		spice_making();
		// 화면 출력
		add_struct_map(); //구조체 내용을 map에 반영
		map_coloring();
		if(change_cursor == 0)
			display2(resource, map, map_color, cursor);
		else
			display(resource, map, map_color, cursor);
		Sleep(TICK);
		if (timer_on) //타이머가 켜져있을때만 카운트
			click_start_timer += 10;
		sys_clock += 10;
	}
}

/* ================= 함수 정의 =================== */
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
	// layer 0(map[0])에 지형 생성
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

	// layer 1(map[1])은 비워 두기(-1로 채움) // 유닛 추가
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			map[1][i][j] = -1;
		}
	}

	//본진
	map[0][1][58] = 'B';
	map[0][1][57] = 'B';
	map[0][2][58] = 'B';
	map[0][2][57] = 'B';
	map[0][15][1] = 'B';
	map[0][15][2] = 'B';
	map[0][16][1] = 'B';
	map[0][16][2] = 'B';
	//장판
	map[0][1][56] = 'P';
	map[0][1][55] = 'P';
	map[0][2][56] = 'P';
	map[0][2][55] = 'P';
	map[0][15][3] = 'P';
	map[0][15][4] = 'P';
	map[0][16][3] = 'P';
	map[0][16][4] = 'P';
	//돌
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

	//스파이스 매장지
	map[0][12][1] = '7';
	map[0][5][58] = '7';

	//하베스터
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

	//샌드웜
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

// (가능하다면) 지정한 방향으로 커서 이동
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
	else if (map[1][x][y] == 'W') {		//샌드웜
		printf("%s", text_situ[11]);
		POSITION pos = { 8, 5 };
		gotoxy(padd(situation_pos, pos));
		printf("%s", text_situ[12]);
	}
	else if (map[0][x][y] == ' ') {		//사막
		printf("%s", text_situ[2]);
		POSITION pos = { 8, 5 };
		gotoxy(padd(situation_pos, pos));
		printf("%s", text_situ[3]);
	}
	else if (map[0][x][y] == 'P') {		//장판
		printf("%s", text_situ[4]);
		POSITION pos = { 8, 5 };
		gotoxy(padd(situation_pos, pos));
		printf("%s", text_situ[9]);
	}
	else if (map[0][x][y] == 'D') {		//숙소
		printf("%s", text_situ[13]);
		POSITION pos = { 8, 5 };
		gotoxy(padd(situation_pos, pos));
		printf("%s", text_situ[14]);
	}
	else if (map[0][x][y] == 'G') {  //창고
		printf("%s", text_situ[15]);
		POSITION pos = { 8, 5 };
		gotoxy(padd(situation_pos, pos));
		printf("%s", text_situ[16]);
	}
	else if (map[0][x][y] == 'S') {  //은신처
		printf("%s", text_situ[19]);
		POSITION pos = { 8, 5 };
		gotoxy(padd(situation_pos, pos));
		printf("%s", text_situ[20]);
	}
	else if ('1'<= map[0][x][y] && map[0][x][y] <= '9') {		//스파이스 매장지
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
		//유닛이 없을 때 샌드웜의 움직임
		int dir = (rand() % 3) + 1; 
		POSITION next_pos = pmove(curr, dir); 
		if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
			1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
			map[0][next_pos.row][next_pos.column] == ' ') return next_pos;
		else return sandW[i].pos;  // 제자리
	}
	else {
		//유닛이 있을 때 샌드웜의 움직임
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
		else return sandW[i].pos;  // 제자리
	}
}

void sample_obj_move(void) {
	for (int i = 0; i < 2; i++) {
		//움직일 시간이면 움직이기 아니면 넘기기
		if (sys_clock >= sandW[i].next_move_time) {
		// 오브젝트(건물, 유닛 등)은 layer1(map[1])에 저장
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
	//초기 목적지 정함
	for (int x = 0; x < 2; x++) {
		for (int i = 0; i < MAP_HEIGHT; i++) {
			for (int j = 0; j < MAP_WIDTH; j++) {
				if (map[1][i][j] == 'H') {
					//초기 목적지 ( 거리에 상관하지 않고 가장 마지막으로 탐색된 유닛임)
					sandW[x].dest.row = i;
					sandW[x].dest.column = j;
				}
			}
		}
	}
	//초기 목적지 보다 가까운 곳을 재탐색
	for (int x = 0; x < 2; x++) {
		for (int i = 0; i < MAP_HEIGHT; i++) {
			for (int j = 0; j < MAP_WIDTH; j++) {
				if (map[1][i][j] == 'H') {
					//새 목적지 거리 탐색
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
			// 아직 시간이 안 됐음
			return;
		}
		// 오브젝트(건물, 유닛 등)은 layer1(map[1])에 저장
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
	//본진 위에서 실행하는지 확인
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
			if (map[1][14][1] != 'H' || map[1][14][2] != 'H' || map[1][14][3] != 'H' || map[1][14][4] != 'H') { //하베스트 자리는 최대 4개
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
	//order_on 가 3일때 실행하도록
	if (order_on == 3 && resource.spice >= 1) {
		strcpy_s(system_view[0], 58, system_view[1]);
		strcpy_s(system_view[1], 58, system_view[2]);
		strcpy_s(system_view[2], 58, system_view[3]);
		strcpy_s(system_view[3], 58, system_view[4]);
		strcpy_s(system_view[4], 58, system_view[5]);
		strcpy_s(system_view[5], 58, system_view[6]);
		int x = cursor.current.row;
		int y = cursor.current.column;
		//커서 기준 4칸에 아무 건물이 없을때만 설치하도록
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
	//order_on 가 4일때 실행하도록
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
	else return;  //order가 4~7이 아니면 실행 x

	strcpy_s(system_view[0], 58, system_view[1]);
	strcpy_s(system_view[1], 58, system_view[2]);
	strcpy_s(system_view[2], 58, system_view[3]);
	strcpy_s(system_view[3], 58, system_view[4]);
	strcpy_s(system_view[4], 58, system_view[5]);
	strcpy_s(system_view[5], 58, system_view[6]);
	int x = cursor.current.row;
	int y = cursor.current.column;
	//커서 기준 4칸에 P가 있을때
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
	//order_on 가 4일때 실행하도록
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
	else return;  //order가 8~9이 아니면 실행 x
	int x = cursor.current.row;
	int y = cursor.current.column;
	//커서 기준 4칸에 P가 있을때
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

	//유닛 수확 종료 조건
	if ((units_AT[i].home.row == units_AT[i].dest.row && units_AT[i].home.column == units_AT[i].dest.column) && curr.row == home.row && curr.column == home.column) {
		units_AT[i].next_move_time = 0;
		return units_AT[i].pos;
	}

	//유닛이 수확하기 전
	if (units_AT[i].get == 0) {
		diff = psub(dest, curr);
		//목적지에 도착했을 때
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
	//유닛이 수확한 후
	else if (units_AT[i].get != 0) {
		if (map[1][home.row][home.column] == 'H' && (home.row == 14 && home.column == 1)) {
			home = (POSITION){ units_AT[i].home.row, units_AT[i].home.column + 1 };
			units_AT[i].home.row = home.row;
			units_AT[i].home.column = home.column;
		}
		diff = psub(home, curr);
	}

	//수확할 스파이스가 x
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
	//집에 도착
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
	else return units_AT[i].pos;  // 제자리
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