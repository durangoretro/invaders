#ifndef _H_GAME
#define _H_GAME

#include <durango.h>

#include "enemy.h"
#include "player.h"

// game functions

#define MAX_ENEMIES 32
#define MAX_BULLETS 4

#define MAX_STEPS 8

#define BULLET_SPEED 6

typedef enum{
    START=0,
    GAME_INIT,
    GAME_LOOP,
    GAMEOVER
}State;



struct{
    State status;
    enemy enemies[MAX_ENEMIES];
    Bullet enemiesBullets[MAX_BULLETS];
    char direction;
    Bullet playerBullet;
    char enemiesSteps;
    int currentBullet;
    int currentEnemyBullet;
    long score;
    char level;
}Game;

void draw();
void update();
void init();
void drawScore();
void drawLives();
void fire_invaders();
void initInvaderBullet(Bullet *output, enemy *invader);
void moveInvadersBullets();
void checkBulletsCols();
void checkInvaderBulletsCols();
void kill();

//Player Bullet Functions
void initPlayerBullet(Bullet* output, byte x, byte y, byte width, byte height, void * resource);

#endif

