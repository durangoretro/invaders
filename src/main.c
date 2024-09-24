/**
 * Invaders Game
 */

// Include Library
#include <durango.h>
#include <font.h>

#include "game.h"

// Resources
#include "playerSptr.h"
#include "redSptr.h"
#include "greenSptr.h"
#include "yellowSptr.h"
#include "bulletSptr.h"
#include "initial.h"
#include "black.h"
#include "gameover.h"

// Functions Prototype
void startGame();
void restartGame();

void updateEnemies();
void drawEnemies();
void moveEnemies();

void firePlayerBullet();
void drawPlayerBullet();
void movePlayerBullets();

// Collisions
void checkCols();
void checkEnemyCols();

void checkEndLevel();

int main()
{
    init();
    // inifinite Loop
    while (1)
    {
        update();
        draw();
        // Wait for VSync
        waitVSync();
    }
    return 0;
}

void init()
{
    Game.status = 0;
    Game.score = 0;
    drawFullScreen(BLACK);
}

// Update Function
void update()
{
    player.action = checkInput();
    switch (Game.status)
    {
    case 0: // TODO Start

        break;
    case 1: // Game Init
        startGame();
        break;
    case 2: // TODO Game Loop
        updateEnemies();

        // Input and Player move
        updatePlayer();
        checkCols();
        break;
    case 3: // Game Over
        break;
    default:
        break;
    }
}

void draw()
{
    switch (Game.status)
    {
    case 0: 
        load_background(initial);
        // Clear Screen and update buffer
        clrscr();
        if (player.action == ACTION_FIRE)
        {
            // Init random seed
            random_init((int)get_time());
            //clear Screen for game mode
            load_background(black);
            clrscr();
            Game.status = 1;
        }
        break;
    case 1: 
        drawEnemies();
        drawPlayer(&player);
        drawScore();
        Game.status = 2; // To Game Loop

        break;
    case 2:
        moveEnemies();
        movePlayer();
        drawPlayerBullet();
        movePlayerBullets();
        moveInvadersBullets();
        drawScore();
        drawLives();
        break;
    case 3: 
        load_background(gameover);
        clrscr();
        if(player.action==ACTION_FIRE){
            Game.status=0;
        }
        break;
    default:
        break;
    }
}

void drawPlayer()
{
    draw_sprite(&player.sprite);
}

void drawScore()
{
    printBCD(80, 1, font, WHITE, BLACK, Game.score);
}
void drawLives(){
    unsigned char i;
    printStr(2,1,font,WHITE,BLACK,"Lives: ");
    //Due to the use of BCD with all the 0 we use this "solution"
    switch (player.lives)
    {
    case 3:
        printStr(40,1,font,WHITE,BLACK,"3");
        break;
    case 2:
        printStr(40,1,font,WHITE,BLACK,"2");
        break;
    case 1:
        printStr(40,1,font,WHITE,BLACK,"1");
        break;
    }
    
}

void initEnemy(enemy *output, unsigned char x, unsigned char y, unsigned char width, unsigned char height, void *resource)
{
    output->lives = 3;
    output->sprite.x = x;
    output->sprite.y = y;
    output->sprite.width = width;
    output->sprite.height = height;
    output->sprite.resource = resource;
}

void initPlayerBullet(Bullet *output, byte x, byte y, byte width, byte height, void *resource)
{

    output->visible = NOT_VISIBLE;
    output->sprite.x = x;
    output->sprite.y = y;
    output->sprite.width = width;
    output->sprite.height = height;
    output->sprite.resource = resource;
}

void initInvaderBullet(Bullet *output, enemy *invader)
{
    output->visible = VISIBLE;
    output->sprite.x = invader->sprite.x+2;
    output->sprite.y = invader->sprite.y+10;
    output->sprite.width = 2;
    output->sprite.height = 6;
    output->sprite.resource = &bulletSptr_0_0;
    draw_sprite(output);
}

void startGame()
{
    unsigned char i;
    // init Player
    player.lives = PLAYER_INITIAL_LIVES;
    player.sprite.resource = &playerSptr_0_0;
    player.sprite.width = 16;
    player.sprite.height = 8;
    player.sprite.x = 56;
    player.sprite.y = 115;
    // Init game Bullets
    Game.currentBullet = 0;
    Game.currentEnemyBullet = 0;
    Game.level=1;
    // red 1
    for (i = 0; i < 8; i++)
    {
        initEnemy(&Game.enemies[i], 5 + (i * 14), 40, 10, 8, &redSptr_0_0);
        Game.enemies[i].visible = VISIBLE;
    }
    // green
    for (i = 8; i < 16; i++)
    {
        initEnemy(&Game.enemies[i], 5 + ((i - 8) * 14), 27, 10, 8, &greenSptr_0_0);
        Game.enemies[i].visible = VISIBLE;
    }
    // yellow
    for (i = 16; i < 24; i++)
    {
        initEnemy(&Game.enemies[i], 5 + ((i - 16) * 14), 12, 10, 8, &yellowSptr_0_0);
        Game.enemies[i].visible = VISIBLE;
    }
    // Second Red
    for (i = 24; i < 32; i++)
    {
        initEnemy(&Game.enemies[i], 5 + ((i - 24) * 14), 52, 10, 8, &redSptr_0_0);
        Game.enemies[i].visible = VISIBLE;
    }
    Game.enemiesSteps=0;
    Game.direction = ENEMY_RIGHT;
}

