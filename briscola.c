#include "Lib/SDL/include/SDL.h"
#include "Lib/SDL/include/SDL_mixer.h"
#include "Lib/SDL/include/SDL_ttf.h"
#include "Lib/SDL/include/SDL_net.h"
#include <stdio.h>

#include "Modes/Briscola.h"
#include "Render/RenderUtil.h"
#include "Render/GUIUtil.h"
#include "Render/GameUtil.h"
#include "Render/FPSUtil.h"

int mainInit();
int mainLoad();
int mainLoop();
int mainClose();

int loadBackground();
int loadMainScrBtts();

int loadMMAssets();
int handleClick(int, int, Uint32);
int quitGame(int, int, Uint32);
int volPlus(int, int, Uint32);
int volMinus(int, int, Uint32);
int muteAudio(int, int, Uint32);
int startBriscola(int, int, Uint32);

//The resolution that we render with
int SCR_WIDTH = 800;
int SCR_HEIGHT = 450;

//The initial game resolution
int SCR_LWIDTH = 800;
int SCR_LHEIGHT = 450;

SDL_Window* mainWindow = NULL;
SDL_Surface* mainScreenSurface = NULL;
SDL_Renderer* mainRenderer = NULL;
SDL_Texture* bgTexture;

FPSCounter fpsDraw;

SDL_Event e;

int breakLoop = 0;

elemGUI logo, btt1, btt2, btt3, volup, voldw, volmut, volmut2;
Mix_Music *bgMusic = NULL;

gGUI globalGUI;

int main(int argc, char* args[]){

    if(!mainInit()){ //Main program starts here

        //Load textures
        mainLoad();

        //Main rendering loop
        mainLoop();

    } else return -1;

    mainClose();

    return 0;

}

int mainInit(){ //This really makes no sense because with a return the "else" isn't needed but I'll fix it another time

    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)<0) {
        printf("%s", errorString[ERR_CANT_LOAD_SDL]);
        return -1;
    } else {
        mainWindow = SDL_CreateWindow("DEBUG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_LWIDTH, SCR_LHEIGHT, SDL_WINDOW_SHOWN);
        if(mainWindow == NULL) {
            printf("%s", errorString[ERR_CANT_CREATE_WINDOW]);
            return -1;
        } else {
	    //La riga successiva funziona su Winzozz, Sparky Linux ma non va su Fedora 29. That's illegal
	    //mainScreenSurface = SDL_GetWindowSurface(mainWindow);
            mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
            if(mainRenderer == NULL) {
                printf("%s", errorString[ERR_INVALID_RENDERER]);
		            printf("\n%s", SDL_GetError());
                return -1;
            } else {
                if(TTF_Init()==-1){
                  return -1;
                } else {
                    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 8192)<0){
                      return -1;
                    } else {

                    }
                }
            }
        }
    }



    //We use the timer to calculate the FPS obviously
    initFPS(&fpsDraw, mainRenderer, 1, FPSTR);

    SDLNet_Init();

    globalGUI.elemCount = 0;

    return 0;

}

int mainClose(){

    breakLoop = 1;

    TTF_Quit();

    Mix_CloseAudio();

    Mix_Quit();

    SDLNet_Quit();

    SDL_DestroyRenderer(mainRenderer);
    mainRenderer = NULL;

    SDL_DestroyWindow(mainWindow);
    mainWindow = NULL;

    SDL_Quit();

    return 0;
}

int mainLoop(){

    while(!breakLoop) {

        SDL_RenderClear(mainRenderer);

        SDL_RenderCopy(mainRenderer, bgTexture, NULL, NULL);
				drawGUI(globalGUI, mainRenderer);
        drawFPS(&fpsDraw, SDL_GetTicks());


        SDL_RenderPresent(mainRenderer);

        while(SDL_PollEvent(&e)!=0){

            stdEventInput(&breakLoop, e, mainWindow); //No function overloading but at least we got the Chinese version of "pass by reference"

            gInput(globalGUI, e);

        }

    }

    return 0;

}

