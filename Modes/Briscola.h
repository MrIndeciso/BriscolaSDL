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

#endif // BRISCOLA_H_INCLUDED
