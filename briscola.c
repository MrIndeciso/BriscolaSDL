#include "Lib/SDL/include/SDL.h"
#include "Lib/SDL/include/SDL_mixer.h"
#include "Lib/SDL/include/SDL_ttf.h"
#include "Lib/SDL/include/SDL_net.h"
#include <stdio.h>
#include <math.h>

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
int mouseHover(int);
int checkForMouseHover(SDL_Event);

int optionsEnabled = 0, FPSEnabled = 0;
int handleOptions();
int enableFPS();


//The resolution that we render with
int SCR_WIDTH = 800;
int SCR_HEIGHT = 450;

//The initial game resolution
int SCR_LWIDTH = 800;
int SCR_LHEIGHT = 450;

SDL_Window* mainWindow = NULL;
SDL_Surface* mainScreenSurface = NULL;
SDL_Renderer* mainRenderer = NULL;
SDL_Texture* bgTexture = NULL;
SDL_Surface* windowIcon = NULL;

FPSCounter fpsDraw;

SDL_Event e;

int breakLoop = 0;

elemGUI logo, btt1, btt1Selected, btt2, btt2Selected, btt3, btt3Selected, volup, voldw, volmut, volmut2, credits, credits2, optionsbg, optitle, opGraphics, opFakeSlider1,
        opMusicVol, opFakeSlider2, opSFXVol, opFakeSlider3, opclose, opicon, opFPStick, opFPScheckbox, opFPSstring, volHitbox, sliderMV1, sliderMV2, sliderMV3;

//Boolean to check if the slider is active
int actSl1 = 0, actSl2 = 0, actSl3 = 0;
int sSSl1();
int sSSl2();
int sSSl3();

//Check for hovering objects
elemGUI *hovObjs[] = {&btt1, &btt2, &btt3, &opFakeSlider1, &opFakeSlider2, &opFakeSlider3};

int qualityRes = 2;

int leftMousePressed = 0;

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

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

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
                    Mix_Init(MIX_INIT_OGG);
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

        if(FPSEnabled == 1){
          drawFPS(&fpsDraw, SDL_GetTicks());
        }

        SDL_RenderPresent(mainRenderer);

        while(SDL_PollEvent(&e)!=0){

            checkForMouseHover(e);

            stdEventInput(&breakLoop, e, mainWindow); //No function overloading but at least we got the Chinese version of "pass by reference"

            gInput(globalGUI, e);

            if(e.type == SDL_MOUSEBUTTONDOWN) leftMousePressed = 1;
            if(e.type == SDL_MOUSEBUTTONUP) leftMousePressed = 0;

        }

    }

    return 0;

}

int mainLoad(){

    SDL_RenderSetLogicalSize(mainRenderer, SCR_LWIDTH, SCR_LHEIGHT);

    //This is probably wrong
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    SDL_SetWindowResizable(mainWindow, SDL_TRUE);

    //SDL_SetRenderDrawColor(mainRenderer, 0x80, 0xFF, 0xFF, 0xFF); //Set background color

    bgTexture = loadTexture("Assets/BG/mainmenuh.bmp", mainRenderer);

    windowIcon = SDL_LoadBMP("Assets/Icon/icon.bmp");
    SDL_SetWindowIcon(mainWindow, windowIcon);
    SDL_FreeSurface(windowIcon);

    loadMMAssets();

    return 0;

}

//Here starts the actual main screen stuff

