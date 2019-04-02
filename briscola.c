#include "Lib/SDL/include/SDL.h"
#include <stdio.h>

#include "Modes/Briscola.h"
#include "Render/RenderUtil.h"
#include "Render/GUIUtil.h"
#include "Render/GameUtil.h"

int mainInit();
int mainLoad();
int mainLoop();
int mainClose();

int loadBackground();
int loadMainScrBtts();

int loadMMAssets();
int handleClick(int, int, int);

int SCR_WIDTH = 800;
int SCR_HEIGHT = 450;

const int SCR_LWIDTH = 800;
const int SCR_LHEIGHT = 450;

SDL_Window* mainWindow = NULL;
SDL_Surface* mainScreenSurface = NULL;
SDL_Renderer* mainRenderer = NULL;

SDL_Event e;

int breakLoop = 0;

elemGUI logo, btt1, btt2, btt3;
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

    globalGUI.elemCount = 0;

    return 0;

}

int mainClose(){

    TTF_Quit();

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

        drawGUI(globalGUI, mainRenderer);

        SDL_RenderPresent(mainRenderer);

        while(SDL_PollEvent(&e)!=0){

            stdEventInput(&breakLoop, e, mainWindow); //No function overloading but at least we got the Chinese version of "pass by reference"

            gInput(globalGUI, e);

            GUIEventInput(btt1, e);

        }

    }

    return 0;

}

int mainLoad(){

    SDL_RenderSetLogicalSize(mainRenderer, SCR_LWIDTH, SCR_LHEIGHT);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    SDL_SetWindowResizable(mainWindow, SDL_TRUE);

    SDL_SetRenderDrawColor(mainRenderer, 0x80, 0xFF, 0x80, 0xFF); //Set background color

    loadMMAssets();

    return 0;

}

//Here starts the actual main screen stuff

int loadMMAssets(){ //Called in mainLoad

  logo = createElement(1, GUI_IMAGE, (SDL_Rect){300, 50, 200, 79}, (SDL_Color){0,0,0,0},
                        loadTexture("Assets/MM/Logo.bmp", mainRenderer), NULL);

  btt1 = createElement(1, GUI_LABEL, (SDL_Rect){350, 180, 100, 40}, (SDL_Color){0,0,0,0},
                        loadFromText("Inizia", (SDL_Color){200, 0, 200, 0}, mainRenderer,
                        "Assets/Font/comicz.ttf", 200), &handleClick);

  btt2 = createElement(1, GUI_LABEL, (SDL_Rect){350, 260, 100, 40}, (SDL_Color){0,0,0,0},
                        loadFromText("Opzioni", (SDL_Color){200, 0, 200, 0}, mainRenderer,
                        "Assets/Font/comicz.ttf", 200), &handleClick);

  btt3 = createElement(1, GUI_LABEL, (SDL_Rect){350, 340, 100, 40}, (SDL_Color){0,0,0,0},
                        loadFromText("Esci", (SDL_Color){200, 0, 200, 0}, mainRenderer,
                        "Assets/Font/comicz.ttf", 200), &handleClick);

  bgMusic = Mix_LoadMUS("Assets/Sound/snd.mp3");
  //Mix_PlayMusic(bgMusic, -1);
  Mix_VolumeMusic(16);

  addElement(&globalGUI, logo);
  addElement(&globalGUI, btt1);
  addElement(&globalGUI, btt2);
  addElement(&globalGUI, btt3);

  return 0;

}

int handleClick(int x, int y, int ptr){

  printf("Iesus %d\n%d\n", x, y);

  return 0;

}
