#include "RenderUtil.h"
#include "../Lib/SDL/include/SDL_ttf.h"

#include <sys/stat.h>
#include <stdio.h>

extern int SCR_WIDTH;
extern int SCR_HEIGHT;

char errorString[ERRCOUNT][63] = {"Error: could not load SDL\n\0", "Error: could not create Window\n\0", "Error: could not load image\n\0", "Error: could not convert image\n\0", "Error: file doesn't exist\n\0", "Error: could not create a renderer\n\0", "Error: could not create a texture\n\0"};

SDL_Surface* loadSurface(char* source, SDL_PixelFormat* format){

    SDL_Surface* temp = NULL,* image = NULL;

    if(!fileExists(source)){
        printf("%s", errorString[ERR_FILE_DOESNT_EXIST]);
        return -1;
    }

    temp = SDL_LoadBMP(source);

    if(temp == NULL) {
        printf("%s", errorString[ERR_CANT_LOAD_IMAGE]);
        return -1;
    }

    //Not really needed but it's just a layer of compatibility for strange BMPs...
    image = SDL_ConvertSurface(temp, format, 0);

    if(image == NULL){
        printf("%s", errorString[ERR_CANT_CONVERT_IMAGE]);
        return -1;
    }

    //Yeah boy, downloadmoreram.com
    SDL_FreeSurface(temp);

    //Returns a pointer to the image as SDL likes
    return image;
}

SDL_Texture* loadTexture(char* source, SDL_Renderer* renderer){
    SDL_Surface* temp = NULL;
    SDL_Texture* loadedTexture = NULL;

    if(!fileExists(source)){
        printf("%s", errorString[ERR_FILE_DOESNT_EXIST]);
        return -1;
    }

    temp = SDL_LoadBMP(source);

    if(temp == NULL) {
        printf("%s", errorString[ERR_CANT_LOAD_IMAGE]);
        return -1;
    }

    loadedTexture = SDL_CreateTextureFromSurface(renderer, temp);

    if(loadedTexture == NULL){
        printf("%s", errorString[ERR_CANT_LOAD_TEXTURE]);
        return -1;
    }

    SDL_FreeSurface(temp);

    //Returns a pointer to the image as SDL likes
    return loadedTexture;
}

SDL_Texture* loadFromText(char* text, SDL_Color textColor, SDL_Renderer* renderer, char* fontName, int fontSize){

  SDL_Surface* temp = NULL;
  SDL_Texture* texture = NULL;

  TTF_Font* font;
  font = TTF_OpenFont(fontName, fontSize);

  temp = TTF_RenderText_Solid(font, text, textColor);

  if(temp == NULL) return -1;

  texture = SDL_CreateTextureFromSurface(renderer, temp);

  if(texture == NULL) return -1;

  SDL_FreeSurface(temp);

  return texture;

}

int fileExists(char* source){
    //This ISN'T the exact copy of some code I found on StackOverflow...
    struct stat buffer;
    return (stat(source, &buffer)==0);
}

int stdEventInput(int* condition, SDL_Event e, SDL_Window* window){
    if(e.type == SDL_QUIT) *condition = 1;
    if(e.type == SDL_KEYDOWN) if(e.key.keysym.sym == SDLK_ESCAPE) *condition = 1; //Beautiful

    if(e.type == SDL_WINDOWEVENT) if(e.window.event ==SDL_WINDOWEVENT_RESIZED) SDL_GetWindowSize(window, &SCR_WIDTH, &SCR_HEIGHT);

    return 0;
}

int drawPolygon(){

    return 0;
}