int loadMMAssets(){ //Called in mainLoad

  logo = createElement(1, GUI_IMAGE, (SDL_Rect){40, 50, 200, 80}, (SDL_Color){0,0,0,0},
                        loadTexture("Assets/MM/Logo.bmp", mainRenderer), &handleClick);

  btt1 = createElement(1, GUI_LABEL, (SDL_Rect){65, 160, 80, 40}, (SDL_Color){0,0,0,0},
                        loadFromText("Inizia", (SDL_Color){255, 255, 255, 0}, mainRenderer,
                        "Assets/Font/comicz.ttf", 100), &doNothing);

  btt2 = createElement(1, GUI_LABEL, (SDL_Rect){65, 240, 100, 40}, (SDL_Color){0,0,0,0},
                        loadFromText("Opzioni", (SDL_Color){255, 255, 255, 0}, mainRenderer,
                        "Assets/Font/comicz.ttf", 100), &doNothing);

  btt3 = createElement(1, GUI_LABEL, (SDL_Rect){65, 320, 60, 40}, (SDL_Color){0,0,0,0},
                        loadFromText("Esci", (SDL_Color){255, 255, 255, 0}, mainRenderer,
                        "Assets/Font/comicz.ttf", 100), &doNothing);

  btt1Selected = createElement(0, GUI_LABEL, (SDL_Rect){65, 160, 80, 40}, (SDL_Color){0,0,0,0},
                        loadFromText("Inizia", (SDL_Color){10, 10, 10, 0}, mainRenderer,
                        "Assets/Font/comicz.ttf", 100), &startBriscola);

  btt2Selected = createElement(0, GUI_LABEL, (SDL_Rect){65, 240, 100, 40}, (SDL_Color){0,0,0,0},
                        loadFromText("Opzioni", (SDL_Color){10, 10, 10, 0}, mainRenderer,
                        "Assets/Font/comicz.ttf", 100), &handleOptions);

  btt3Selected = createElement(0, GUI_LABEL, (SDL_Rect){65, 320, 60, 40}, (SDL_Color){0,0,0,0},
                        loadFromText("Esci", (SDL_Color){10, 10, 10, 0}, mainRenderer,
                        "Assets/Font/comicz.ttf", 100), &quitGame);

  credits = createElement(1, GUI_LABEL, (SDL_Rect){5, 436, 140, 10}, (SDL_Color){0,0,0,0},
                        loadFromText("Programming & memes by Roberto Bertolini", (SDL_Color){10, 10, 10, 0},
                        mainRenderer, "Assets/Font/OpenSans-Regular.ttf", 20), &doNothing);

  credits2 = createElement(1, GUI_LABEL, (SDL_Rect){5, 425, 100, 10}, (SDL_Color){0,0,0,0},
                        loadFromText("Music & design by Matteo Cerri", (SDL_Color){10, 10, 10, 0},
                        mainRenderer, "Assets/Font/OpenSans-Regular.ttf", 20), &doNothing);

  //Options menu elements start here
  optionsbg = createElement(0, GUI_IMAGE, (SDL_Rect){265, 60, 480, 350}, (SDL_Color){0,0,0,0},
                        loadTexture("Assets/MM/Options/optionsbg.bmp", mainRenderer), &doNothing);

  optitle = createElement(0, GUI_LABEL, (SDL_Rect){378, 80, 90, 35}, (SDL_Color){0,0,0,0},
                        loadFromText("Options", (SDL_Color){255, 255, 255, 0}, mainRenderer,
                        "Assets/Font/comicz.ttf", 60), &doNothing);

  opicon = createElement(0, GUI_IMAGE, (SDL_Rect){340, 84, 30, 30}, (SDL_Color){0,0,0,0},
                        loadTexture("Assets/MM/Options/gear.bmp", mainRenderer), &doNothing);

  opclose = createElement(0, GUI_IMAGE, (SDL_Rect){692, 90, 20, 20}, (SDL_Color){0,0,0,0},
                        loadTexture("Assets/MM/Options/quit.bmp", mainRenderer), &handleOptions);

  opGraphics = createElement(0, GUI_LABEL, (SDL_Rect){300, 140, 105, 20}, (SDL_Color){0,0,0,0},
                        loadFromText("Graphics quality", (SDL_Color){255, 255, 255, 0}, mainRenderer,
                        "Assets/Font/comicz.ttf", 60), &doNothing);

  opFakeSlider1 = createElement(0, GUI_LABEL, (SDL_Rect){300, 150, 350, 35}, (SDL_Color){0,0,0,0},
                        loadFromText("-------------------------------WIP-----------------------------",
                        (SDL_Color){255, 255, 255, 0}, mainRenderer, "Assets/Font/comicz.ttf", 60), &sSSl1);

  opMusicVol = createElement(0, GUI_LABEL, (SDL_Rect){300, 205, 90, 20}, (SDL_Color){0,0,0,0},
                        loadFromText("Music volume", (SDL_Color){255, 255, 255, 0}, mainRenderer,
                        "Assets/Font/comicz.ttf", 60), &doNothing);

  opFakeSlider2 = createElement(0, GUI_LABEL, (SDL_Rect){300, 215, 350, 35}, (SDL_Color){0,0,0,0},
                        loadFromText("-------------------------------WIP-----------------------------",
                        (SDL_Color){255, 255, 255, 0}, mainRenderer, "Assets/Font/comicz.ttf", 60), &sSSl2);

  opSFXVol = createElement(0, GUI_LABEL, (SDL_Rect){300, 270, 97, 20}, (SDL_Color){0,0,0,0},
                        loadFromText("Effects volume", (SDL_Color){255, 255, 255, 0}, mainRenderer,
                        "Assets/Font/comicz.ttf", 60), &doNothing);

  opFakeSlider3 = createElement(0, GUI_LABEL, (SDL_Rect){300, 280, 350, 35}, (SDL_Color){0,0,0,0},
                        loadFromText("-------------------------------WIP-----------------------------",
                        (SDL_Color){255, 255, 255, 0}, mainRenderer, "Assets/Font/comicz.ttf", 60), &sSSl3);

  opFPSstring = createElement(0, GUI_LABEL, (SDL_Rect){360, 338, 90, 25}, (SDL_Color){0,0,0,0},
                        loadFromText("Show FPS",
                        (SDL_Color){255, 255, 255, 0}, mainRenderer, "Assets/Font/comicz.ttf", 60), &doNothing);

  opFPScheckbox = createElement(0, GUI_LABEL, (SDL_Rect){300, 322, 50, 50}, (SDL_Color){0,0,0,0},
                        loadFromText("[ ]",
                        (SDL_Color){255, 255, 255, 0}, mainRenderer, "Assets/Font/OpenSans-Regular.ttf", 80), &enableFPS);

  opFPStick = createElement(0, GUI_LABEL, (SDL_Rect){313, 338, 25, 25}, (SDL_Color){0,0,0,0},
                        loadFromText("V",
                        (SDL_Color){255, 255, 255, 0}, mainRenderer, "Assets/Font/OpenSans-Regular.ttf", 60), &doNothing);

  volHitbox = createElement(0, GUI_LABEL, (SDL_Rect){680, 240, 20, 20}, (SDL_Color){0,0,0,0},
                        loadFromText(" ",
                        (SDL_Color){0, 0, 0, 255}, mainRenderer, "Assets/Font/OpenSans-Regular.ttf", 60), &muteAudio);

  volmut = createElement(0, GUI_IMAGE, (SDL_Rect){680, 240, 20, 20}, (SDL_Color){0,0,0,0},
                        loadTexture("Assets/Sound/music.bmp", mainRenderer), &doNothing);

  volmut2 = createElement(0, GUI_IMAGE, (SDL_Rect){680, 240, 20, 20}, (SDL_Color){0,0,0,0},
                        loadTexture("Assets/Sound/mute.bmp", mainRenderer), &doNothing);

  volup = createElement(0, GUI_IMAGE, (SDL_Rect){690, 220, 20, 20}, (SDL_Color){0,0,0,0},
                        loadTexture("Assets/Sound/high.bmp", mainRenderer), &volPlus);

  voldw = createElement(0, GUI_IMAGE, (SDL_Rect){670, 220, 20, 20}, (SDL_Color){0,0,0,0},
                        loadTexture("Assets/Sound/low.bmp", mainRenderer), &volMinus);

  sliderMV1 = createElement(0, GUI_BUTTON, (SDL_Rect){296, 159, 8, 17}, (SDL_Color){255,255,255,0},
                        NULL, &doNothing);

  sliderMV2 = createElement(0, GUI_BUTTON, (SDL_Rect){296, 224, 8, 17}, (SDL_Color){255,255,255,0},
                        NULL, &doNothing);

  sliderMV3 = createElement(0, GUI_BUTTON, (SDL_Rect){296, 289, 8, 17}, (SDL_Color){255,255,255,0},
                        NULL, &doNothing);

  bgMusic = Mix_LoadMUS("Assets/Sound/mainmenu.ogg");
  Mix_PlayMusic(bgMusic, -1);
  Mix_VolumeMusic(0);

  addElement(&globalGUI, &logo);
  addElement(&globalGUI, &btt1);
  addElement(&globalGUI, &btt2);
  addElement(&globalGUI, &btt3);
  addElement(&globalGUI, &btt1Selected);
  addElement(&globalGUI, &btt2Selected);
  addElement(&globalGUI, &btt3Selected);
  addElement(&globalGUI, &credits);
  addElement(&globalGUI, &credits2);

  addElement(&globalGUI, &optionsbg);
  addElement(&globalGUI, &optitle);
  addElement(&globalGUI, &opicon);
  addElement(&globalGUI, &opclose);
  addElement(&globalGUI, &opGraphics);
  addElement(&globalGUI, &opFakeSlider1);
  addElement(&globalGUI, &opMusicVol);
  addElement(&globalGUI, &opFakeSlider2);
  addElement(&globalGUI, &volHitbox);
  addElement(&globalGUI, &volup);
  addElement(&globalGUI, &voldw);
  addElement(&globalGUI, &volmut);
  addElement(&globalGUI, &volmut2);
  addElement(&globalGUI, &opSFXVol);
  addElement(&globalGUI, &opFakeSlider3);
  addElement(&globalGUI, &opFPStick);
  addElement(&globalGUI, &opFPSstring);
  addElement(&globalGUI, &opFPScheckbox);

  addElement(&globalGUI, &sliderMV1);
  addElement(&globalGUI, &sliderMV2);
  addElement(&globalGUI, &sliderMV3);

  return 0;

}

