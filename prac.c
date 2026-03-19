#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

// 색상 및 설정
#define RESET   "\033[0m"
#define RED1    "\033[31m"
#define YELLOW1 "\033[33m"
#define BLUE1   "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN1   "\033[36m"
#define BG_WHITE "\033[47m"

struct Player { int hp, mp, hpp, mpp, x, y, level, dmg; } player;
struct Monster { int hp, max_hp, x, y; } monster;

// --- 시스템 함수 ---
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

int kbhit(void) {
    struct termios oldt, newt;
    int ch, oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) { ungetc(ch, stdin); return 1; }
    return 0;
}

void gotoxy(int x, int y) { printf("\033[%d;%dH", y + 1, x + 1); }
void cls() { printf("\033[H\033[J"); }
void Sleep(int ms) { usleep(ms * 1000); }

// --- 그래픽 함수 ---
void drawui() {
    gotoxy(0, 0); printf(RED1 "[ HP ] %d/100 " RESET, player.hp);
    gotoxy(0, 1); printf(BLUE1 "[ MP ] %d/50  " RESET, player.mp);
    gotoxy(0, 2); printf("[ Lv %d ] [ DMG : %d ]", player.level, player.dmg);
    gotoxy(0, 4); printf("H: HP포션(%d) / M: MP포션(%d)", player.hpp, player.mpp);
    gotoxy(0, 5); printf("Ult:z");
}

void drawuser(int x, int y) {
    gotoxy(x, y);     printf("   O   ");
    gotoxy(x, y + 1); printf(" ⊂[ ]⊃ ");
    gotoxy(x, y + 2); printf("  / \\  ");
}

void drawmonster(int isBoss, int bind) {
    if (monster.hp <= 0) return;
    if (bind) { printf(CYAN1); gotoxy(monster.x, monster.y - 1); printf(" [ BINDED ] "); }
    else { printf(RED1); }

    gotoxy(monster.x, monster.y); printf("  HP:%d  ", monster.hp);
    if (isBoss) {
        gotoxy(monster.x, monster.y + 1); printf(" < @@@@ >");
        gotoxy(monster.x, monster.y + 2); printf("  @@@@@@ ");
    }
    else {
        gotoxy(monster.x, monster.y + 1); printf("  (oo)  ");
        gotoxy(monster.x, monster.y + 2); printf("   @@   ");
    }
    printf(RESET);
}

void mapbase() {
    printf(YELLOW1);
    gotoxy(0, 23); printf("===================================================================================================");
    printf(RESET);
}

