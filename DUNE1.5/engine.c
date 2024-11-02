#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "common.h"
#include "io.h"
#include "display.h"

// 중립 유닛인 샌드웜 움직임 구현


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
int click_start_timer = 0; //타이머 시작
int double_click = 90;
bool timer_on = 0;
CURSOR dash_cursor = { {0,0},{0,0} }; //대쉬를 했을 때 대쉬 후 전 위치를 저장 -> 맵 상 흔적을 남기지 않기 위해
KEY last_key=k_none;
CURSOR cursor = { { 1, 1 }, {1, 1} };
POSITION destination = { 0, 0 };  //유닛이 있을때 샌드웜의 목적지임

/* ================= game data =================== */
char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };   //map 정보
int map_color[MAP_HEIGHT][MAP_WIDTH] = { 0 };		//map 색 정보
char text_situ[13][50] = {
	"나의 [본진] 이다.", // text[0] 우리 본진
	"이 곳은 적의 ???이다.", // text[1] 적 본진
	"[사막지형] 이다.", //text[2] 사막지형
	"아무 것도 할 수 없어 보인다.", //text[3] 사막지형
	"[장판]이다.",  //text[4] 장판
	"[스파이스 매장지] 이다.", //text[5] 스파이스
	"스파이스 매장량 : 8 ",  //text[6] 스파이스
	"딱딱한 [돌] 이다.",  //text[7] 돌
	"자세히 알 수는 없을 것 같다.", //text[8] 적 본진
	"건물을 지을 수 있다.", //text[9] 장판
	"[하베스터] 이다.", //text[10] 하베스터
	"[샌드웜] 이다.", //text[11] 하베스터
	"다가가지 않는 것이 좋아보인다.." //text[12] 하베스터
};
char text_order[10][50] = {
	"H : 하베스터 생산", //text[0] 본진 명령어
	"H : 수확하기",		//text[1] 농부 명령어
	"M : 움직이기"		//text[2] 
};
// extern
extern char backbuf[MAP_HEIGHT][MAP_WIDTH]; 
extern const POSITION message_pos; //message가 시작하는 위치(map아래)
extern const POSITION situation_pos; //situation이 시작하는 위치(map옆)
extern const POSITION order_pos; //order가 시작하는 위치(map대각선)


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
		// 샘플 오브젝트 동작
		dest_sandwarm();
		sample_obj_move();
		// 화면 출력
		map_coloring();
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
	map[0][12][1] = 'S';
	map[0][5][58] = 'S';

	//하베스터
	map[1][14][1] = 'H';
	map[1][3][58] = 'H';

	//샌드웜 
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

// (가능하다면) 지정한 방향으로 커서 이동
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
		//유닛이 없을 때 샌드웜의 움직임
		int key = (rand() % 5) + 1;
		POSITION next_pos = pmove(curr, key);
		if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
			1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
			map[1][next_pos.row][next_pos.column] < 0 && \
			map[0][next_pos.row][next_pos.column] != 'R' && \
			map[0][next_pos.row][next_pos.column] != 'B' && \
			map[0][next_pos.row][next_pos.column] != 'P') return next_pos;
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
		else return sandW[i].pos;  // 제자리
	}
}

void sample_obj_move(void) {
	for (int i = 0; i < 2; i++) {
		if (sys_clock <= sandW[i].next_move_time) {
			// 아직 시간이 안 됐음
			return;
		}

		// 오브젝트(건물, 유닛 등)은 layer1(map[1])에 저장
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
					//초기 목적지 ( 거리에 상관하지 않고 가장 마지막으로 탐색된 유닛임)
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