void updateEnemies()
{
    // Enemy Direction
    if (Game.direction == ENEMY_DOWN && Game.enemiesSteps >= MAX_STEPS/Game.level)
    {
        Game.direction = ENEMY_LEFT;
    }
    if (Game.direction == ENEMY_DOWN && Game.enemiesSteps <= 0)
    {
        Game.direction = ENEMY_RIGHT;
    }
    if (Game.direction == ENEMY_LEFT && Game.enemiesSteps <= 0)
    {
        Game.direction = ENEMY_DOWN;
    }
    if (Game.direction == ENEMY_RIGHT && Game.enemiesSteps >= MAX_STEPS/Game.level)
    {
        Game.direction = ENEMY_DOWN;
    }

    switch (Game.direction)
    {
    case ENEMY_RIGHT:
        Game.enemiesSteps++;
        break;
    case ENEMY_LEFT:
        Game.enemiesSteps--;
        break;
    default:
        break;
    }
    
    fire_invaders();
}

void fire_invaders() {
    unsigned char column;
    unsigned char i,j;
    Bullet *currentBullet;
    enemy *currentInvader;
    // Fire invaders bullet
    if(random()>245) {
        consoleLogStr("FIRE\n");
        column = random() % 8;
        consoleLogHex(column);
        consoleLogStr("\n");
        for(i=0; i<MAX_BULLETS; i++){
            if(Game.enemiesBullets[i].visible==NOT_VISIBLE) {
                currentBullet = &(Game.enemiesBullets[i]);
                for(j=24+column; j>0; j-=8) {
                    if(Game.enemies[j].visible==1) {
                        currentInvader = &(Game.enemies[j]);
                        initInvaderBullet(currentBullet, currentInvader);
                        break;
                    }
                }
                break;
            }
        }
    }
}

void updatePlayer()
{
    switch (player.action)
    {
    case ACTION_RIGHT:

        break;
    case ACTION_LEFT:

        break;
    case ACTION_FIRE:

        break;
    default:
        break;
    }
}

void firePlayerBullet()
{
    initPlayerBullet(&Game.playerBullet, player.sprite.x + 6, player.sprite.y - 8,
                     2, 6, &bulletSptr_0_0);
}

unsigned char checkInput()
{
    unsigned char action;
    unsigned char value = readGamepad(GAMEPAD_1);

    if ((value & BUTTON_RIGHT) || readKeyboard(ROW_KEY_P) & KEY_P)
    {
        // Move player right
        action = ACTION_RIGHT;
    }
    else
    {

        if (value & BUTTON_LEFT || readKeyboard(ROW_KEY_Q) & KEY_Q)
        {
            // Move player left
            action = ACTION_LEFT;
        }
        else
        {

            if (value & BUTTON_A || readKeyboard(ROW_KEY_SPACE) & KEY_SPACE)
            {
                // Fire
                action = ACTION_FIRE;
            }
            else
            {
                // Do Nothing
                action = ACTION_NONE;
            }
        }
    }
    return action;
}

void moveEnemies()
{
    unsigned char i,j;
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (Game.enemies[i].visible == VISIBLE)
        {
            j=0;
            do{
                moveEnemy(&Game.enemies[i]);
                j++;
            }while(j<Game.level);
            
        }
    }
}

void moveEnemy(enemy *enem)
{
    switch (Game.direction)
    {
    case ENEMY_RIGHT:
        move_sprite_right(&enem->sprite);
        break;
    case ENEMY_LEFT:
        move_sprite_left(&enem->sprite);
        break;
    case ENEMY_DOWN:
        move_sprite_down(&enem->sprite);
        break;
    default:
        break;
    }
}

void drawEnemies()
{
    unsigned char i;
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (Game.enemies[i].visible == VISIBLE)
        {
            draw_sprite(&Game.enemies[i].sprite);
        }
    }
}

void drawPlayerBullet()
{
    if (player.action == ACTION_FIRE)
    {
        if (Game.playerBullet.visible == NOT_VISIBLE)
        {
            clean_sprite(&Game.playerBullet.sprite);
            firePlayerBullet();
            draw_sprite(&Game.playerBullet.sprite);
            Game.playerBullet.visible = VISIBLE;
        }

        
    }
}

