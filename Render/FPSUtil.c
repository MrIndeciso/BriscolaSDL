#include "FPSUtil.h"
#include "../Lib/SDL/include/SDL.h"
#include "RenderUtil.h"

extern int SCR_WIDTH;
extern int SCR_HEIGHT;

int initFPS(FPSCounter* fps, SDL_Renderer* render, int text, int pos){

  fps->renderer = render;
  fps->showText = text;
  fps->pos = pos;
  fps->lastTicks = SDL_GetTicks() + 1000;
  fps->fpsNum = 0;

  int i;
  char c[5];
  for(i=0;i<10;i++){
    snprintf(&c, 2, "%d\0", i);
    fps->nums[i] = loadFromText(c, (SDL_Color){255, 255, 0, 255}, render, "Assets/Font/joystix_monospace.ttf", 32);
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

  blyat.x -= 4*size;
  blyat.w =  4*size;

  SDL_RenderCopy(fps->renderer, fps->text, NULL, &blyat);

  ++fps->counter;

  return 0;

}
