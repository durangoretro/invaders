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
    GAME_LOOP,
    GAMEOVER
}State;

struct{
    State status;//TODO: Change to STart
    enemy enemies[MAX_ENEMIES];
    sprite playerBullets[MAX_BULLETS];
    sprite enemiesBullets[MAX_BULLETS];
}Game;

void draw();
void update();
void init();


#endif

