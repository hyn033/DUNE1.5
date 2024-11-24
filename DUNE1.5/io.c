/*
* raw(?) I/O
*/
#include "io.h"

void gotoxy(POSITION pos) {
	COORD coord = { pos.column, pos.row }; // 행, 열 반대로 전달
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void set_color(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void printc(POSITION pos, char ch, int color) {
	if (color >= 0) {
		set_color(color);
	}
	gotoxy(pos);
	printf("%c", ch);
}

KEY get_key(void) {
	if (!_kbhit()) {  // 입력된 키가 있는지 확인
		return k_none;
	}

	int byte = _getch();    // 입력된 키를 전달 받기
	switch (byte) {
	case 's': return k_S;	// 's'를 누르면 명령 실행
	case 'S': return k_S;	// 'S'를 누르면 명령 실행 
	case 'g': return k_G;	// 'g'를 누르면 명령 실행
	case 'G': return k_G;	// 'G'를 누르면 명령 실행 
	case 'D': return k_D;	// 'D'를 누르면 명령 실행
	case 'd': return k_D;	// 'd'를 누르면 명령 실행 
	case 'p': return k_P;	// 'P'를 누르면 명령 실행
	case 'P': return k_P;	// 'p'를 누르면 명령 실행 
	case 'b': return k_B;	// 'b'를 누르면 명령 실행
	case 'B': return k_B;	// 'B'를 누르면 명령 실행 
	case 'h': return k_H;	// 'h'를 누르면 명령 실행
	case 'H': return k_H;	// 'H'를 누르면 명령 실행 
	case 'q': return k_quit;  // 'q'를 누르면 종료
	case 32: return k_space;  //'스페이스바를 누르면 k_space 전달
	case 27: return k_esc;	 //esc키 누르면 k_esc전달
	case 224:
		byte = _getch();  // MSB 224가 입력 되면 1바이트 더 전달 받기
		switch (byte) {
		case 72: return k_up;
		case 75: return k_left;
		case 77: return k_right;
		case 80: return k_down;
		default: return k_undef;
		}
	default: return k_undef;
	}
}