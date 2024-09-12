#ifndef _H_ENEMY
#define _H_ENEMY

#include <durango.h>

#define VISIBLE 1
#define NOT_VISIBLE 0

//Enemy Directions
#define ENEMY_RIGHT 1
#define ENEMY_LEFT 2
#define ENEMY_DOWN 0

typedef struct{
    sprite sprite;
    byte visible;
}Bullet;

// Enemy Functions and Data

typedef struct {
    sprite sprite;
    int lives;
    unsigned char visible;
    Bullet bullet;
}enemy;


void initEnemy(enemy* output,unsigned char, unsigned char, unsigned char width, unsigned char height, void * resource);
void drawEnemy(enemy * enem);
void moveEnemy(enemy* enem);
void updateEnemy(enemy* output);

#endif