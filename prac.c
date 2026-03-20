#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// 해상도 설정
const int SCREEN_W = 1280;
const int SCREEN_H = 720;
const int GROUND_Y = 540; 

typedef enum { STATE_PLAYING, STATE_GAME_OVER, STATE_VICTORY } GameState;
typedef enum { IDLE, RUN, ATTACK, JUMP, HURT, DIE } PlayerState;
typedef enum { M_IDLE, M_ATTACK, M_HURT, M_DEAD } MonsterAnim;
typedef enum { S_GREEN, S_BLUE, S_RED, S_YELLOW } SlimeColor;

struct Player {
    float x, y, dy;
    int hp, mp;
    PlayerState state;
    int frame, timer, max_frames;
    int attack_timer, hurt_timer, inv_timer;
    bool isGrounded;
    SDL_RendererFlip flip;
} player = {100, 540, 0, 100, 50, IDLE, 0, 0, 8, 0, 0, 0, true, SDL_FLIP_NONE};

struct Monster {
    float x, y;
    int hp, max_hp;
    bool alive, isBoss;
    MonsterAnim animState;
    SlimeColor color;
    int frame, timer;
    int size; 
    int hurt_timer;
};

struct Item { float x, y; bool active; } potion = {0, 0, false};
struct Projectile { float x, y, vx, vy; bool active; } bossBullet = {0, 0, 0, 0, false};

struct Monster monsters[3];
int currentStage = 1;
int monsterCount = 1;
GameState gameState = STATE_PLAYING;

// 스테이지 초기화 함수
void initStage(int stage) {
    if (stage == 1) {
        monsterCount = 1;
        monsters[0] = (struct Monster){900, GROUND_Y - 140, 100, 100, true, false, M_IDLE, S_GREEN, 0, 0, 150, 0};
    } else if (stage == 2) {
        monsterCount = 3;
        monsters[0] = (struct Monster){700, GROUND_Y - 130, 80, 80, true, false, M_IDLE, S_BLUE, 0, 0, 130, 0};
        monsters[1] = (struct Monster){900, GROUND_Y - 130, 80, 80, true, false, M_IDLE, S_BLUE, 0, 0, 130, 0};
        monsters[2] = (struct Monster){1100, GROUND_Y - 140, 100, 100, true, false, M_IDLE, S_RED, 0, 0, 150, 0};
    } else if (stage == 3) {
        monsterCount = 1;
        monsters[0] = (struct Monster){900, GROUND_Y - 340, 500, 500, true, true, M_IDLE, S_YELLOW, 0, 0, 400, 0};
    }
    potion.active = false;
    bossBullet.active = false;
}