int mainLoad(){

    SDL_RenderSetLogicalSize(mainRenderer, SCR_LWIDTH, SCR_LHEIGHT);

    //This is probably wrong
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    SDL_SetWindowResizable(mainWindow, SDL_TRUE);

    //SDL_SetRenderDrawColor(mainRenderer, 0x80, 0xFF, 0xFF, 0xFF); //Set background color

    bgTexture = loadTexture("Assets/BG/mainmenu.bmp", mainRenderer);

    loadMMAssets();

    return 0;

}

//Here starts the actual main screen stuff

int loadMMAssets(){ //Called in mainLoad

  logo = createElement(1, GUI_IMAGE, (SDL_Rect){300, 50, 200, 80}, (SDL_Color){0,0,0,0},
                        loadTexture("Assets/MM/Logo.bmp", mainRenderer), &handleClick);

  btt1 = createElement(1, GUI_LABEL, (SDL_Rect){350, 180, 100, 40}, (SDL_Color){0,0,0,0},
                        loadFromText("Inizia", (SDL_Color){200, 0, 200, 0}, mainRenderer,
                        "Assets/Font/comicz.ttf", 200), &startBriscola);

  btt2 = createElement(1, GUI_LABEL, (SDL_Rect){350, 260, 100, 40}, (SDL_Color){0,0,0,0},
                        loadFromText("Opzioni", (SDL_Color){0, 200, 200, 0}, mainRenderer,
                        "Assets/Font/comicz.ttf", 200), &handleClick);

  btt3 = createElement(1, GUI_LABEL, (SDL_Rect){350, 340, 100, 40}, (SDL_Color){0,0,0,0},
                        loadFromText("Esci", (SDL_Color){200, 200, 0, 0}, mainRenderer,
                        "Assets/Font/comicz.ttf", 200), &quitGame);

  volmut = createElement(0, GUI_IMAGE, (SDL_Rect){780, 430, 20, 20}, (SDL_Color){0,0,0,0},
                        loadTexture("Assets/Sound/music.bmp", mainRenderer), &muteAudio);

  volmut2 = createElement(1, GUI_IMAGE, (SDL_Rect){720, 430, 20, 20}, (SDL_Color){0,0,0,0},
                        loadTexture("Assets/Sound/mute.bmp", mainRenderer), &muteAudio);

  volup = createElement(1, GUI_IMAGE, (SDL_Rect){760, 430, 20, 20}, (SDL_Color){0,0,0,0},
                        loadTexture("Assets/Sound/high.bmp", mainRenderer), &volPlus);

  voldw = createElement(1, GUI_IMAGE, (SDL_Rect){740, 430, 20, 20}, (SDL_Color){0,0,0,0},
                        loadTexture("Assets/Sound/low.bmp", mainRenderer), &volMinus);

  bgMusic = Mix_LoadMUS("Assets/Sound/snd.mp3");
  Mix_PlayMusic(bgMusic, -1);
  Mix_VolumeMusic(50);

  addElement(&globalGUI, &logo);
  addElement(&globalGUI, &btt1);
  addElement(&globalGUI, &btt2);
  addElement(&globalGUI, &btt3);
  addElement(&globalGUI, &volup);
  addElement(&globalGUI, &voldw);
  addElement(&globalGUI, &volmut);
  addElement(&globalGUI, &volmut2);

  return 0;

}

int handleClick(int x, int y, Uint32 ptr){

  //elemGUI *shish = ptr;

  printf("Geesus\n");

  return 0;

}

int quitGame(int x, int y, Uint32 ptr) {mainClose();}

int volPlus(int x, int y, Uint32 ptr) {Mix_VolumeMusic(Mix_VolumeMusic(-1) + 2);}

int volMinus(int x, int y, Uint32 ptr) {Mix_VolumeMusic(Mix_VolumeMusic(-1) - 2);}

int muteAudio(int x, int y, Uint32 ptr) {
  Mix_PausedMusic()?Mix_ResumeMusic():Mix_PauseMusic();
  volmut.active = Mix_PausedMusic();
  volmut2.active = !Mix_PausedMusic();
}

int startBriscola(int x, int y, Uint32 ptr){
	breakLoop = 1;
	initBriscola();
}
