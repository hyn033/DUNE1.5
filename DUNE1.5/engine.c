#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "common.h"
#include "io.h"
#include "display.h"

// 건물&유닛, 커서 색 추가

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
int click_start_timer = 0; //타이머 시작
int double_click = 90;
bool timer_on = 0;
CURSOR dash_cursor = { {0,0},{0,0} }; //대쉬를 했을 때 대쉬 후 전 위치를 저장 -> 맵 상 흔적을 남기지 않기 위해
KEY last_key=k_none;
CURSOR cursor = { { 1, 1 }, {1, 1} };


/* ================= game data =================== */
char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };   //map 정보
char message[BOX_HEIGHT][MAP_WIDTH] = { 0 };		//메세지 창 
char situation[MAP_HEIGHT][BOX_WIDTH] = { 0 };		//상태창 
char order[BOX_HEIGHT][BOX_WIDTH] = { 0 };			//명령창
int map_color[MAP_HEIGHT][MAP_WIDTH] = { 0 };		//map 색 정보

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
			case k_none:
			case k_undef:
			default: break;
			}
		}
		// 샘플 오브젝트 동작
		sample_obj_move();
		// 화면 출력
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
	//샌드웜 			
	map[1][3][10] = 'W';
	map[1][13][40] = 'W';
	//하베스터
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
	// layer 0(map[0])에 지형 생성
	for (int i = 0; i < MAP_WIDTH; i++) {
		message[0][i] = '*';
		message[BOX_HEIGHT - 1][i] = '*';
	}//박스의 윗, 아랫부분 채우기

	for (int i = 1; i < BOX_HEIGHT - 1; i++) {
		message[i][0] = '*';
		message[i][MAP_WIDTH - 1] = '*';
		for (int j = 1; j < MAP_WIDTH - 1; j++) {
			message[i][j] = ' ';
		}//가운데 공백 채우기
	}//박스의 양쪽 채우기
}

void situation_making(void) {
	// layer 0(map[0])에 지형 생성
	for (int i = 0; i < BOX_WIDTH; i++) {
		situation[0][i] = '*';
		situation[MAP_HEIGHT - 1][i] = '*';
	}//박스의 윗, 아랫부분 채우기

	for (int i = 1; i < MAP_HEIGHT - 1; i++) {
		situation[i][0] = '*';
		situation[i][BOX_WIDTH - 1] = '*';
		for (int j = 1; j < BOX_WIDTH - 1; j++) {
			situation[i][j] = ' ';
		}//가운데 공백 채우기
	}//박스의 양쪽 채우기
}

void order_making(void) {
	// layer 0(map[0])에 지형 생성
	for (int i = 0; i < BOX_WIDTH; i++) {
		order[0][i] = '*';
		order[BOX_HEIGHT - 1][i] = '*';
	}//박스의 윗, 아랫부분 채우기

	for (int i = 1; i < BOX_HEIGHT - 1; i++) {
		order[i][0] = '*';
		order[i][BOX_WIDTH - 1] = '*';
		for (int j = 1; j < BOX_WIDTH - 1; j++) {
			order[i][j] = ' ';
		}//가운데 공백 채우기
	}//박스의 양쪽 채우기
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

/* ================= sample object movement =================== */
POSITION sample_obj_next_position(void) {
	// 현재 위치와 목적지를 비교해서 이동 방향 결정	
	POSITION diff = psub(obj.dest, obj.pos);
	DIRECTION dir;

	// 목적지 도착. 지금은 단순히 원래 자리로 왕복
	if (diff.row == 0 && diff.column == 0) {
		if (obj.dest.row == 1 && obj.dest.column == 1) {
			// topleft --> bottomright로 목적지 설정
			POSITION new_dest = { MAP_HEIGHT - 2, MAP_WIDTH - 2 };
			obj.dest = new_dest;
		}
		else {
			// bottomright --> topleft로 목적지 설정
			POSITION new_dest = { 1, 1 };
			obj.dest = new_dest;
		}
		return obj.pos;
	}

	// 가로축, 세로축 거리를 비교해서 더 먼 쪽 축으로 이동
	if (abs(diff.row) >= abs(diff.column)) {
		dir = (diff.row >= 0) ? d_down : d_up;
	}
	else {
		dir = (diff.column >= 0) ? d_right : d_left;
	}

	// validation check
	// next_pos가 맵을 벗어나지 않고, (지금은 없지만)장애물에 부딪히지 않으면 다음 위치로 이동
	// 지금은 충돌 시 아무것도 안 하는데, 나중에는 장애물을 피해가거나 적과 전투를 하거나... 등등
	POSITION next_pos = pmove(obj.pos, dir);
	if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
		1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
		map[1][next_pos.row][next_pos.column] < 0) {

		return next_pos;
	}
	else {
		return obj.pos;  // 제자리
	}
}

void sample_obj_move(void) {
	if (sys_clock <= obj.next_move_time) {
		// 아직 시간이 안 됐음
		return;
	}

	// 오브젝트(건물, 유닛 등)은 layer1(map[1])에 저장
	map[1][obj.pos.row][obj.pos.column] = -1;
	obj.pos = sample_obj_next_position();
	map[1][obj.pos.row][obj.pos.column] = obj.repr;

	obj.next_move_time = sys_clock + obj.speed;
}