// --- 게임 로직 ---
int playStage(const char* stageName, int targetX, int isBoss) {
    int bind_timer = 0;
    int monster_move_tick = 0;

    while (1) {
        cls();
        mapbase();
        drawui();
        gotoxy(40, 0); printf("--- %s ---", stageName);
        if (monster.hp <= 0) {
            printf(BLUE1); gotoxy(targetX, 20); printf("EXIT▶"); printf(RESET);
        }

        drawmonster(isBoss, bind_timer > 0);
        drawuser(player.x, player.y);
        fflush(stdout);

        // 1. 몬스터 AI: 플레이어 추적 (바인드 아닐 때만)
        if (monster.hp > 0 && bind_timer <= 0) {
            monster_move_tick++;
            if (monster_move_tick >= 3) { // 몬스터 속도 조절
                if (monster.x < player.x) monster.x++;
                else if (monster.x > player.x) monster.x--;
                monster_move_tick = 0;
            }
        }
        if (bind_timer > 0) bind_timer--;

        // 2. 키 입력 처리
        if (kbhit()) {
            int key = getch();
            if (key == 27) { // 방향키
                getch(); key = getch();
                if (key == 68 && player.x > 0) player.x--;
                if (key == 67 && player.x < 85) player.x++;
            }
            else if (key == 'a' || key == 'A') { // 일반 공격
                if (player.mp >= 2) {
                    player.mp -= 2;

                    // 1. 방향 및 히트 판정 계산
                    // 캐릭터 중앙과 몬스터 중앙의 거리 차이를 계산
                    int diff = monster.x - player.x;
                    int dist = abs(diff);
                    int dir = (diff > 0) ? 1 : -1;

                    // 2. 공격 애니메이션 (잠깐 표시)
                    gotoxy(player.x + (dir * 4), player.y + 1);
                    printf(CYAN1 "==* HIT *==" RESET);
                    fflush(stdout);

                    // 3. 대미지 판정 (거리가 10 이내면 무조건 적중)
                    if (dist < 10) {
                        monster.hp -= player.dmg; // 대미지 차감

                        // [핵심] 바인드 중이 아닐 때만 넉백(밀려남) 발생
                        if (bind_timer <= 0) {
                            monster.x += dir * 2;
                        }

                        // 대미지 텍스트 효과
                        gotoxy(monster.x, monster.y - 1);
                        printf(RED1 "-%d" RESET, player.dmg);
                        fflush(stdout);
                    }

                    // 애니메이션을 위해 아주 짧게 대기
                    usleep(50000);
                }
            }
            else if (key == 'z' || key == 'Z') { // 바인드 스킬
                int distance = abs(player.x - monster.x); // 플레이어와 몬스터 사이의 거리 계산
                int range = 15; // 바인드 사거리 설정

                if (player.mp >= 20) {
                    // 1. 사거리 체크
                    if (distance <= range) {
                        player.mp -= 20;
                        bind_timer = 40; // 약 2초간 멈춤
                        monster.hp -= (player.dmg * 2); // 바인드 대미지

                        // 성공 이펙트
                        gotoxy(monster.x, monster.y - 1);
                        printf(MAGENTA " [!! BINDED !!] " RESET);
                        gotoxy(monster.x, monster.y - 2);
                        printf(RED1 " -%d ", player.dmg * 2); // 대미지 표시
                        fflush(stdout);
                        Sleep(400);
                    }
                    else {
                        // 2. 사거리 밖일 때 알림 (MP 소모 없음)
                        gotoxy(player.x, player.y - 1);
                        printf(YELLOW1 "너무 멀어서 닿지 않습니다!" RESET);
                        fflush(stdout);
                        Sleep(300);
                    }
                }
                else {
                    // MP 부족 시 알림
                    gotoxy(player.x, player.y - 1);
                    printf(BLUE1 "MP가 부족합니다!" RESET);
                    fflush(stdout);
                    Sleep(300);
                }
            }
            else if (key == ' ' && player.y == 20) { // 점프
                for (int i = 0; i < 4; i++) { player.y--; Sleep(20); }
                for (int i = 0; i < 4; i++) { player.y++; Sleep(20); }
            }
            else if (key == 'h' && player.hpp > 0) { player.hp = 100; player.hpp--; }
            else if (key == 'm' && player.mpp > 0) { player.mp = 50; player.mpp--; }
        }

        // 3. 충돌 처리 (몬스터와 부딪힘)
        if (monster.hp > 0 && abs(player.x - monster.x) < 3 && player.y == monster.y) {
            player.hp -= 15;
            // 넉백 로직
            if (player.x < monster.x) player.x -= 5; else player.x += 5;
            if (player.x < 0) player.x = 0;
            gotoxy(player.x, player.y - 1); printf(RED1 "(-15 HP)" RESET);
            fflush(stdout);
            Sleep(300);
        }

        if (player.hp <= 0) return 0;
        if (monster.hp <= 0 && player.x >= targetX - 2 && player.x <= targetX + 2) return 1;

        Sleep(50);
    }
}

int main() {
    srand(time(NULL));
    printf("\033[?25l"); // 커서 숨기기

    cls();
    gotoxy(30, 10); printf("GAME START (Press Enter)");
    while (getch() != 10);

    player = (struct Player){ 100, 50, 5, 5, 5, 20, 1, 10 };

    // 스테이지 구성
    monster = (struct Monster){ 100, 100, 60, 20 };
    if (!playStage("Stage 1", 80, 0)) goto OVER;

    player.level = 2; player.dmg += 5; player.x = 5;
    monster = (struct Monster){ 150, 150, 70, 20 };
    if (!playStage("Stage 2", 80, 0)) goto OVER;

    player.level = 3; player.dmg += 10; player.x = 5;
    monster = (struct Monster){ 300, 300, 50, 20 };
    if (!playStage("BOSS BATTLE", 50, 1)) goto OVER;

    cls();
    gotoxy(30, 12); printf(YELLOW1 "★ CONGRATULATIONS! GAME CLEAR ★" RESET);
    Sleep(3000);
    return 0;

OVER:
    cls();
    gotoxy(35, 12); printf(RED1 "G A M E  O V E R" RESET);
    Sleep(2000);
    return 0;
}