void drawText(SDL_Renderer* ren, TTF_Font* font, const char* text, int x, int y, SDL_Color col) {
    if (!font || !text) return;
    SDL_Surface* surf = TTF_RenderUTF8_Blended(font, text, col);
    if (!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
    SDL_Rect dst = { x, y, surf->w, surf->h };
    SDL_RenderCopy(ren, tex, NULL, &dst);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_Window* win = SDL_CreateWindow("C-RPG Final Edition (1280x720)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, 0);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font* font = TTF_OpenFont("font.ttf", 28);
    TTF_Font* bigFont = TTF_OpenFont("font.ttf", 90);
    
    SDL_Texture* pTex = IMG_LoadTexture(ren, "player_sheet.png");
    SDL_SetTextureBlendMode(pTex, SDL_BLENDMODE_BLEND);
    SDL_Texture* mTex = IMG_LoadTexture(ren, "monster_sheet.png");
    SDL_SetTextureBlendMode(mTex, SDL_BLENDMODE_BLEND);
    
    SDL_Texture* bgTex1 = IMG_LoadTexture(ren, "bg_stage1.png");
    SDL_Texture* bgTex2 = IMG_LoadTexture(ren, "bg_stage2.png");
    SDL_Texture* bgTexBoss = IMG_LoadTexture(ren, "bg_boss.png");
    
    SDL_Texture* iTex = IMG_LoadTexture(ren, "item.png");
    SDL_SetTextureBlendMode(iTex, SDL_BLENDMODE_BLEND);

    bool running = true;
    SDL_Event ev;
    int bossAttackTimer = 0;
    initStage(1);

    while (running) {
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) running = false;
            if (ev.type == SDL_KEYDOWN) {
                if (gameState == STATE_PLAYING && player.hp > 0) {
                    if (ev.key.keysym.sym == SDLK_a && player.attack_timer <= 0) {
                        player.attack_timer = 30; player.state = ATTACK; player.frame = 0;
                        for(int i=0; i<monsterCount; i++) {
                            if (monsters[i].alive && monsters[i].animState != M_DEAD) {
                                float range = monsters[i].isBoss ? 300 : 180;
                                if (fabs(player.x - monsters[i].x) < range) {
                                    monsters[i].hp -= 40;
                                    if (monsters[i].hp <= 0) { monsters[i].animState = M_DEAD; monsters[i].frame = 0; }
                                    else { monsters[i].animState = M_HURT; monsters[i].hurt_timer = 20; monsters[i].frame = 0; }
                                }
                            }
                        }
                    }
                    if (ev.key.keysym.sym == SDLK_SPACE && player.isGrounded && player.attack_timer <= 0) {
                        player.dy = -18.0f; player.isGrounded = false;
                    }
                } else if (ev.key.keysym.sym == SDLK_r) { 
                    currentStage = 1; initStage(1);
                    player = (struct Player){100, 540, 0, 100, 50, IDLE, 0, 0, 8, 0, 0, 0, true, SDL_FLIP_NONE};
                    gameState = STATE_PLAYING;
                }
            }
        }

        if (gameState == STATE_PLAYING) {
            const Uint8* ks = SDL_GetKeyboardState(NULL);
            if (!player.isGrounded) { player.dy += 1.0f; player.y += player.dy; }
            if (player.y >= GROUND_Y) { player.y = GROUND_Y; player.dy = 0; player.isGrounded = true; }

            if (player.hp > 0 && player.hurt_timer <= 0 && player.attack_timer <= 0) {
                if (ks[SDL_SCANCODE_LEFT] && player.x > 0) { player.x -= 7; player.state = RUN; player.flip = SDL_FLIP_NONE; }
                else if (ks[SDL_SCANCODE_RIGHT] && player.x < SCREEN_W - 150) { player.x += 7; player.state = RUN; player.flip = SDL_FLIP_HORIZONTAL; }
                else player.state = IDLE;
            }

            if (player.hp <= 0) player.state = DIE;
            else if (player.hurt_timer > 0) { player.state = HURT; player.hurt_timer--; }
            else if (player.attack_timer > 0) { player.attack_timer--; if (player.attack_timer == 0) player.state = IDLE; }
            else if (!player.isGrounded) player.state = JUMP;

            for(int i=0; i<monsterCount; i++) {
                if (!monsters[i].alive) continue;
                if (monsters[i].animState == M_DEAD) {
                    monsters[i].timer++;
                    if (monsters[i].timer >= 6) {
                        monsters[i].frame++;
                        if (monsters[i].frame >= 6) { 
                            monsters[i].alive = false; 
                            if (!potion.active) { potion.x = monsters[i].x; potion.y = GROUND_Y + 50; potion.active = true; }
                        }
                        monsters[i].timer = 0;
                    }
                } else if (monsters[i].animState == M_HURT) {
                    monsters[i].hurt_timer--; monsters[i].timer++;
                    if (monsters[i].timer >= 5) { monsters[i].frame = (monsters[i].frame + 1) % 4; monsters[i].timer = 0; }
                    if (monsters[i].hurt_timer <= 0) monsters[i].animState = M_IDLE;
                } else {
                    float dist = fabs(player.x - monsters[i].x);
                    if (dist < 250 && monsters[i].animState == M_IDLE) monsters[i].animState = M_ATTACK;
                    monsters[i].timer++;
                    if (monsters[i].timer >= 8) {
                        int m_max = (monsters[i].animState == M_IDLE) ? 8 : 6;
                        monsters[i].frame = (monsters[i].frame + 1) % m_max;
                        if (monsters[i].animState == M_ATTACK && monsters[i].frame == 0) monsters[i].animState = M_IDLE;
                        monsters[i].timer = 0;
                    }
                    int collX = monsters[i].isBoss ? 160 : 70;
                    if (player.inv_timer <= 0 && dist < collX && fabs(player.y - monsters[i].y) < 250) {
                        player.hp -= 15; player.hurt_timer = 25; player.inv_timer = 60;
                        player.x += (player.x < monsters[i].x) ? -100 : 100;
                    }
                }
            }

            if (potion.active && fabs(player.x - potion.x) < 60) {
                player.hp = (player.hp + 30 > 100) ? 100 : player.hp + 30; potion.active = false;
            }

            if (currentStage == 3 && monsters[0].alive && monsters[0].animState != M_DEAD) {
                bossAttackTimer++;
                if (bossAttackTimer > 90) {
                    bossBullet.active = true; bossBullet.x = monsters[0].x + 180; bossBullet.y = monsters[0].y + 180;
                    float dx = player.x - bossBullet.x, dy = player.y - bossBullet.y;
                    float d = sqrt(dx*dx + dy*dy);
                    bossBullet.vx = (dx/d)*10; bossBullet.vy = (dy/d)*10;
                    bossAttackTimer = 0;
                }
            }
            if (bossBullet.active) {
                bossBullet.x += bossBullet.vx; bossBullet.y += bossBullet.vy;
                if (player.inv_timer <= 0 && fabs(player.x-bossBullet.x)<50 && fabs(player.y-bossBullet.y)<120) {
                    player.hp -= 10; player.hurt_timer = 20; player.inv_timer = 40; bossBullet.active = false;
                }
                if (bossBullet.x < 0 || bossBullet.x > SCREEN_W || bossBullet.y > SCREEN_H) bossBullet.active = false;
            }

            bool allDead = true;
            for(int i=0; i<monsterCount; i++) if (monsters[i].alive) allDead = false;
            if (allDead) {
                if (currentStage < 3) { currentStage++; initStage(currentStage); player.x = 100; }
                else gameState = STATE_VICTORY;
            }
            if (player.hp <= 0) gameState = STATE_GAME_OVER;
            
            player.timer++;
            if (player.timer >= 7) {
                // 점프 시 이미지 개수에 맞게 최대 프레임을 2로 제한 (검은 박스 방지)
                player.max_frames = (player.state == JUMP) ? 2 : 8;
                if (player.state == DIE && player.frame == 7) player.frame = 7;
                else player.frame = (player.frame + 1) % player.max_frames;
                player.timer = 0;
            }
            if (player.inv_timer > 0) player.inv_timer--;
        }

        // --- 렌더링 ---
        SDL_RenderClear(ren);
        SDL_Rect bgRect = {0, 0, SCREEN_W, SCREEN_H};
        
        SDL_Texture* curBG = bgTex1;
        if (currentStage == 2) curBG = bgTex2;
        else if (currentStage == 3) curBG = bgTexBoss;
        SDL_RenderCopy(ren, curBG, NULL, &bgRect);

        if (gameState != STATE_PLAYING) {
            SDL_Color col = (gameState == STATE_VICTORY) ? (SDL_Color){0,255,0} : (SDL_Color){255,0,0};
            drawText(ren, bigFont, (gameState == STATE_VICTORY) ? "VICTORY!" : "GAME OVER", SCREEN_W/2 - 200, SCREEN_H/2 - 100, col);
            drawText(ren, font, "Press 'R' to Restart", SCREEN_W/2 - 120, SCREEN_H/2 + 50, (SDL_Color){255,255,255});
        } else {
            if (potion.active) { SDL_Rect iR = {(int)potion.x, (int)potion.y, 60, 60}; SDL_RenderCopy(ren, iTex, NULL, &iR); }
            for(int i=0; i<monsterCount; i++) {
                if (monsters[i].alive) {
                    int colorRow = monsters[i].color * 5;
                    int animRow = (monsters[i].animState == M_IDLE) ? 0 : (monsters[i].animState == M_ATTACK ? 3 : (monsters[i].animState == M_HURT ? 2 : 4));
                    SDL_Rect mSrc = { monsters[i].frame * 32, (colorRow + animRow) * 32, 32, 32 };
                    SDL_Rect mDst = { (int)monsters[i].x, (int)monsters[i].y, monsters[i].size, monsters[i].size };
                    SDL_RendererFlip mFlip = (player.x > monsters[i].x ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
                    SDL_RenderCopyEx(ren, mTex, &mSrc, &mDst, 0, NULL, mFlip);
                }
            }
            if (bossBullet.active) {
                SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
                SDL_Rect bR = {(int)bossBullet.x, (int)bossBullet.y, 25, 25}; SDL_RenderFillRect(ren, &bR);
            }
            
            // 플레이어 렌더링 로직 (프레임 0~1만 순환하므로 옆의 검은 공간을 그리지 않음)
            SDL_Rect pSrc = { player.frame * 550, player.state * 598, 550, 598 };
            SDL_Rect pDst = { (int)player.x, (int)player.y, 180, 200 };
            if (player.inv_timer % 10 < 5) SDL_RenderCopyEx(ren, pTex, &pSrc, &pDst, 0, NULL, player.flip);
            
            char b[64]; sprintf(b, "STAGE %d  HP %d", currentStage, player.hp);
            drawText(ren, font, b, 40, 40, (SDL_Color){255,255,255});
        }
        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    TTF_CloseFont(font); TTF_CloseFont(bigFont);
    SDL_DestroyTexture(pTex); SDL_DestroyTexture(mTex); 
    SDL_DestroyTexture(bgTex1); SDL_DestroyTexture(bgTex2); SDL_DestroyTexture(bgTexBoss);
    SDL_DestroyTexture(iTex);
    SDL_DestroyRenderer(ren); SDL_DestroyWindow(win);
    TTF_Quit(); IMG_Quit(); SDL_Quit();
    return 0;
}