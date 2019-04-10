#include "Briscola.h"
#include "../Render/GameUtil.h"
#include "../Lib/SDL/include/SDL_ttf.h"
#include "../Render/FPSUtil.h"
#include "../Render/GUIUtil.h"
#include "../Render/RenderUtil.h"
#include "../Render/NetUtil.h"
#include <stdio.h>

//They have to be the same from the main loop
extern SDL_Renderer *mainRenderer;
extern SDL_Window* mainWindow;

//Just SDL stuff
SDL_Event e;


int briscolaLoop = 0;
int playingPlayer = 0;

FPSCounter fpsCtr;

Mazzo jiisus;

gGUI briscolaGUI;

gGUI cards;

enum currentState {
	P1WON,
	P2WON,
	START,
	END
};

Player pls[2];
Player pl1, pl2;

//Music stuff
elemGUI volupb, voldwb, volmutb, volmut2b;

//That thing with a 6 line comment stuff
elemGUI txcard1, txcard2, txcard3;

int initBriscola(){

	pls[0] = pl1;
	pls[1] = pl2;

	initMazzo(&jiisus, mainRenderer);

	initFPS(&fpsCtr, mainRenderer, 1, FPSTR);

	initRan();

	scrambleMazzo(&jiisus);

	loadMusicElements();

	instaGUIelem();

	giveFirstCards();

	pls[0] = pl1;
	pls[1] = pl2;

	mainBriscolaLoop();

	return 0;

}

//Music stuff starts here
int loadMusicElements(){
	volmutb = createElement(0, GUI_IMAGE, (SDL_Rect){780, 430, 20, 20}, (SDL_Color){0,0,0,0},
												loadTexture("Assets/Sound/music.bmp", mainRenderer), &muteAudioB);
	volmut2b = createElement(1, GUI_IMAGE, (SDL_Rect){720, 430, 20, 20}, (SDL_Color){0,0,0,0},
												loadTexture("Assets/Sound/mute.bmp", mainRenderer), &muteAudioB);
	volupb = createElement(1, GUI_IMAGE, (SDL_Rect){760, 430, 20, 20}, (SDL_Color){0,0,0,0},
												loadTexture("Assets/Sound/high.bmp", mainRenderer), &volPlusB);
	voldwb = createElement(1, GUI_IMAGE, (SDL_Rect){740, 430, 20, 20}, (SDL_Color){0,0,0,0},
												loadTexture("Assets/Sound/low.bmp", mainRenderer), &volMinusB);

	addElement(&briscolaGUI, &volmutb);
	addElement(&briscolaGUI, &volmut2b);
	addElement(&briscolaGUI, &volupb);
	addElement(&briscolaGUI, &voldwb);

	return 0;
}
/*I could probably handle this in a different way, since the following functions are basically identical to those
	in briscola.c, but I prefer keeping them different in case I want to change their behaviour quickly */
int volPlusB(int x, int y, Uint32 ptr) {Mix_VolumeMusic(Mix_VolumeMusic(-1) + 2);}
int volMinusB(int x, int y, Uint32 ptr) {Mix_VolumeMusic(Mix_VolumeMusic(-1) - 2);}
int muteAudioB(int x, int y, Uint32 ptr) {
  Mix_PausedMusic()?Mix_ResumeMusic():Mix_PauseMusic();
  volmutb.active = Mix_PausedMusic();
  volmut2b.active = !Mix_PausedMusic();
}
//Music stuff luckily ends here

int mainBriscolaLoop(){

	int i = 0;

	while(!briscolaLoop){

		SDL_RenderClear(mainRenderer);

		drawFPS(&fpsCtr, SDL_GetTicks());
		drawGUI(briscolaGUI, mainRenderer);

		//drawGUI(cards, mainRenderer);
		drawPLCards();

		drawHand();

		SDL_RenderPresent(mainRenderer);

		while(SDL_PollEvent(&e)!=0){

				stdEventInput(&briscolaLoop, e, mainWindow); //No function overloading but at least we got the Chinese version of "pass by reference"

				gInput(briscolaGUI, e);

		}

	}

	return 0;

}

int giveFirstCards(){

	int i;
	for(i=5;i>0;i-=2){
		pl1.mano[i/2] = jiisus.carte[i];
		pl2.mano[i/2] = jiisus.carte[i-1];
	} //Gurini triggered PT2

	return 0;

}

int drawHand(){

	SDL_RenderCopy(mainRenderer, jiisus.texture, &jiisus.mazzoGirato, &(SDL_Rect){310, -66, 60, 132});
	SDL_RenderCopy(mainRenderer, jiisus.texture, &jiisus.mazzoGirato, &(SDL_Rect){370, -66, 60, 132});
	SDL_RenderCopy(mainRenderer, jiisus.texture, &jiisus.mazzoGirato, &(SDL_Rect){430, -66, 60, 132});

	SDL_RenderCopyEx(mainRenderer, jiisus.texture, &jiisus.briscola.pos, &(SDL_Rect){60, 130, 100, 190}, 90, NULL, SDL_FLIP_NONE);

	SDL_RenderCopy(mainRenderer, jiisus.texture, &jiisus.mazzoGirato, &(SDL_Rect){10, 130, 100, 190});

	return 0;

}

//Here starts the real business

int instaGUIelem(){
		/* Principle of operation:
		Runs every time something happens that changes cards or player
		Manages 3 elements
		Basically a second GUI renderer just draws these 3 elements
		The elements have to change every time
		It's not hard but I still wanted to write a comment for this */

		cards.elemCount = 0;

		txcard1 = createElement(1, GUI_IMAGE, (SDL_Rect){250, 300, 100, 220}, (SDL_Color){0,0,0,0},
														jiisus.texture, &clickCard1);
		txcard2 = createElement(1, GUI_IMAGE, (SDL_Rect){350, 300, 100, 220}, (SDL_Color){0,0,0,0},
														jiisus.texture, &clickCard2);
		txcard3 = createElement(1, GUI_IMAGE, (SDL_Rect){450, 300, 100, 220}, (SDL_Color){0,0,0,0},
														jiisus.texture, &clickCard3);

		addElement(&cards, &txcard1);
		addElement(&cards, &txcard2);
		addElement(&cards, &txcard3);

		return 0;

}

int drawPLCards(){

	SDL_RenderCopy(mainRenderer, jiisus.texture, &pls[playingPlayer].mano[0].pos, &(SDL_Rect){250, 300, 100, 220});
	SDL_RenderCopy(mainRenderer, jiisus.texture, &pls[playingPlayer].mano[1].pos, &(SDL_Rect){350, 300, 100, 220});
	SDL_RenderCopy(mainRenderer, jiisus.texture, &pls[playingPlayer].mano[2].pos, &(SDL_Rect){450, 300, 100, 220});

	return 0;

}

int clickCard1(int x, int y, int ptr){}
int clickCard2(int x, int y, int ptr){}
int clickCard3(int x, int y, int ptr){}

//Dont delete, very important, program breaks and I don't know why (cit.)
int doNothing(int x, int y, Uint32 ptr) {return 0;}
