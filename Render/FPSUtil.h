#ifndef FPSUTIL_H_INCLUDED
#define FPSUTIL_H_INCLUDED

#include "../Lib/SDL/include/SDL.h"

enum FPSPOS {
  FPSTR, //Top right
  FPSTL, //Top left
  FPSBR, //Bottom right
  FPSBL, //Bottom left
  FPSPOSCOUNT
};

typedef struct FPSCounter {
    SDL_Texture *text;
    SDL_Renderer* renderer;
    int showText;
    int pos;
    SDL_Texture *nums[10];
    int counter;
    Uint32 lastTicks;
    int textSize;
    int fpsNum;
} FPSCounter;

//Not fully implemented but it's ok
int initFPS(FPSCounter* fps, SDL_Renderer* render, int text, int pos);

int drawFPS(FPSCounter* fps, Uint32 ticks);

#endif