int handleClick(int x, int y, Uint32 ptr){

  //elemGUI *shish = ptr;

  //printf("Geesus\n");

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

  Mix_HaltMusic();
	breakLoop = 1;
	initBriscola();

  int i;
  for(i=0;i<globalGUI.elemCount;i++) SDL_DestroyTexture(globalGUI.element[i]);

  return 0;

}

int mouseHover(int bttn) {
  switch(bttn){
    case 0:
      btt1.active = 0;
      btt1Selected.active = 1;
    break;
    case 1:
      btt2.active = 0;
      btt2Selected.active = 1;
    break;
    case 2:
      btt3.active = 0;
      btt3Selected.active = 1;
    break;
    case 3:
      {
        //if(!actSl1) break;
        if(!leftMousePressed) break;
        int x, y;
        SDL_GetTruePos(&x, &y);
        sliderMV1.pos.x = (x>650)?650:(x<300)?300:x;
        qualityRes = round(2.0f * (float)(sliderMV1.pos.x-300) / 350.0f);
        printf("%d\n", qualityRes);
      }
    break;
    case 4:
      {
        //if(!actSl2) break;
        if(!leftMousePressed) break;
        int x, y;
        SDL_GetTruePos(&x, &y);
        sliderMV2.pos.x = (x>650)?650:(x<300)?300:x;
        Mix_VolumeMusic((int)(128.0f * (float)(sliderMV2.pos.x-300) / 650.0f));
      }
    break;
    case 5:
      {
        //if(!actSl3) break;
        if(!leftMousePressed) break;
        int x, y;
        SDL_GetTruePos(&x, &y);
        sliderMV3.pos.x = (x>650)?650:(x<300)?300:x;
        //TODO: implementare effetti sonori
      }
    break;
  }
}

