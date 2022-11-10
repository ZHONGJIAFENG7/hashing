#include "SDL2/SDL.h"
#include <math.h>

#define WWIDTH 800
#define WHEIGHT 600

struct SDL_Simplewin
{
  SDL_bool finished;
  SDL_Window *win;
  SDL_Renderer *renderer;
};
typedef struct SDL_Simplewin SDL_Simplewin;

void Custom_SDL_Init(SDL_Simplewin *sw);
void Custom_SDL_Events(SDL_Simplewin *sw);
void Custom_SDL_SetDrawColour(SDL_Simplewin *sw, Uint8 r, Uint8 g, Uint8 b);
void Custom_SDL_RenderFillCircle(SDL_Renderer *rend, int cx, int cy, int r);
void Custom_SDL_RenderDrawCircle(SDL_Renderer *rend, int cx, int cy, int r);
