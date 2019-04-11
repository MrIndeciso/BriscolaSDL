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


SDL_Texture* briscolaBG;
int briscolaLoop = 0;
int playingPlayer = 0;

FPSCounter fpsCtr;

Mazzo jiisus;

Carta emptyCarta; //Let's see if I can scam the system

Carta banco[2];

gGUI briscolaGUI;

gGUI cards;

enum currentState {
	P1WON,
	P2WON,
	START,
	END
};

Player pl1, pl2;
Player *pls[2] = {&pl1, &pl2}; //Used almost everywhere

//Music stuff
elemGUI volupb, voldwb, volmutb, volmut2b;

//That thing with a 6 line comment stuff
elemGUI txcard1, txcard2, txcard3;

//Button to confirm the choice
elemGUI confirmBttPL1, confirmBttPL2;

int initBriscola(){

	briscolaBG = loadTexture("Assets/BG/wooden.bmp", mainRenderer);

	initMazzo(&jiisus, mainRenderer);

	initFPS(&fpsCtr, mainRenderer, 1, FPSTR);

	initRan();

	scrambleMazzo(&jiisus);

	loadMusicElements();

	instaGUIelem();

	otherElements();

	giveFirstCards();

	emptyCarta.num = 42;

	banco[0] = emptyCarta;
	banco[1] = emptyCarta;

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

		SDL_RenderCopy(mainRenderer, briscolaBG, NULL, NULL);

		drawFPS(&fpsCtr, SDL_GetTicks());
		drawGUI(briscolaGUI, mainRenderer);

		//drawGUI(cards, mainRenderer);
		drawPLCards();

		drawHand();

		SDL_RenderPresent(mainRenderer);

		while(SDL_PollEvent(&e)!=0){

				stdEventInput(&briscolaLoop, e, mainWindow); //No function overloading but at least we got the Chinese version of "pass by reference"

				gInput(briscolaGUI, e);

				gInput(cards, e);

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

	jiisus.currentNum += 6;

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

int otherElements(){
	confirmBttPL1 = createElement(1, GUI_LABEL, (SDL_Rect){625, 320, 100, 40}, (SDL_Color){0,0,0,0},
															loadFromText("Confirm", (SDL_Color){255, 0, 0, 0}, mainRenderer,
															"Assets/Font/comicz.ttf", 200), &confirmCard);

	confirmBttPL2 = createElement(0, GUI_LABEL, (SDL_Rect){625, 320, 100, 40}, (SDL_Color){0,0,0,0},
															loadFromText("Confirm", (SDL_Color){0, 255, 0, 0}, mainRenderer,
															"Assets/Font/comicz.ttf", 200), &confirmCard);


	addElement(&briscolaGUI, &confirmBttPL1);
	addElement(&briscolaGUI, &confirmBttPL2);

	return 0;

}

int drawPLCards(){

	SDL_RenderCopy(mainRenderer, jiisus.texture, &pls[playingPlayer]->mano[0].pos, &(SDL_Rect){250, 300, 100, 220});
	SDL_RenderCopy(mainRenderer, jiisus.texture, &pls[playingPlayer]->mano[1].pos, &(SDL_Rect){350, 300, 100, 220});
	SDL_RenderCopy(mainRenderer, jiisus.texture, &pls[playingPlayer]->mano[2].pos, &(SDL_Rect){450, 300, 100, 220});

	SDL_RenderCopy(mainRenderer, jiisus.texture, &banco[0].pos, &(SDL_Rect){310, 80, 80, 180});

	SDL_RenderCopy(mainRenderer, jiisus.texture, &banco[1].pos, &(SDL_Rect){410, 80, 80, 180});

	return 0;

}

int clickCard1(int x, int y, int ptr){ banco[playingPlayer] = pls[playingPlayer]->mano[0]; pls[playingPlayer]->chosenNum = 0; }
int clickCard2(int x, int y, int ptr){ banco[playingPlayer] = pls[playingPlayer]->mano[1]; pls[playingPlayer]->chosenNum = 1; }
int clickCard3(int x, int y, int ptr){ banco[playingPlayer] = pls[playingPlayer]->mano[2]; pls[playingPlayer]->chosenNum = 2; }

int confirmCard(int x, int y, int ptr){

	if(banco[1].num == emptyCarta.num || banco[0].num == emptyCarta.num) playingPlayer = !playingPlayer;
	else {

		if(banco[0].seed == banco[1].seed){
			if(banco[0].take>banco[1].take){
				//PL1 wins
				printf("DebuggoniPT1\n");

				pl1Wins();

			} else if(banco[0].take<banco[1].take){
				//PL2 wins
				printf("DebuggoniV2PT1\n");

				pl2Wins();

			} else {
				printf("Dang I am bad and this is wrong\n");
			}
		} else if(banco[0].seed == jiisus.briscola.seed){
			//PL1 wins
			printf("DebuggoniPT2\n");

			pl1Wins();

		} else if(banco[1].seed == jiisus.briscola.seed){
			//PL2 wins
			printf("DebuggoniV2PT2\n");

			pl2Wins();

		} else if(playingPlayer){
			//PL1 wins
			printf("DebuggoniPT3\n");

			pl1Wins();

		} else if(!playingPlayer){
			//PL2 wins
			printf("DebuggoniV2PT3\n");

			pl2Wins();

		} else {
			printf("Dang I am bad and this is wrong\n");
		}

	}

	//confirmBttPL1.active = 0;
	//confirmBttPL2.active = 0;
	//if(!playingPlayer) confirmBttPL1.active = 1;
	//else if (playingPlayer) confirmBttPL2.active = 1;

	SDL_Delay(500); //Yo this seems cool

	return 0;

}

int pl1Wins(){

	pl1.mucchio[pl1.mucchioNum] = banco[0];
	++pl1.mucchioNum;
	pl1.mucchio[pl1.mucchioNum] = banco[1];
	++pl1.mucchioNum;

	pl1.mano[pl1.chosenNum] = jiisus.carte[jiisus.currentNum];
	++jiisus.currentNum;

	pl2.mano[pl2.chosenNum] = jiisus.carte[jiisus.currentNum];
	++jiisus.currentNum;

	banco[0] = emptyCarta;
	banco[1] = emptyCarta;

	playingPlayer = 0;

	return 0;

}

int pl2Wins(){

	pl2.mucchio[pl2.mucchioNum] = banco[0];
	++pl2.mucchioNum;
	pl2.mucchio[pl2.mucchioNum] = banco[1];
	++pl2.mucchioNum;

	pl2.mano[pl2.chosenNum] = jiisus.carte[jiisus.currentNum];
	++jiisus.currentNum;

	pl1.mano[pl1.chosenNum] = jiisus.carte[jiisus.currentNum];
	++jiisus.currentNum;

	banco[0] = emptyCarta;
	banco[1] = emptyCarta;

	playingPlayer = 1;

	return 0;

}

//Dont delete, very important, program breaks and I don't know why (cit.)
int doNothing(int x, int y, Uint32 ptr) {return 0;}
