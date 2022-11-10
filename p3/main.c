#include "custom_sdl2.h"
#include <stdio.h>
#include <stdlib.h>

#define MILLISECONDDELAY 100

int main()
{
  SDL_Simplewin sw;
  SDL_Rect r;
  SDL_Rect r1;

  Custom_SDL_Init(&sw);

  r.x = 100;
  r.y = 100;
  r.w = 50;
  r.h = 50;

  r1.x = 170;
  r1.y = 100;
  r1.w = 50;
  r1.h = 50;

  while (!sw.finished)
  {
    SDL_Delay(MILLISECONDDELAY);
    Custom_SDL_SetDrawColour(&sw, 255, 255, 255);
    SDL_RenderClear(sw.renderer);

    Custom_SDL_SetDrawColour(&sw, 50, 100, 255);

    /*draw line*/
    SDL_RenderDrawLine(sw.renderer, 70, 50, 70, 150);

    /*draw rectangle*/
    SDL_RenderDrawRect(sw.renderer, &r);
    SDL_RenderFillRect(sw.renderer, &r1);

    /*draw circle*/
    Custom_SDL_RenderDrawCircle(sw.renderer, 300, 100, 50);
    Custom_SDL_RenderFillCircle(sw.renderer, 420, 100, 50);

    SDL_RenderPresent(sw.renderer);

    Custom_SDL_Events(&sw);
  }

  atexit(SDL_Quit);

  return 0;
}
