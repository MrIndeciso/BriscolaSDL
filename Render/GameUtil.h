#ifndef GAMEUTIL_H_INCLUDED
#define GAMEUTIL_H_INCLUDED

#include "../Lib/SDL/include/SDL.h"

typedef struct Carta {
  int num;
  int value;
  int seed;
  int take;
  char name[32];
  int absPos;
  SDL_Rect pos;
} Carta;

typedef struct Mazzo {
  Carta carte[40];
  int currentNum;
  SDL_Texture *texture;
  Carta briscola;
  SDL_Rect mazzoGirato;
} Mazzo;

typedef struct Player {
  Carta mano[3];
  Carta mucchio[40];
  int mucchioNum;
  int chosenNum;
  int punti;
  char nome[15];
} Player;

//Loads textures and that's all
int initMazzo(Mazzo *mazzo, SDL_Renderer *renderer);

//Scrambles the cards
int scrambleMazzo(Mazzo *mazzo);

//Initializes the random number generator by setting the seed
int initRan();

//Number from 0 to max-1
int genRan(int max);

//Number from min to max-1
int genRanMin(int min, int max);

#endif
