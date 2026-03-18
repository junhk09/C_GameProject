#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include <mmsystem.h>
#include <process.h>
#pragma comment(lib,"winmm.lib")
// 색상 정의
#define BLACK	0
#define BLUE1	1
#define GREEN1	2
#define CYAN1	3
#define RED1	4
#define MAGENTA1 5
#define YELLOW1	6
#define GRAY1	7
#define GRAY2	8
#define BLUE2	9
#define GREEN2	10
#define CYAN2	11
#define RED2	12
#define MAGENTA2 13
#define YELLOW2	14
#define WHITE	15
#define SELECT 32



void Sleep2(int delay) { // delay(msec) 동안 멈추기
	clock_t start;
	int duration = 0;
	start = clock();
	do {
		duration = clock() - start;
	} while (duration <= delay);
}

void removeCursor(void) { // 커서를 안보이게 한다

	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void gotoxy(int x, int y) //내가 원하는 위치로 커서 이동
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API 함수입니다. 이건 알필요 없어요
}

void cls(int bg_color, int text_color)
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);

}
void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}


int map[30][50] = {
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,2,0,0,2,0,0,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,2,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,2,0,0,2,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,2,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,2,0,0,0,0,2,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,



};
int end[30][50] = {
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,2,0,0,0,2,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,2,0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,2,2,2,2,2,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,2,0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,2,0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,2,0,0,0,2,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,2,0,0,0,0,2,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,2,2,0,0,0,2,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,2,0,2,0,0,2,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,2,0,0,2,0,2,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,2,0,0,0,2,2,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,2,0,0,0,0,2,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,



};
void drawui(int hp, int mp, int level, int hpp, int mpp, int dmg) {
	gotoxy(0, 0);
	printf("[ HP ] %d/100 ", hp);
	gotoxy(0, 1);
	printf("[ MP ] %d/50 ", mp);
	gotoxy(0, 2);
	printf("[ Lv %d ] ", level);
	gotoxy(0, 3);
	printf("[ DMG : %d ] ", dmg);
	gotoxy(0, 4);
	printf("[ HP 포션: %d ]", hpp);
	gotoxy(0, 5);
	printf("[ MP 포션: %d ]", mpp);

}
void skillui() {
	gotoxy(40, 4);
	printf("공격 : A");
	gotoxy(40, 5);
	printf("스킬 : Z");
	gotoxy(40, 6);
	printf("점프 : Space");
	gotoxy(40, 7);
	printf("포션 : H,M");
	gotoxy(40, 8);
	printf("이동 : 방향키");

}
struct player {
	int hp;
	int mp;
	int hpp; // 포션
	int mpp;
	int x;
	int y;
	int level;
}player;
struct monster
{
	int hp;
	int x;
	int y;

}monster;

void attack2(int x, int y) {
	gotoxy(x, y);
	printf("    O   \n");
	gotoxy(x, y + 1);
	printf("  <-lo  \n");
	gotoxy(x, y + 2);
	printf("  / ㅣ \n");
}
void attack(int x, int y) {
	gotoxy(x, y);
	printf("   O   \n");
	gotoxy(x, y + 1);
	printf("  olo->  \n");
	gotoxy(x, y + 2);
	printf("  ㅣ＼  \n");

}
void drawuser(int x, int y) {
	gotoxy(x, y);
	printf("   O   \n");
	gotoxy(x, y + 1);
	printf(" ⊂l⊃ \n");
	gotoxy(x, y + 2);
	printf("  l l  \n");

}


void drawmonster() {

	if (monster.hp > 0) {
		textcolor(RED1, WHITE);
		gotoxy(monster.x, monster.y);
		printf("  %d  ", monster.hp);
		gotoxy(monster.x, monster.y + 1);
		printf("  ⌒  ");
		gotoxy(monster.x, monster.y + 2);
		printf("  @@  ");


		textcolor(BLACK, WHITE);

	}


}
void bossmonster() {

	if (monster.hp > 0) {
		textcolor(RED1, WHITE);
		gotoxy(monster.x, monster.y);
		printf("  %d  ", monster.hp);
		gotoxy(monster.x, monster.y + 1);
		printf("  @@@@ ");
		gotoxy(monster.x, monster.y + 2);
		printf(" @@@@@@ ");


		textcolor(BLACK, WHITE);

	}


}






