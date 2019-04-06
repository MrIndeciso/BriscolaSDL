#include "FPSUtil.h"
#include "../Lib/SDL/include/SDL.h"
#include "RenderUtil.h"

extern int SCR_WIDTH;
extern int SCR_HEIGHT;
extern int SCR_LWIDTH;
extern int SCR_LHEIGHT;

int initFPS(FPSCounter* fps, SDL_Renderer* render, int text, int pos){

  fps->renderer = render;
  fps->showText = text;
  fps->pos = pos;
  fps->lastTicks = SDL_GetTicks() + 1000;
  fps->fpsNum = 0;

  int i;
  char c[10][2] = {"0\0", "1\0", "2\0", "3\0", "4\0", "5\0", "6\0", "7\0", "8\0", "9\0"};

  for(i=0;i<10;i++){
    fps->nums[i] = loadFromText(c[i], (SDL_Color){255, 255, 0, 255}, render, "Assets/Font/joystix_monospace.ttf", 32);
  }

  fps->text = loadFromText("FPS: ", (SDL_Color){255, 255, 0, 255}, render, "Assets/Font/joystix_monospace.ttf", 32);

  return 0;

}

int drawFPS(FPSCounter* fps, Uint32 ticks){

  int size = 16;
  SDL_Rect blyat;

  blyat.x = SCR_WIDTH - size;
  blyat.y = 0;
  blyat.w = size;
  blyat.h = 24;

  if((ticks-fps->lastTicks)>1000){

    fps->fpsNum = fps->counter / ((ticks-(fps->lastTicks))/1000);
    fps->lastTicks = ticks;
    fps->counter = 0;

  }

  int ctr = fps->fpsNum;

  while(ctr){

    blyat.x -= size;
    SDL_RenderCopy(fps->renderer, fps->nums[ctr%10], NULL, &blyat);
    ctr /= 10;

  }

  blyat.x -= 5*size;
  blyat.w =  5*size;

  SDL_RenderCopy(fps->renderer, fps->text, NULL, &blyat);

  ++fps->counter;

  return 0;

}
