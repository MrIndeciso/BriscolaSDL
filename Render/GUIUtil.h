#ifndef GUIUTIL_H_INCLUDED
#define GUIUTIL_H_INCLUDED

#include "../Lib/SDL/include/SDL.h"

//If you are asking yourself if I'm crazy for trying to make a GUI library myself while there are many online on "The Internet" I can give you a short answer, I am

enum elementTypes {
    GUI_BUTTON,
    GUI_LABEL,
    GUI_TEXTBOX,
    GUI_MESSAGE,
    GUICOUNT
};

//I don't know why but I remember that it's done this way
typedef struct elemPos elemPos;
struct elemPos {
    int x;
    int y;
    int w;
    int h;
};

//This is almost Java
typedef struct elemGUI elemGUI;
struct elemGUI {
    int active;
    int type;
    elemPos pos;
    int* args;
};

//IBaseEnterpriseVisitorOuputPrintableContextGeneratorSingletonMiddlewareStrategyConfiguratorFactoryReturner.class
typedef struct textureElemGUI textureElemGUI;
struct textureElemGUI {
    elemGUI elemGUI;
    SDL_Texture* texture;
};

int drawElement(elemGUI element);
int drawTextureElement(textureElemGUI textureElemGUI);

elemGUI createElement(int active, int type, elemPos pos, int* args);



#endif // GUIUTIL_H_INCLUDED
