#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
 
// 색상 정의 (ANSI Escape Codes)
#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED1    "\033[31m"
#define YELLOW1 "\033[33m"
#define CYAN1   "\033[36m"
#define BLUE1   "\033[34m"
#define WHITE   "\033[37m"
#define BG_WHITE "\033[47m"

// 구조체 정의
struct Player {
    int hp, mp, hpp, mpp, x, y, level, dmg;
} player;

struct Monster {
    int hp, x, y;
} monster;

// 리눅스용 getch() 구현
int getch(void) {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

// 리눅스용 kbhit() 구현
int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

// 유틸리티 함수
void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
}

void removeCursor(void) {
    printf("\033[?25l");
}

void textcolor(const char* color) {
    printf("%s", color);
}

void cls() {
    printf("\033[H\033[J"); // 화면 지우고 커서 홈으로
}

void Sleep(int ms) {
    usleep(ms * 1000);
}

// UI 및 캐릭터 출력 함수
void drawui() {
    gotoxy(0, 0); printf("[ HP ] %d/100 ", player.hp);
    gotoxy(0, 1); printf("[ MP ] %d/50  ", player.mp);
    gotoxy(0, 2); printf("[ Lv %d ]      ", player.level);
    gotoxy(0, 3); printf("[ DMG : %d ]  ", player.dmg);
    gotoxy(0, 4); printf("[ HP 포션: %d ]", player.hpp);
    gotoxy(0, 5); printf("[ MP 포션: %d ]", player.mpp);
}

void skillui() {
    gotoxy(40, 4); printf("공격 : A / 스킬 : Z");
    gotoxy(40, 5); printf("점프 : Space");
    gotoxy(40, 6); printf("포션 : H(HP), M(MP)");
    gotoxy(40, 7); printf("이동 : 방향키 (w,a,s,d 추천)");
}

void drawuser(int x, int y) {
    gotoxy(x, y);     printf("   O   ");
    gotoxy(x, y + 1); printf(" ⊂l⊃ ");
    gotoxy(x, y + 2); printf("  l l  ");
}

void drawmonster(int isBoss) {
    if (monster.hp > 0) {
        printf(RED1 BG_WHITE);
        gotoxy(monster.x, monster.y); printf("  %d  ", monster.hp);
        if (isBoss) {
            gotoxy(monster.x, monster.y + 1); printf("  @@@@ ");
            gotoxy(monster.x, monster.y + 2); printf(" @@@@@@ ");
        }
        else {
            gotoxy(monster.x, monster.y + 1); printf("  ⌒  ");
            gotoxy(monster.x, monster.y + 2); printf("  @@  ");
        }
        printf(RESET);
    }
}

void attack_effect(int x, int y, int dir) {
    gotoxy(x, y);     printf("   O   ");
    if (dir == 1) { // 오른쪽
        gotoxy(x, y + 1); printf("  olo-> ");
        gotoxy(x, y + 2); printf("  ㅣ＼  ");
    }
    else { // 왼쪽
        gotoxy(x, y + 1); printf(" <-lo   ");
        gotoxy(x, y + 2); printf("  / ㅣ  ");
    }
}

void potal(int x, int y) {
    printf(BLUE1);
    for (int i = 0; i < 3; i++) { gotoxy(x, y + i); printf("∥∥"); }
    printf(RESET);
}

void mapbase(int x, int y) {
    printf(YELLOW1);
    gotoxy(x, y); printf("===================================================================================================");
    for (int i = 1; i <= 5; i++) {
        gotoxy(x, y + i);
        printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
    }
    printf(RESET);
}

int playStage(const char* stageName, int targetX, int isBoss) {
    int bind = 0;

    while (1) {
        cls();
        mapbase(0, 23);
        skillui();
        drawui();
        gotoxy(40, 0); printf("%s", stageName);
        if (monster.hp <= 0) potal(targetX, 20);

        drawuser(player.x, player.y);
        drawmonster(isBoss);
        fflush(stdout); // 리눅스 터미널 즉시 출력

        if (kbhit()) {
            int key = getch();
            if (key == 27) { // 방향키 처리 (ESC 시퀀스)
                getch(); // [ 건너뛰기
                key = getch();
                if (key == 68 && player.x > 0) player.x--; // 좌
                if (key == 67 && player.x < 85) player.x++; // 우
            }
            else if (key == 'a' || key == 'A') { // 공격
                if (player.mp >= 2) {
                    attack_effect(player.x, player.y, (player.x < monster.x ? 1 : 0));
                    player.mp -= 2;
                    if (abs(player.x - monster.x) < 7 && monster.hp > 0) {
                        monster.hp -= player.dmg;
                        if (!bind) { if (player.x < monster.x) monster.x += 2; else monster.x -= 2; }
                    }
                }
            }
            else if (key == ' ' && player.y == 20) { // 점프
                for (int i = 0; i < 5; i++) { player.y--; cls(); mapbase(0, 23); drawuser(player.x, player.y); drawmonster(isBoss); drawui(); fflush(stdout); Sleep(30); }
                for (int i = 0; i < 5; i++) { player.y++; cls(); mapbase(0, 23); drawuser(player.x, player.y); drawmonster(isBoss); drawui(); fflush(stdout); Sleep(30); }
            }
            else if (key == 'h') { if (player.hpp > 0) { player.hp = 100; player.hpp--; } }
            else if (key == 'm') { if (player.mpp > 0) { player.mp = 50; player.mpp--; } }
            else if (key == 'z') { if (player.mp >= 20) { player.mp -= 20; bind = 1; gotoxy(40, 10); printf("BIND!"); fflush(stdout); Sleep(500); } }
        }

        if (monster.hp > 0 && player.x >= monster.x - 2 && player.x <= monster.x + 2 && player.y == monster.y) {
            player.hp -= 20;
            Sleep(100);
        }
        if (player.hp <= 0) return 0;
        if (monster.hp <= 0 && player.x == targetX) return 1;

        Sleep(50);
    }
}

int main() {
    srand((unsigned)time(NULL));
    removeCursor();

    cls();
    gotoxy(30, 10); printf("GAME START (Press Enter)");
    while (getch() != 10); // 리눅스 엔터는 10

    player = (struct Player){ 100, 50, 5, 5, 0, 20, 1, 5 };

    // Stage 1
    monster = (struct Monster){ 100, 40, 20 };
    if (!playStage("Stage 1", 80, 0)) { printf("GAME OVER\n"); return 0; }

    // Stage 2
    player.level = 2; player.dmg = 7; player.x = 0;
    monster = (struct Monster){ 120, 50, 20 };
    if (!playStage("Stage 2", 80, 0)) { printf("GAME OVER\n"); return 0; }

    // Boss Stage
    player.level = 3; player.dmg = 10; player.x = 0;
    monster = (struct Monster){ 200, 40, 20 };
    if (!playStage("Boss Stage", 50, 1)) { printf("GAME OVER\n"); return 0; }

    cls();
    printf(YELLOW1);
    gotoxy(35, 12); printf("CONGRATULATIONS! GAME CLEAR!\n\n");
    printf(RESET);
    return 0;
}