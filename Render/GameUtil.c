#include "GameUtil.h"
#include "RenderUtil.h"
#include <time.h>
#include <stdio.h> //Just for debug purposes

int initMazzo(Mazzo *mazzo, SDL_Renderer *renderer){
	//Cuori, quadri, fiori, picche
	//Ordine 1-10
	const int mapCardPos[40] = {
										3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
										14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
										1, 2, 13, 24, 37, 40, 35, 36, 38, 39,
										25, 26, 27, 28, 29, 30, 31, 32, 33, 34
	}; //If you want to understand this, just have a look at those card textures that we absolutely didn't steal

	const int startX = 155; //Number of pixels we gotta skip

  const int cardTake[10] = {9, 0, 8, 1, 2, 3, 4, 5, 6, 7};
  const int cardValue[10] = {11, 0, 10, 0, 0, 0, 0, 2, 3, 4};

  Carta tempCarte[40];

  int i;

  SDL_Rect texturePos;
  texturePos.h = 454;
  texturePos.w = 234;


  for(i=0;i<40;i++){
    mazzo->carte[i].num = i%10;
    mazzo->carte[i].value = cardValue[i%10];
    mazzo->carte[i].seed = i/10;
    mazzo->carte[i].take = cardTake[i%10];
    mazzo->carte[i].absPos = i;
    texturePos.x = 310 + ((mapCardPos[i]-1)/3 * texturePos.w);
    texturePos.y = (mapCardPos[i]-1)%3 * texturePos.h;
    mazzo->carte[i].pos = texturePos;
  }

  mazzo->texture = loadTexture("Assets/Briscola/Milanesi.bmp", renderer);
	mazzo->mazzoGirato = (SDL_Rect){0, 0, 310, 566};

  return 0;

}

int scrambleMazzo(Mazzo *mazzo){

	int i, j, temp, new;
	int randoms[40];

	//Genera una lista con ogni numero da 0 a 40 escluso in ordine casuale
	for(i=0;i<40;new=0){
		temp = genRan(40);
		for(j=0;j<i;j++) if(randoms[j]==temp) ++new;
		randoms[i] = temp;
		if(!new) ++i;
	}	//Gurini triggered

	Mazzo tempMazzo = *mazzo;

	for(i=0;i<40;i++){
		mazzo->carte[i] = tempMazzo.carte[randoms[i]];
	}

	mazzo->briscola = mazzo->carte[39];

	return 0;

}

int initRan() { srand(time(NULL)); }

int genRan(int max) { return rand()%max; }

int genRanMin(int min, int max) { return rand()%(max-min) + min; }
