#ifndef BRISCOLA_H_INCLUDED
#define BRISCOLA_H_INCLUDED

#include "../Lib/SDL/include/SDL.h"

int initBriscola();

int mainBriscolaLoop();

//Music stuff starts here
int loadMusicElements();
int volPlusB(int x, int y, Uint32 ptr);
int volMinusB(int x, int y, Uint32 ptr);
int muteAudioB(int x, int y, Uint32 ptr);
//Music stuff luckily ends here

int clickCard1(int x, int y, int ptr);
int clickCard2(int x, int y, int ptr);
int clickCard3(int x, int y, int ptr);

int confirmCard(int x, int y, int ptr);

int doNothing(int x, int y, Uint32 ptr);

int drawPLCards();

int pl1Wins();
int pl2Wins();

int instaGUIelem();
int drawHand();

int otherElements();

int giveFirstCards();

#endif // BRISCOLA_H_INCLUDED
