#include "Lib/SDL/include/SDL.h"
#include <stdio.h>

//#include "SDL2_framerate.h"

#include "Modes/Briscola.h"
#include "Render/RenderUtil.h"

int mainInit();
int mainLoad();
int mainLoop();
int mainClose();

int loadBackground();
int loadMainScrBtts();

const int SCR_WIDTH = 640;
const int SCR_HEIGHT = 480;

SDL_Window* mainWindow = NULL;
SDL_Surface* mainScreenSurface = NULL;
SDL_Renderer* mainRenderer = NULL;

//FPSmanager* FPSLimiter;

SDL_Event e;

int breakLoop = 0;

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
    if(SDL_Init(SDL_INIT_VIDEO)<0) {
        printf("%s", errorString[ERR_CANT_LOAD_SDL]);
        return -1;
    } else {
        mainWindow = SDL_CreateWindow("DEBUG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_SHOWN);
        if(mainWindow == NULL) {
            printf("%s", errorString[ERR_CANT_CREATE_WINDOW]);
            return -1;
        } else {
            mainScreenSurface = SDL_GetWindowSurface(mainWindow);
            mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
            if(mainRenderer == NULL) {
                printf("%s", errorString[ERR_INVALID_RENDERER]);
                return -1;
            } else {
                //SDL_initFramerate(FPSLimiter);
                //SDL_setFramerate(FPSLimiter, 60);
            }
        }
    }

    return 0;
}

int mainClose(){
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

        SDL_RenderPresent(mainRenderer);

        while(SDL_PollEvent(&e)!=0){

            stdEventInput(&breakLoop, e); //No function overloading but at least we got the Chinese version of "pass by reference"

            if(e.type == SDL_MOUSEBUTTONDOWN){
                int x, y;
                SDL_GetMouseState(&x, &y);
            }
        }

        //SDL_framerateDelay(FPSLimiter);
    }

    return 0;
}

int mainLoad(){
    SDL_SetRenderDrawColor(mainRenderer, 0x80, 0xFF, 0x80, 0xFF); //Set background color

    return 0;
}
