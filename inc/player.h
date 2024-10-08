#ifndef _H_PLAYER
#define _H_PLAYER

#include <durango.h>
// Player Functions and Data

#define PLAYER_INITIAL_LIVES 3

#define ACTION_RIGHT 1
#define ACTION_LEFT 2
#define ACTION_FIRE 3
#define ACTION_NONE 0
struct{
    sprite sprite;
    char lives;
    unsigned char action;
}player;

unsigned char checkInput();
void drawPlayer();
void movePlayer();
void updatePlayer();
void firePlayerBullet();
#endif
