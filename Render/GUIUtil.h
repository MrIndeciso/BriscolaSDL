#ifndef GUIUTIL_H_INCLUDED
#define GUIUTIL_H_INCLUDED

#include "../Lib/SDL/include/SDL.h"

//Used to switch between types
enum elementTypes {
    GUI_BUTTON,
    GUI_LABEL,
    GUI_TEXTBOX,
    GUI_MESSAGE,
    GUI_IMAGE,
    GUI_POLY,
    GUICOUNT
};

//Useless for now
enum polyTypes {
  POLY_RECT,
  POLYCOUNT
};

//This is almost Java
//Should cover every possible usage
typedef struct elemGUI {
    int active;
    int type;
    SDL_Rect pos;
    SDL_Color color;
    SDL_Texture *texture;
    int (*onClick)(int, int, Uint32);
} elemGUI;

typedef struct gGUI {
  elemGUI *element[255];
  int elemCount;
} gGUI;

//Just draws an element
int drawElement(elemGUI element, SDL_Renderer* mainRenderer);

//Handles any SDL event for an element
int GUIEventInput(elemGUI element, SDL_Event e);

//Creates a new element
elemGUI createElement(int active, int type, SDL_Rect pos, SDL_Color color, SDL_Texture* texture, int (*onClick)(int, int, Uint32));

//Adds an element to the global GUI structure
int addElement(gGUI* global, elemGUI* element);

//Renders the whole GUI structure
int drawGUI(gGUI global, SDL_Renderer* mainRenderer);

//Handles every input for the element
int gInput(gGUI global, SDL_Event e);

#endif // GUIUTIL_H_INCLUDED