int handleOptions(){
  switch(optionsEnabled){
    case 0:

      optionsEnabled = 1;
      optionsbg.active = 1;
      optitle.active = 1;
      opGraphics.active = 1;
      opFakeSlider1.active = 1;
      opMusicVol.active = 1;
      opFakeSlider2.active = 1;
      opSFXVol.active = 1;
      opFakeSlider3.active = 1;
      opclose.active = 1;
      opicon.active = 1;
      opFPScheckbox.active = 1;
      opFPSstring.active = 1;
      volup.active = 1;
      voldw.active = 1;
      volHitbox.active = 1;

      /*
      if(FPSEnabled == 0){
        opFPStick.active = 0;
      }
      else if(FPSEnabled == 1){
        opFPStick.active = 1;
      }
      */

      opFPStick.active = FPSEnabled; //Molto complicato così

      if(Mix_PausedMusic() == 0){
        volmut2.active = 1;
        volmut.active = 0;
      }
      else if(Mix_PausedMusic() == 1){
        volmut2.active = 0;
        volmut.active = 1;
      }

      sliderMV1.active = 1;
      sliderMV2.active = 1;
      sliderMV3.active = 1;

    break;
    case 1:
      optionsEnabled = 0;
      optionsbg.active = 0;
      optitle.active = 0;
      opGraphics.active = 0;
      opFakeSlider1.active = 0;
      opMusicVol.active = 0;
      opFakeSlider2.active = 0;
      opSFXVol.active = 0;
      opFakeSlider3.active = 0;
      opclose.active = 0;
      opicon.active = 0;
      opFPScheckbox.active = 0;
      opFPSstring.active = 0;
      opFPStick.active = 0;
      volmut2.active = 0;
      volmut.active = 0;
      volup.active = 0;
      voldw.active = 0;
      volHitbox.active = 0;

      sliderMV1.active = 0;
      sliderMV2.active = 0;
      sliderMV3.active = 0;

    break;
  }
}

int enableFPS(){
  switch(FPSEnabled){
    case 0:
      FPSEnabled = 1;
      opFPStick.active = 1;
    break;
    case 1:
      FPSEnabled = 0;
      opFPStick.active = 0;
    break;
  }
}

int checkForMouseHover(SDL_Event e){

  if(e.type != SDL_MOUSEMOTION) return 0;

  int i;
  for(i=0;i<6;i++){
    if(e.motion.x >= hovObjs[i]->pos.x && e.motion.x <= (hovObjs[i]->pos.x + hovObjs[i]->pos.w) &&
       e.motion.y >= hovObjs[i]->pos.y && e.motion.y <= (hovObjs[i]->pos.y + hovObjs[i]->pos.h)) {
         mouseHover(i);
         break;
      }
  }

  if(!(i-6)) { //Put this in the meme with the brain
    btt1.active = 1;
    btt1Selected.active = 0;
    btt2.active = 1;
    btt2Selected.active = 0;
    btt3.active = 1;
    btt3Selected.active = 0;
  }

  return 0;

}

//Switch State Slider
int sSSl1() { actSl1 = !actSl1; }
int sSSl2() { actSl2 = !actSl2; }
int sSSl3() { actSl3 = !actSl3; }