void skill() {

	gotoxy(player.x, player.y);
	printf("   O   \n");
	gotoxy(player.x, player.y + 1);
	printf(" ∴l∴ \n");
	gotoxy(player.x, player.y + 2);
	printf("  l l \n");



}
void potal(int x, int y) {
	textcolor(1, 15);
	gotoxy(x, y);
	printf("∥∥");
	gotoxy(x, y + 1);
	printf("∥∥");
	gotoxy(x, y + 2);
	printf("∥∥");

	textcolor(0, 15);
}
void mapbase(int x, int y) {
	textcolor(YELLOW1, WHITE);
	gotoxy(x, y);
	printf("===================================================================================================");
	gotoxy(x, y + 1);
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
	gotoxy(x, y + 2);
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
	gotoxy(x, y + 3);
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
	gotoxy(x, y + 4);
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
	gotoxy(x, y + 5);
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
	textcolor(BLACK, WHITE);
}
void map1(int x, int y) {
	gotoxy(x, y);
	printf("================");

}
void tutorial(int x, int y) {
	gotoxy(x, y);
	printf("Tutorial");
}
void stage1(int x, int y) {
	gotoxy(x, y);
	printf("Stage 1");
}
void stage2(int x, int y) {
	gotoxy(x, y);
	printf("Stage 2");
}
void stage3(int x, int y) {
	gotoxy(x, y);
	printf("Boss Stage");
}
void main(int argc, char* argv[])
{

	// bgm 재생
	PlaySound(TEXT("thefinalWar.wav"), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);



	struct player;
	struct monster;
	int i = 0, x, y;
	int menu = 0;
	int gravity = 1;
	int bind = 0;

	int dmg = 5;

	int jump = 0;
	int height = 7;

	removeCursor();

	textcolor(15, 0);

	for (y = 0; y < 30; y++) {

		gotoxy(0, y);
		for (x = 0; x < 50; x++) {
			if (map[y][x] == 1)
				printf("▒ ");
			else if (map[y][x] == 2) {
				i = rand() % 15;
				textcolor(i, 0);
				printf("■");
				textcolor(15, 0);
			}

			else {

				printf("  ");

			}

		}

	}

	gotoxy(30, 20); printf("게임 시작");

	gotoxy(50, 20); printf("게임 종료");

	gotoxy(30, 20);
	while (1) {
		if (GetAsyncKeyState(VK_LEFT))

			if (menu == 0) menu = 1;

			else menu -= 1;

		else if (GetAsyncKeyState(VK_RIGHT))

			if (menu == 1) menu = 0;

			else menu += 1;

		else if (GetAsyncKeyState(VK_RETURN))

			break;

		switch (menu) {

		case 0:

			textcolor(11, 0);

			gotoxy(30, 20); printf("게임 시작");

			textcolor(12, 0);

			gotoxy(50, 20); printf("게임 종료");

			break;

		case 1:
			gotoxy(30, 20); printf("게임 시작");

			textcolor(11, 0);

			gotoxy(50, 20); printf("게임 종료");

			textcolor(12, 0);

			break;

		}

		Sleep(200);

	}

	if (menu == 0) cls(15, 0);

	else if (menu == 1) return;
	player.hp = 100;
	player.mp = 50;
	/*player.exp = 0;*/
	player.level = 1;
	player.x = 0;
	player.y = 20;
	monster.x = 40;
	monster.y = 20;
	monster.hp = 100;
	player.hpp = 50;
	player.mpp = 50;
	// 튜토리얼
	while (1) {

		mapbase(0, 23);


		skillui();

		drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
		map1(0, 18);

		tutorial(40, 0);

		drawuser(player.x, player.y);
		bossmonster();



		if (kbhit()) {

			int key = getch();

			switch (key) {

			case 224:

				key = getch();

				switch (key) {


				case 75: // 왼쪽

					if (player.x > 0)
						player.x--;
					else
						player.x = 1;

					if (player.x > 11 && player.y == 14) {
						player.y = 20;
						system("cls");
					}

					break;

				case 77:  // 오른쪽
					if (player.x < 85) {
						player.x++;
					}

					if (player.x > 11 && player.y == 14) {
						player.y = 20;
						system("cls");
					}

					break;


				}

				break;

			case 32:  // 스페이스바


				if (player.y == 20 && !jump) {

					jump = 1;


					for (int i = 0; i < height; i++) {
						if (monster.hp <= 0) {
							gotoxy(40, 10);
							printf("Tutorial Clear!!");
							gotoxy(40, 11);
							printf("포탈을 타고 다음맵으로 넘어가세요!");

						}
						if (bind == 1 && monster.hp > 0) {
							gotoxy(40, 10);
							printf("Bind!");
						}
						if (player.y == 14 && player.x >= 0 && player.x <= 11) {
							map1(0, 18);
						}


						player.y--;

						system("cls");
						mapbase(0, 23);

						drawuser(player.x, player.y);

						drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
						tutorial(40, 0);
						bossmonster();
						skillui();

						Sleep(30);




					}

					for (int i = 0; i < height; i++) {
						if (monster.hp <= 0) {
							gotoxy(40, 10);
							printf("Tutorial Clear!!");
							gotoxy(40, 11);
							printf("포탈을 타고 다음맵으로 넘어가세요!");

						}
						if (bind == 1 && monster.hp > 0) {
							gotoxy(40, 10);
							printf("Bind!");
						}
						if (player.y == 14 && player.x >= 0 && player.x <= 11) {
							map1(0, 18);
							break;
						}


						player.y++;
						system("cls");
						mapbase(0, 23);
						tutorial(40, 0);

						drawuser(player.x, player.y);
						drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
						skillui();
						bossmonster();
						Sleep(30);





					}

					jump = 0;


				}
				break;


			case 'm':

				drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
				if (player.mpp > 0) {
					player.mp = 50;
					player.mpp -= 1;
				}
				break;

			case 'z':
				skill();
				Sleep(100);

				player.mp -= 20;
				if (monster.hp > 0) {
					bind = 1;
					gotoxy(40, 10);
					printf("Bind!");
				}
				break;
			case 'h':
				drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
				if (player.hpp > 0) {
					player.hpp -= 1;
					player.hp = 100;
				}
				break;

			case 'a':
				if (player.mp > 0 && player.x > monster.x) {
					attack2(player.x, player.y);
					player.mp -= 2;
					drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
					Sleep(100);
				}
				if (player.mp > 0 && player.x < monster.x) {
					attack(player.x, player.y);
					player.mp -= 2;
					drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
					Sleep(100);
				}
				if (player.mp <= 0) break;
				if (player.mp != 0 && (player.x + 6 == monster.x || player.x + 5 == monster.x || player.x + 4 == monster.x || player.x + 3 == monster.x || player.x + 2 == monster.x || player.x + 1 == monster.x) || (player.x - 4 == monster.x || player.x - 3 == monster.x || player.x - 2 == monster.x || player.x - 5 == monster.x || player.x - 1 == monster.x))
				{
					monster.hp -= 7;

				}
				if (monster.hp <= 0) {
					system("cls");
					mapbase(0, 23);
					map1(0, 18);
					drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
					drawuser(player.x, player.y);
					potal(50, 20);
					skillui();
					gotoxy(40, 10);
					printf("Tutorial Clear!!");
					gotoxy(40, 11);
					printf("포탈을 타고 다음맵으로 넘어가세요!");
					break;


				}
			}

			Sleep(30);

			if (monster.hp > 0 && player.y == monster.y && player.x == monster.x) {
				player.hp -= 20;
				system("cls");
				Sleep(50);
			}

			if (player.hp <= 0) return;
		}
		if (player.x == 50 && monster.hp <= 0) {

			break;
		}


	}
	system("cls");


	player.hp = 100;
	player.mp = 50;
	/*player.exp = 0;*/
	player.level = 1;
	player.x = 0;
	player.y = 20;
	monster.x = 40;
	monster.y = 20;
	monster.hp = 100;
	player.hpp = 5;
	player.mpp = 5;







	// map1

	while (1) {

		mapbase(0, 23);


		//map1(0, 18); //16개

		drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);

		potal(80, 20);
		stage1(40, 0);

		drawuser(player.x, player.y);
		drawmonster();



		if (kbhit()) {

			int key = getch();

			switch (key) {

			case 224:

				key = getch();

				switch (key) {


				case 75: // 왼쪽

					if (player.x > 0)
						player.x--;
					else
						player.x = 1;
					if (player.x > monster.x && bind != 1 && monster.y == player.y)
						monster.x += 1;
					if (player.x < monster.x && bind != 1 && monster.y == player.y)
						monster.x -= 1;
					/*if (player.x > 11 && player.y == 14) {
						player.y = 20;
						system("cls");
					}*/

					break;

				case 77:  // 오른쪽
					if (player.x < 85) {
						player.x++;
					}
					if (player.x > monster.x && bind != 1 && monster.y == player.y)
						monster.x += 1;
					if (player.x < monster.x && bind != 1 && monster.y == player.y)
						monster.x -= 1;
					/*if (player.x > 11 && player.y == 14) {
						player.y = 20;
						system("cls");
					}*/

					break;


				}

				break;

			case 32:  // 스페이스바


				if (player.y == 20 && !jump) {

					jump = 1;


					for (int i = 0; i < height; i++) {
						if (monster.hp <= 0) {
							gotoxy(40, 10);
							printf("Stage1 Clear!!");
							gotoxy(40, 11);
							printf("포탈을 타고 다음맵으로 넘어가세요!");
							gotoxy(40, 12);
							printf("Level up!!");
						}
						if (bind == 1 && monster.hp > 0) {
							gotoxy(40, 10);
							printf("Bind!");
						}
						/*if (player.y == 14 && player.x >= 0 && player.x <= 11) {


							(0, 18);
						}*/


						player.y--;

						system("cls");
						mapbase(0, 23);
						potal(80, 20);
						drawuser(player.x, player.y);

						drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
						stage1(40, 0);
						drawmonster();


						Sleep(30);




					}

					for (int i = 0; i < height; i++) {
						if (monster.hp <= 0) {
							gotoxy(40, 10);
							printf("Stage1 Clear!!");
							gotoxy(40, 11);
							printf("포탈을 타고 다음맵으로 넘어가세요!");
							gotoxy(40, 12);
							printf("Level up!!");
						}
						if (bind == 1 && monster.hp > 0) {
							gotoxy(40, 10);
							printf("Bind!");
						}
						/*if (player.y == 14 && player.x >= 0 && player.x <= 11) {
							map1(0, 18);
							break;
						}*/


						player.y++;
						system("cls");
						mapbase(0, 23);
						stage1(40, 0);
						potal(80, 20);
						drawuser(player.x, player.y);
						drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);

						drawmonster();
						Sleep(30);





					}

					jump = 0;


				}
				break;


			case 'm':

				drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
				if (player.mpp > 0) {
					player.mp = 50;
					player.mpp -= 1;
				}
				break;

			case 'z':
				skill();
				Sleep(100);

				player.mp -= 20;
				if (monster.hp > 0) {
					bind = 1;
					gotoxy(40, 10);
					printf("Bind!");
				}
				break;
			case 'h':
				drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
				if (player.hpp > 0) {
					player.hpp -= 1;
					player.hp = 100;
				}
				break;

			case 'a':
				if (player.mp > 0 && player.x > monster.x) {
					attack2(player.x, player.y);
					player.mp -= 2;
					drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
					Sleep(100);
				}
				if (player.mp > 0 && player.x < monster.x) {
					attack(player.x, player.y);
					player.mp -= 2;
					drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
					Sleep(100);
				}
				if (player.mp <= 0) break;
				if (player.mp != 0 && (player.x + 6 == monster.x || player.x + 5 == monster.x || player.x + 4 == monster.x || player.x + 3 == monster.x || player.x + 2 == monster.x) || (player.x - 4 == monster.x || player.x - 3 == monster.x || player.x - 2 == monster.x || player.x - 5 == monster.x))
				{
					monster.hp -= 5;
					if (bind != 1 && monster.x < 70 && player.x < monster.x && player.mp != 0)
						monster.x += 2;
					if (bind != 1 && monster.x<70 && player.x>monster.x && player.mp != 0)
						monster.x -= 2;
				}
				if (monster.hp <= 0) {
					system("cls");
					mapbase(0, 23);
					/*map1(0, 18);*/
					drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
					potal(80, 20);
					stage1(40, 0);
					drawuser(player.x, player.y);
					gotoxy(40, 10);
					printf("Stage1 Clear!!");
					gotoxy(40, 11);
					printf("포탈을 타고 다음맵으로 넘어가세요!");
					gotoxy(40, 12);
					printf("Level up!!");
					dmg = 6;
				}
			}
			/*if (!jump && player.y < 20) {

				player.y += gravity;

			}*/
			Sleep(30);

			if (player.x == monster.x && monster.hp > 0 && player.y == monster.y) {
				player.hp -= 20;
				system("cls");
				Sleep(50);
			}

			if (player.hp <= 0) return;
			if (monster.hp <= 0) {
				player.level = 2;

			}



		}
		if (player.x == 80 && monster.hp <= 0) {
			break;
		}


	}
	system("cls");
	player.x = 0;
	player.y = 20;
	monster.x = 40;
	monster.y = 20;
	monster.hp = 100;
	bind = 0;
	// ------------------------------------------------------------------------------------------map2------------------------------------------------------------
	while (1) {

		mapbase(0, 23);


		//map1(0, 18); //16개

		drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);

		potal(80, 20);
		stage2(40, 0);

		drawuser(player.x, player.y);
		drawmonster();



		if (kbhit()) {

			int key = getch();

			switch (key) {

			case 224:

				key = getch();

				switch (key) {


				case 75: // 왼쪽

					if (player.x > 0)
						player.x--;
					else
						player.x = 1;
					if (player.x > monster.x && bind != 1 && monster.y == player.y)
						monster.x += 1;
					if (player.x < monster.x && bind != 1 && monster.y == player.y)
						monster.x -= 1;
					/*if (player.x > 11 && player.y == 14) {
						player.y = 20;
						system("cls");
					}*/

					break;

				case 77:  // 오른쪽
					if (player.x < 85) {
						player.x++;
					}
					if (player.x > monster.x && bind != 1 && monster.y == player.y)
						monster.x += 1;
					if (player.x < monster.x && bind != 1 && monster.y == player.y)
						monster.x -= 1;
					/*if (player.x > 11 && player.y == 14) {
						player.y = 20;
						system("cls");
					}*/

					break;


				}

				break;

			case 32:  // 스페이스바


				if (player.y == 20 && !jump) {

					jump = 1;


					for (int i = 0; i < height; i++) {
						if (monster.hp <= 0) {
							gotoxy(40, 10);
							printf("Stage1 Clear!!");
							gotoxy(40, 11);
							printf("포탈을 타고 다음맵으로 넘어가세요!");
							gotoxy(40, 12);
							printf("Level up!!");
						}
						if (bind == 1 && monster.hp > 0) {
							gotoxy(40, 10);
							printf("Bind!");
						}
						/*if (player.y == 14 && player.x >= 0 && player.x <= 11) {


							(0, 18);
						}*/


						player.y--;

						system("cls");
						mapbase(0, 23);
						potal(80, 20);
						drawuser(player.x, player.y);

						drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
						stage2(40, 0);
						drawmonster();


						Sleep(30);




					}

					for (int i = 0; i < height; i++) {
						if (monster.hp <= 0) {
							gotoxy(40, 10);
							printf("Stage1 Clear!!");
							gotoxy(40, 11);
							printf("포탈을 타고 다음맵으로 넘어가세요!");
							gotoxy(40, 12);
							printf("Level up!!");
						}
						if (bind == 1 && monster.hp > 0) {
							gotoxy(40, 10);
							printf("Bind!");
						}
						/*if (player.y == 14 && player.x >= 0 && player.x <= 11) {
							map1(0, 18);
							break;
						}*/


						player.y++;
						system("cls");
						mapbase(0, 23);
						stage2(40, 0);
						potal(80, 20);
						drawuser(player.x, player.y);
						drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);

						drawmonster();
						Sleep(30);





					}

					jump = 0;


				}
				break;


			case 'm':

				drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
				if (player.mpp > 0) {
					player.mp = 50;
					player.mpp -= 1;
				}
				break;

			case 'z':
				skill();
				Sleep(100);

				player.mp -= 20;
				if (monster.hp > 0) {
					bind = 1;
					gotoxy(40, 10);
					printf("Bind!");
				}
				break;
			case 'h':
				drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
				if (player.hpp > 0) {
					player.hpp -= 1;
					player.hp = 100;
				}
				break;

			case 'a':
				if (player.mp > 0 && player.x > monster.x) {
					attack2(player.x, player.y);
					player.mp -= 2;
					drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
					Sleep(100);
				}
				if (player.mp > 0 && player.x < monster.x) {
					attack(player.x, player.y);
					player.mp -= 2;
					drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
					Sleep(100);
				}
				if (player.mp <= 0) break;
				if (player.mp != 0 && (player.x + 6 == monster.x || player.x + 5 == monster.x || player.x + 4 == monster.x || player.x + 3 == monster.x || player.x + 2 == monster.x) || (player.x - 4 == monster.x || player.x - 3 == monster.x || player.x - 2 == monster.x || player.x - 5 == monster.x))
				{
					monster.hp -= 6;
					if (bind != 1 && monster.x < 70 && player.x < monster.x && player.mp != 0)
						monster.x += 2;
					if (bind != 1 && monster.x<70 && player.x>monster.x && player.mp != 0)
						monster.x -= 2;
				}
				if (monster.hp <= 0) {
					system("cls");
					mapbase(0, 23);
					/*map1(0, 18);*/
					drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
					potal(80, 20);
					stage2(40, 0);
					drawuser(player.x, player.y);
					gotoxy(40, 10);
					printf("Stage2 Clear!!");
					gotoxy(40, 11);
					printf("포탈을 타고 다음맵으로 넘어가세요!");
					gotoxy(40, 12);
					printf("Level up!!");
					dmg = 7;
				}
			}
			/*if (!jump && player.y < 20) {

				player.y += gravity;

			}*/
			Sleep(30);

			if (player.x == monster.x && monster.hp > 0 && player.y == monster.y) {
				player.hp -= 20;
				system("cls");
				Sleep(50);
			}

			if (player.hp <= 0) return;
			if (monster.hp <= 0) {
				player.level = 3;

			}



		}
		if (player.x == 80 && monster.hp <= 0) {
			break;
		}


	}
	system("cls");
	player.x = 30;
	player.y = 20;
	monster.x = 40;
	monster.y = 20;
	monster.hp = 200;
	bind = 0;
	//=========================================================final stage=================================================================================
	while (1) {

		mapbase(0, 23);




		drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
		map1(0, 18);

		stage3(40, 0);

		drawuser(player.x, player.y);
		bossmonster();



		if (kbhit()) {

			int key = getch();

			switch (key) {

			case 224:

				key = getch();

				switch (key) {


				case 75: // 왼쪽

					if (player.x > 0)
						player.x--;
					else
						player.x = 1;
					if (player.x > monster.x && bind != 1 && monster.y == player.y)
						monster.x += 1;
					if (player.x < monster.x && bind != 1 && monster.y == player.y)
						monster.x -= 1;
					if (player.x > 11 && player.y == 14) {
						player.y = 20;
						system("cls");
					}

					break;

				case 77:  // 오른쪽
					if (player.x < 85) {
						player.x++;
					}
					if (player.x > monster.x && bind != 1 && monster.y == player.y)
						monster.x += 1;
					if (player.x < monster.x && bind != 1 && monster.y == player.y)
						monster.x -= 1;
					if (player.x > 11 && player.y == 14) {
						player.y = 20;
						system("cls");
					}

					break;


				}

				break;

			case 32:  // 스페이스바


				if (player.y == 20 && !jump) {

					jump = 1;


					for (int i = 0; i < height; i++) {
						if (monster.hp <= 0) {
							gotoxy(40, 10);
							printf("Stage1 Clear!!");
							gotoxy(40, 11);
							printf("포탈을 타고 다음맵으로 넘어가세요!");
							gotoxy(40, 12);
							printf("Level up!!");
						}
						if (bind == 1 && monster.hp > 0) {
							gotoxy(40, 10);
							printf("Bind!");
						}
						if (player.y == 14 && player.x >= 0 && player.x <= 11) {
							map1(0, 18);
						}


						player.y--;

						system("cls");
						mapbase(0, 23);

						drawuser(player.x, player.y);

						drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
						stage3(40, 0);
						bossmonster();


						Sleep(30);




					}

					for (int i = 0; i < height; i++) {
						if (monster.hp <= 0) {
							gotoxy(40, 10);
							printf("Stage1 Clear!!");
							gotoxy(40, 11);
							printf("포탈을 타고 다음맵으로 넘어가세요!");
							gotoxy(40, 12);
							printf("Level up!!");
						}
						if (bind == 1 && monster.hp > 0) {
							gotoxy(40, 10);
							printf("Bind!");
						}
						if (player.y == 14 && player.x >= 0 && player.x <= 11) {
							map1(0, 18);
							break;
						}


						player.y++;
						system("cls");
						mapbase(0, 23);
						stage3(40, 0);

						drawuser(player.x, player.y);
						drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);

						bossmonster();
						Sleep(30);





					}

					jump = 0;


				}
				break;


			case 'm':

				drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
				if (player.mpp > 0) {
					player.mp = 50;
					player.mpp -= 1;
				}
				break;

			case 'z':
				skill();
				Sleep(100);

				player.mp -= 20;
				if (monster.hp > 0) {
					bind = 1;
					gotoxy(40, 10);
					printf("Bind!");
				}
				break;
			case 'h':
				drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
				if (player.hpp > 0) {
					player.hpp -= 1;
					player.hp = 100;
				}
				break;

			case 'a':
				if (player.mp > 0 && player.x > monster.x) {
					attack2(player.x, player.y);
					player.mp -= 2;
					drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
					Sleep(100);
				}
				if (player.mp > 0 && player.x < monster.x) {
					attack(player.x, player.y);
					player.mp -= 2;
					drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
					Sleep(100);
				}
				if (player.mp <= 0) break;
				if (player.mp != 0 && (player.x + 6 == monster.x || player.x + 5 == monster.x || player.x + 4 == monster.x || player.x + 3 == monster.x || player.x + 2 == monster.x || player.x + 1 == monster.x) || (player.x - 4 == monster.x || player.x - 3 == monster.x || player.x - 2 == monster.x || player.x - 5 == monster.x || player.x - 1 == monster.x))
				{
					monster.hp -= 7;
					if (bind != 1 && monster.x < 70 && player.x < monster.x && player.mp != 0)
						monster.x += 2;
					if (bind != 1 && monster.x<70 && player.x>monster.x && player.mp != 0)
						monster.x -= 2;
				}
				if (monster.hp <= 0) {
					system("cls");
					mapbase(0, 23);
					map1(0, 18);
					drawui(player.hp, player.mp, player.level, player.hpp, player.mpp, dmg);
					gotoxy(40, 11);
					printf("축하합니다! 보스 몬스터를 처치하셨습니다.");
					gotoxy(40, 12);
					printf(" 포탈을 타고 게임을 완료해 주세요!");
					stage3(40, 0);
					drawuser(player.x, player.y);
					potal(50, 20);
					break;


				}
			}

			Sleep(30);

			if (monster.hp > 0 && player.y == monster.y && player.x == monster.x) {
				player.hp -= 20;
				system("cls");
				Sleep(50);
			}

			if (player.hp <= 0) return;




		}
		if (player.x == 50 && monster.hp <= 0) {

			break;
		}


	}
	system("cls");
	// 클리어시 나타탤 맵 그리기
	textcolor(15, 0);

	for (y = 0; y < 30; y++) {

		gotoxy(0, y);
		for (x = 0; x < 50; x++) {
			if (end[y][x] == 1)
				printf("▒ ");
			else if (end[y][x] == 2) {
				i = rand() % 15;
				textcolor(i, 0);
				printf("■");
				textcolor(15, 0);
			}

			else {

				printf("  ");

			}

		}
	}


	gotoxy(40, 20); printf("게임 종료");

	gotoxy(40, 20);
	while (1) {
		if (GetAsyncKeyState(VK_LEFT))

			if (menu == 0) menu = 1;

			else menu -= 1;

		else if (GetAsyncKeyState(VK_RIGHT))

			if (menu == 1) menu = 0;

			else menu += 1;

		else if (GetAsyncKeyState(VK_RETURN))

			break;

		switch (menu) {

		case 0:


			textcolor(12, 0);

			gotoxy(40, 20); printf("게임 종료");

			break;

		case 1:

			gotoxy(40, 20); printf("게임 종료");

			textcolor(12, 0);

			break;

		}

		Sleep(200);

	}

	if (menu == 0) cls(15, 0);

	else if (menu == 1) return;
}