#ifndef GUIUTIL_H_INCLUDED
#define GUIUTIL_H_INCLUDED

#include "../Lib/SDL/include/SDL.h"

enum elementTypes {
    GUI_BUTTON,
    GUI_LABEL,
    GUI_TEXTBOX,
    GUI_MESSAGE,
    GUI_IMAGE,
    GUICOUNT
};

//This is almost Java
typedef struct elemGUI elemGUI;
struct elemGUI {
    int active;
    int type;
    SDL_Rect pos;
    SDL_Texture *texture;
    int* args;
};

//IBaseEnterpriseVisitorOuputPrintableContextGeneratorSingletonMiddlewareStrategyConfiguratorFactoryReturner.class
typedef struct textureElemGUI textureElemGUI;
struct textureElemGUI {
    elemGUI elemGUI;
    SDL_Texture* texture;
};

int drawElement(elemGUI element, SDL_Renderer *mainRenderer);
int drawTextureElement(textureElemGUI textureElemGUI);

elemGUI createElement(int active, int type, SDL_Rect pos, int* args);



#endif // GUIUTIL_H_INCLUDED
