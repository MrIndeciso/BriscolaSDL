#include "Briscola.h"
#include "../Render/GameUtil.h"
#include "../Lib/SDL/include/SDL_ttf.h"
#include "../Render/FPSUtil.h"
#include "../Render/GUIUtil.h"
#include "../Render/RenderUtil.h"
#include "../Render/NetUtil.h"
#include <stdio.h>

extern SDL_Renderer *mainRenderer;
extern SDL_Window* mainWindow;

SDL_Event e;

int briscolaLoop = 0;

FPSCounter fpsCtr;

Mazzo jiisus;

gGUI briscolaGUI;

int initBriscola(){

	initMazzo(&jiisus, mainRenderer);

	int shish;
	for(shish=0;shish<40;shish++) printf("\n%d", jiisus.carte[shish].num);

	initFPS(&fpsCtr, mainRenderer, 1, FPSTR);

	initRan();

	scrambleMazzo(&jiisus);

	loadMusicElements();

	mainBriscolaLoop();

	return 0;

}

//Music stuff starts here
elemGUI volupb, voldwb, volmutb, volmut2b;
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

		SDL_RenderPresent(mainRenderer);

		while(SDL_PollEvent(&e)!=0){

				stdEventInput(&briscolaLoop, e, mainWindow); //No function overloading but at least we got the Chinese version of "pass by reference"

		}

	}

	return 0;

}
