/**
 * Invaders Game
 */

// Include Library
#include <durango.h>


#include "game.h"

// Resources
#include "playerSptr.h"
#include "redSptr.h"
#include "greenSptr.h"
#include "yellowSptr.h"
#include "bulletSptr.h"


// Functions Prototype
void startGame();
void updateEnemies();

void drawEnemies();

void moveEnemies();

void firePlayerBullet();
void drawPlayerBullet();
void movePlayerBullets();

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

    drawFullScreen(BLACK);
}

// Update Function
void update()
{
    switch (Game.status)
    {
    case 0: // TODO Start
    case 1: // Game Init
        startGame();
        break;
    case 2: // TODO Game Loop
        updateEnemies();
        // Enemy Direction
        if (Game.direction == ENEMY_DOWN && Game.enemies[0].sprite.x > 20)
        {
            Game.direction = ENEMY_LEFT;
        }
        if (Game.direction == ENEMY_DOWN && Game.enemies[0].sprite.x <= 0)
        {
            Game.direction = ENEMY_RIGHT;
        }
        if (Game.direction == ENEMY_LEFT && Game.enemies[0].sprite.x <= 0)
        {
            Game.direction = ENEMY_DOWN;
        }
        if (Game.direction == ENEMY_RIGHT && Game.enemies[0].sprite.x > 20)
        {
            Game.direction = ENEMY_DOWN;
        }
        // Input and Player move
        player.action = checkInput();
        updatePlayer();
        updatePlayerBullets();
    case 3: // Game Over
    default:
        break;
    }
}

void updatePlayerBullets()
{
    unsigned char i;
    for (i = 0; i < MAX_BULLETS; i++)
    {
        if (Game.playerBullets[i].visible == VISIBLE)
        {
            Game.playerBullets[i].sprite.y-=4;
        }
    }
}

void draw()
{
    switch (Game.status)
    {
    case 0: // TODO Start
    case 1: // TODO: GAME_INIT
        drawEnemies();
        drawPlayer(&player);
        Game.status = 2; // To Game Loop

        break;
    case 2:
        moveEnemies();
        movePlayer();
        drawPlayerBullet();
        movePlayerBullets();
        break;
    case 3: // TODO: Game Over
    default:
        break;
    }
}

void drawPlayer()
{
    draw_sprite(&player.sprite);
}

void initEnemy(enemy *output, unsigned char x, unsigned char y, unsigned char width, unsigned char height, void *resource)
{
    output->lives = 1;
    output->sprite.x = x;
    output->sprite.y = y;
    output->sprite.width = width;
    output->sprite.height = height;
    output->sprite.resource = resource;
    output->visible = VISIBLE;
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

void startGame()
{
    unsigned char i;
    // init Player
    player.lives = 3;
    player.sprite.resource = &playerSptr_0_0;
    player.sprite.width = 16;
    player.sprite.height = 8;
    player.sprite.x = 56;
    player.sprite.y = 115;
    // Init game Bullets
    Game.currentBullet = 0;
    Game.currentEnemyBullet = 0;
    // red 1
    for (i = 0; i < 8; i++)
    {
        initEnemy(&Game.enemies[i], 5 + (i * 14), 30, 10, 8, &redSptr_0_0);
    }
    // green
    for (i = 8; i < 16; i++)
    {
        initEnemy(&Game.enemies[i], 5 + ((i - 8) * 14), 17, 10, 8, &greenSptr_0_0);
    }
    // yellow
    for (i = 16; i < 24; i++)
    {
        initEnemy(&Game.enemies[i], 5 + ((i - 16) * 14), 2, 10, 8, &yellowSptr_0_0);
    }
    // Second Red
    for (i = 24; i < 32; i++)
    {
        initEnemy(&Game.enemies[i], 5 + ((i - 24) * 14), 42, 10, 8, &redSptr_0_0);
    }
    Game.direction = ENEMY_RIGHT;
}

void updateEnemies()
{
    unsigned char i;
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        updateEnemy(&Game.enemies[i]);
    }
}

void updateEnemy(enemy *enem)
{
    switch (Game.direction)
    {
    case ENEMY_RIGHT:
        enem->sprite.x++;
        break;
    case ENEMY_LEFT:
        enem->sprite.x--;
        break;
    case ENEMY_DOWN:
        enem->sprite.y++;
        break;
    default:
        break;
    }
}

void updatePlayer()
{
    switch (player.action)
    {
    case ACTION_RIGHT:
        player.sprite.x++;
        break;
    case ACTION_LEFT:
        player.sprite.x--;
        break;
    case ACTION_FIRE:
        firePlayerBullet();
        break;
    default:
        break;
    }
}

void firePlayerBullet()
{
    Bullet *bullet;
    bullet = &Game.playerBullets[Game.currentBullet];
    initPlayerBullet(bullet, player.sprite.x+4, player.sprite.y - 8,
                     8, 10, &bulletSptr_0_0);
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
    unsigned char i;
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        moveEnemy(&Game.enemies[i]);
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
        draw_sprite(&Game.playerBullets[Game.currentBullet].sprite);
        Game.playerBullets[Game.currentBullet].visible = VISIBLE;
        Game.currentBullet = Game.currentBullet++ % MAX_BULLETS;
    }
}

void movePlayer()
{
    switch (player.action)
    {
    case ACTION_RIGHT:
        move_sprite_right(&player.sprite);
        break;
    case ACTION_LEFT:
        move_sprite_left(&player.sprite);
        break;

    default:
        break;
    }
}

void movePlayerBullets()
{
    unsigned char i;
    for (i = 0; i < MAX_BULLETS; i++)
    {
        if (Game.playerBullets[i].visible == VISIBLE)
        {
            // Delete Sprite if reach end of screen
            if (Game.playerBullets[i].sprite.y <= 8)
            {
                clean_sprite(&Game.playerBullets[i].sprite);
                Game.playerBullets[i].visible = NOT_VISIBLE;
            }
            
            move_sprite_up(&Game.playerBullets[i].sprite);
            move_sprite_up(&Game.playerBullets[i].sprite);
            move_sprite_up(&Game.playerBullets[i].sprite);
            move_sprite_up(&Game.playerBullets[i].sprite);
            move_sprite_up(&Game.playerBullets[i].sprite);
        }
    }
}
