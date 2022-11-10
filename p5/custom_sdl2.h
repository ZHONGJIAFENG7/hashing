#include "SDL2/SDL.h"
#include <math.h>

#define WWIDTH 800
#define WHEIGHT 600

/* Font stuff */
typedef unsigned char fntrow;
#define FNTWIDTH (sizeof(fntrow) * 8)
#define FNTHEIGHT 16
#define FNTCHARS 96
#define FNT1STCHAR 32
#define SDL_8BITCOLOUR 256

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
void Custom_SDL_DrawChar(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], unsigned char chr, int ox, int oy);
void Custom_SDL_DrawString(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], char *str, int ox, int oy);