void movePlayer()
{
    switch (player.action)
    {
    case ACTION_RIGHT:
        if (player.sprite.x < 112)
        {
            move_sprite_right(&player.sprite);
            move_sprite_right(&player.sprite);
        }
        break;
    case ACTION_LEFT:
        if (player.sprite.x > 0)
        {
            move_sprite_left(&player.sprite);
            move_sprite_left(&player.sprite);
        }
        break;

    default:
        break;
    }
}

void movePlayerBullets()
{
    unsigned char i;

    if (Game.playerBullet.visible == VISIBLE)
    {
        i = 0;
        do
        {
            // Delete Sprite if reach end of screen
            if (Game.playerBullet.sprite.y == 1)
            {
                Game.playerBullet.visible = NOT_VISIBLE;
                clean_sprite(&Game.playerBullet.sprite);
                break;
            }
            else
            {
                move_sprite_up(&Game.playerBullet.sprite);
            }
            i++;
        } while (i < BULLET_SPEED);
    }
}

void moveInvadersBullets()
{
    unsigned char i;
    Bullet *bullet;
    for(i=0; i<MAX_BULLETS; i++) {
        bullet = &(Game.enemiesBullets[i]);
        if(bullet->visible == VISIBLE) {
            // Delete Sprite if reach end of screen
            if (bullet->sprite.y > 120) {
                Game.playerBullet.visible = NOT_VISIBLE;
                clean_sprite(bullet);
            }
            else
            {
                move_sprite_down(bullet);
            }
        }
    }
}

void checkCols()
{
    unsigned char j;
    long points;

    if (Game.playerBullet.visible == VISIBLE)
    {
        for (j = 0; j < MAX_ENEMIES; j++)
        {
            if (Game.enemies[j].visible == VISIBLE && check_collisions(&Game.playerBullet.sprite, &Game.enemies[j].sprite))
            {
                Game.playerBullet.visible = NOT_VISIBLE;
                Game.enemies[j].visible = NOT_VISIBLE;
                clean_sprite(&Game.playerBullet.sprite);
                clean_sprite(&Game.enemies[j].sprite);
                // red 1 i< 8
                if (j < 8)
                {
                    points = 10;
                    addBCD(&(Game.score), &points);
                }
                // green i < 16
                else if (j < 16)
                {
                    points = 30;
                    addBCD(&(Game.score), &points);
                }
                // yellow i < 24
                else if (j < 24)
                {
                    points = 40;
                    addBCD(&(Game.score), &points);
                }
                // Second Red i < 32
                else
                {
                    points = 10;
                    addBCD(&(Game.score), &points);
                }
            }
        }
    }

    checkEnemyCols();
    checkEndLevel();
}

void checkEnemyCols()
{
    unsigned char i;
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (Game.enemies[i].visible == VISIBLE)
        {
            if (check_collisions(&Game.enemies[i].sprite, &player.sprite) || Game.enemies[i].sprite.y>120)
            {
                player.lives--;
                if (player.lives <= 0)
                {
                    Game.status = GAMEOVER;
                }
                else
                {
                    // Restart Game
                    restartGame();
                }
            }
        }
    }
}

void restartGame()
{
    // remove all sprites
    unsigned char i;
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (Game.enemies[i].visible == VISIBLE)
        {
            clean_sprite(&Game.enemies[i].sprite);
        }
    }
    // move sprites coords
    //  red 1
    for (i = 0; i < 8; i++)
    {
        initEnemy(&Game.enemies[i], 5 + (i * 14), 40, 10, 8, &redSptr_0_0);
    }
    // green
    for (i = 8; i < 16; i++)
    {
        initEnemy(&Game.enemies[i], 5 + ((i - 8) * 14), 27, 10, 8, &greenSptr_0_0);
    }
    // yellow
    for (i = 16; i < 24; i++)
    {
        initEnemy(&Game.enemies[i], 5 + ((i - 16) * 14), 12, 10, 8, &yellowSptr_0_0);
    }
    // Second Red
    for (i = 24; i < 32; i++)
    {
        initEnemy(&Game.enemies[i], 5 + ((i - 24) * 14), 52, 10, 8, &redSptr_0_0);
    }
    // redraw sprites
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (Game.enemies[i].visible == VISIBLE)
        {
            draw_sprite(&Game.enemies[i].sprite);
        }
    }
    Game.enemiesSteps=0;
    for(i=0; i<MAX_BULLETS; i++)
    {
        Game.enemiesBullets[i].visible=0;
    }
}

void checkEndLevel(){
    unsigned char i;
    unsigned char status=NOT_VISIBLE;
    for(i=0;i<MAX_ENEMIES;i++){
        if(Game.enemies[i].visible==VISIBLE){
            status=VISIBLE;
        }
    }
    //IF the status is not visible the level is complete; go to next level
    if(status==NOT_VISIBLE){
        Game.level++;
        for(i=0;i<MAX_ENEMIES;i++){
            Game.enemies[i].visible=VISIBLE;
        }
        restartGame();
    }

}
