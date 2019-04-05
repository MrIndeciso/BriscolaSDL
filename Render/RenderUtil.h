#ifndef RENDERUTIL_H_INCLUDED
#define RENDERUTIL_H_INCLUDED

#include "../Lib/SDL/include/SDL.h"

//Yeah this way is better
//typedef enum errorCodes;
//No actually the compiler says that it's useless

enum errorCodes {
    ERR_CANT_LOAD_SDL,
    ERR_CANT_CREATE_WINDOW,
    ERR_CANT_LOAD_IMAGE,
    ERR_CANT_CONVERT_IMAGE,
    ERR_FILE_DOESNT_EXIST,
    ERR_INVALID_RENDERER,
    ERR_CANT_LOAD_TEXTURE,
    ERRCOUNT
};

//Prototype
extern char errorString[ERRCOUNT][63];

//Returns the pointer to a loaded image as a surface
SDL_Surface* loadSurface(char* source, SDL_PixelFormat* format);

//Returns the pointer to a loaded image as a texture
SDL_Texture* loadTexture(char* source, SDL_Renderer* renderer);

//Returns the pointer to a string converted to texture using a custom font
SDL_Texture* loadFromText(char* text, SDL_Color textColor, SDL_Renderer* renderer, char* fontName, int fontSize);

//Returns the pixel size of a certain string. Type = 0 equals w, type = 1 equals h
int getTextSize(char* text, char* fontName, int fontSize, int type);

//Self-documenting
int fileExists(char* source);

//Handles standard input like ESCAPE or pressing the X button
int stdEventInput(int* condition, SDL_Event e, SDL_Window* window);

int drawPolygon();

#endif // RENDERUTIL_H_INCLUDED
