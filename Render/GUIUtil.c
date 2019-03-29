#include "GUIUtil.h"

int drawElement(elemGUI element, SDL_Renderer *mainRenderer){

  if(!element.active) return 0;

  switch(element.type) {

    case GUI_IMAGE:

      SDL_RenderCopy(mainRenderer, element.texture, NULL, &element.pos);

      break;

    default:
      return 0;
      break;

  }

  return 0;
}

int createElement(int active, int type, SDL_Rect pos, int* args){

}
