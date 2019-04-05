#include "GUIUtil.h"
#include "../Lib/SDL/include/SDL_ttf.h"

extern int SCR_WIDTH;
extern int SCR_HEIGHT;
extern int SCR_LWIDTH;
extern int SCR_LHEIGHT;

int drawElement(elemGUI element, SDL_Renderer *mainRenderer){

  if(!element.active) return 0;

  switch(element.type) {

    case GUI_BUTTON:
      {
      SDL_Color temp;
      SDL_GetRenderDrawColor(mainRenderer, &temp.r, &temp.g, &temp.b, &temp.a);
      SDL_SetRenderDrawColor(mainRenderer, element.color.r, element.color.g, element.color.b, element.color.a);
      SDL_RenderFillRect(mainRenderer, &element.pos);
      SDL_SetRenderDrawColor(mainRenderer, temp.r, temp.g, temp.b, temp.a);
      break;
      } //Apparently if you want to declare a variable in a case you need to define the scope with {} because otherwise the compiler acts like it is retarded

    case GUI_LABEL:
      SDL_RenderCopy(mainRenderer, element.texture, NULL, &element.pos);
      break;

    case GUI_TEXTBOX:
      break;

    case GUI_MESSAGE:
      break;

    case GUI_IMAGE:
      SDL_RenderCopy(mainRenderer, element.texture, NULL, &element.pos);
      break;

    case GUI_POLY:
      break;

    default:
      return 0;
      break;

  }

  return 0;

}

elemGUI createElement(int active, int type, SDL_Rect pos, SDL_Color color, SDL_Texture *texture, int (*onClick)(int, int, int)){

  elemGUI buffer;

  buffer.active = active;
  buffer.type = type;
  buffer.pos = pos;
  buffer.color = color;
  buffer.texture = texture;
  buffer.onClick = onClick;

  return buffer;

}

int GUIEventInput(elemGUI element, SDL_Event e){

  if(e.type == SDL_MOUSEBUTTONDOWN){
      int x, y;
      SDL_GetMouseState(&x, &y);

      x = (int)((float)x * (float)SCR_LWIDTH / (float)SCR_WIDTH);
      y = (int)((float)y * (float)SCR_LHEIGHT / (float)SCR_HEIGHT);


      if(x<element.pos.x && x>=(element.pos.x-element.pos.w) && y<element.pos.y && y>=(element.pos.y-element.pos.h))
        (*element.onClick)(x, y, &element);

  }

  return 0;

}

int addElement(gGUI* global, elemGUI element){

  global->element[global->elemCount] = element;
  global->elemCount++;

  return 0;

}

int drawGUI(gGUI global, SDL_Renderer* mainRenderer){

  int i;

  for(i=0;i<global.elemCount;i++){
    drawElement(global.element[i], mainRenderer);
  }

  return 0;

}

int gInput(gGUI global, SDL_Event e){

  int i;

  for(i=0;i<global.elemCount;i++){
    GUIEventInput(global.element[i], e);
  }

  return 0;

}
