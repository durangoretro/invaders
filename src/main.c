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
    player.lives = 3;
    player.sprite.resource = &playerSptr_0_0;
    player.sprite.width = 32;
    player.sprite.width = 16;
    player.sprite.x = 56;
    player.sprite.y = 115;
    drawFullScreen(BLACK);
    drawPlayer(&player);
}

void update()
{
    switch (Game.status)
    {
    case 0: // TODO Start
    case 1:
        /* code */
        break;
    case 2: // TODO Game Over
    default:
        break;
    }
}

void draw()
{
    switch (Game.status)
    {
    case 0: // TODO Start
    case 1:
        /* code */

        break;
    case 2: // TODO Game Over
    default:
        break;
    }
}

void drawPlayer()
{
    unsigned char i;
    clrscr();
    calculate_coords(&player.sprite);
    draw_sprite(&player.sprite);
    //Red
    for(i=0;i<8;i++){
        Game.enemies[i].sprite.resource = &redSptr_0_0;
        Game.enemies[i].sprite.x = 5+(i*14);
        Game.enemies[i].sprite.y = 30;
        Game.enemies[i].sprite.width = 10;
        Game.enemies[i].sprite.height = 8;
        draw_sprite(&Game.enemies[i].sprite);
    }
    //green
    for(i=8;i<16;i++){
        Game.enemies[i].sprite.resource = &greenSptr_0_0;
        Game.enemies[i].sprite.x = 5+((i-8)*14);
        Game.enemies[i].sprite.y = 17;
        Game.enemies[i].sprite.width = 10;
        Game.enemies[i].sprite.height = 8;
        draw_sprite(&Game.enemies[i].sprite);
    }
    //yellow
    for(i=16;i<24;i++){
        Game.enemies[i].sprite.resource = &yellowSptr_0_0;
        Game.enemies[i].sprite.x = 5+((i-16)*14);
        Game.enemies[i].sprite.y = 2;
        Game.enemies[i].sprite.width = 10;
        Game.enemies[i].sprite.height = 8;
        draw_sprite(&Game.enemies[i].sprite);
    }
    //Second Red
     for(i=24;i<32;i++){
        Game.enemies[i].sprite.resource = &redSptr_0_0;
        Game.enemies[i].sprite.x = 5+((i-24)*14);
        Game.enemies[i].sprite.y = 42;
        Game.enemies[i].sprite.width = 10;
        Game.enemies[i].sprite.height = 8;
        draw_sprite(&Game.enemies[i].sprite);
    }
}