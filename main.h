#include "SDL2/SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FPS 24
#define SCREEN_INIT_WIDTH 360
#define SCREEN_INIT_HEIGHT 240
#define MY_TOTAL_Button 3
#define MY_BUTTON_Width SCREEN_INIT_WIDTH / MY_TOTAL_Button / 2
#define MY_BUTTON_Hight SCREEN_INIT_HEIGHT / 2



enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_DOWN = 1,
    BUTTON_SPRITE_MOUSE_TOTAL = 2
};

struct LButton
{
    SDL_Point mPosition;
    int index;
    enum LButtonSprite mCurrentSprite;
};