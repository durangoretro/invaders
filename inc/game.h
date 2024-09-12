#ifndef _H_GAME
#define _H_GAME

#include <durango.h>

#include "enemy.h"
#include "player.h"

// game functions

#define MAX_ENEMIES 32
#define MAX_BULLETS 20

typedef enum{
    START=0,
    GAME_INIT,
    GAME_LOOP,
    GAMEOVER
}State;



struct{
    State status;//TODO: Change to STart
    enemy enemies[MAX_ENEMIES];
    char direction;
    Bullet playerBullet;
    int currentBullet;
    Bullet enemiesBullets[MAX_BULLETS];
    int currentEnemyBullet;
    long score;
}Game;

void draw();
void update();
void init();
void drawScore();

//Player Bullet Functions
void initPlayerBullet(Bullet* output, byte x, byte y, byte width, byte height, void * resource);

#endif

