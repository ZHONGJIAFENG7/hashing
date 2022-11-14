#include "custom_sdl2.h"

void Custom_SDL_Init(SDL_Simplewin *sw)
{

  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
    SDL_Quit();
    exit(1);
  }

  sw->finished = 0;

  sw->win = SDL_CreateWindow("SDL Window",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             WWIDTH, WHEIGHT,
                             SDL_WINDOW_SHOWN);
  if (sw->win == NULL)
  {
    fprintf(stderr, "\nUnable to initialize SDL Window:  %s\n", SDL_GetError());
    SDL_Quit();
    exit(1);
  }

  sw->renderer = SDL_CreateRenderer(sw->win, -1, 0);
  if (sw->renderer == NULL)
  {
    fprintf(stderr, "\nUnable to initialize SDL Renderer:  %s\n", SDL_GetError());
    SDL_Quit();
    exit(1);
  }

  /* Set screen to black */
  Custom_SDL_SetDrawColour(sw, 0, 0, 0);
  SDL_RenderClear(sw->renderer);
  SDL_RenderPresent(sw->renderer);
}

void Custom_SDL_Events(SDL_Simplewin *sw)
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
    case SDL_MOUSEBUTTONDOWN:
    case SDL_KEYDOWN:
      sw->finished = 1;
    }
  }
}

void Custom_SDL_SetDrawColour(SDL_Simplewin *sw, Uint8 r, Uint8 g, Uint8 b)
{

  SDL_SetRenderDrawColor(sw->renderer, r, g, b, SDL_ALPHA_OPAQUE);
}

void Custom_SDL_DrawString(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], char *str, int ox, int oy)
{

  int i = 0;
  unsigned char chr;
  do
  {
    chr = str[i++];
    Custom_SDL_DrawChar(sw, fontdata, chr, ox + i * FNTWIDTH, oy);
  } while (str[i]);
}

void Custom_SDL_DrawChar(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], unsigned char chr, int ox, int oy)
{
  unsigned x, y;
  printf("%c", chr);

  for (y = 0; y < FNTHEIGHT; y++)
  {
    for (x = 0; x < FNTWIDTH; x++)
    {
      if (fontdata[chr - FNT1STCHAR][y] >> (FNTWIDTH - 1 - x) & 1)
      {
        printf("*");
        /* White Ink */
        Custom_SDL_SetDrawColour(sw, 255, 255, 255);
        SDL_RenderDrawPoint(sw->renderer, x + ox, y + oy);
      }
      else
      {
        printf(" ");
        /* Black Ink */
        Custom_SDL_SetDrawColour(sw, 0, 0, 0);
        SDL_RenderDrawPoint(sw->renderer, x + ox, y + oy);
      }
    }
    printf("\n");
  }
}
