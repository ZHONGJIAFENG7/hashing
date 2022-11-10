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

void Custom_SDL_RenderFillCircle(SDL_Renderer *rend, int cx, int cy, int r)
{

  double dy;
  for (dy = 1; dy <= r; dy += 1.0)
  {
    double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
    SDL_RenderDrawLine(rend, cx - dx, cy + r - dy, cx + dx, cy + r - dy);
    SDL_RenderDrawLine(rend, cx - dx, cy - r + dy, cx + dx, cy - r + dy);
  }
}

/*void Custom_SDL_RenderFillCircle2(SDL_Renderer *renderer, int centerX, int centerY, int radius)
{
  const int diameter = (radius * 2);

  int x = (radius - 1);
  int y = 0;
  int tx = 1;
  int ty = 1;
  int error = (tx - diameter);

  while (x >= y)
  {

    // top
    SDL_RenderDrawLine(renderer, centerX - y, centerY - x, centerX + y, centerY - x);

    // top-center piece
    SDL_RenderDrawLine(renderer, centerX - x, centerY - y, centerX + x, centerY - y);

    // lower-center piece
    SDL_RenderDrawLine(renderer, centerX - x, centerY + y, centerX + x, centerY + y);

    // lower piece
    SDL_RenderDrawLine(renderer, centerX - y, centerY + x, centerX + y, centerY + x);

    if (error <= 0)
    {
      ++y;
      error += ty;
      ty += 2;
    }

    if (error > 0)
    {
      --x;
      tx += 2;
      error += (tx - diameter);
    }
  }
}*/

void Custom_SDL_RenderDrawCircle(SDL_Renderer *rend, int cx, int cy, int r)
{
  double dx, dy;
  dx = floor(sqrt(r));
  SDL_RenderDrawLine(rend, cx - dx, cy + r, cx + dx, cy + r);
  SDL_RenderDrawLine(rend, cx - dx, cy - r, cx + dx, cy - r);
  for (dy = 1; dy <= r; dy += 1.0)
  {
    dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
    SDL_RenderDrawPoint(rend, cx + dx, cy + r - dy);
    SDL_RenderDrawPoint(rend, cx + dx, cy - r + dy);
    SDL_RenderDrawPoint(rend, cx - dx, cy + r - dy);
    SDL_RenderDrawPoint(rend, cx - dx, cy - r + dy);
  }
}
