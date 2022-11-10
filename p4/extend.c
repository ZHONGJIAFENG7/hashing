#include "custom_sdl2.h"
#include "dhash.h"
#include "exthash.h"
#include "shash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MILLISECONDDELAY 100
#define BEGIN_SIZE 31
#define MAXSIZE 100
#define NUM_ARGUMENT 3
#define PRINT_X 2
#define PRINT_Y_STEP 24
#define SIZE 1024
#define FONT_PATH "asset/ArialUnicode.ttf"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TEXT_POS_X 200
#define TEXT_POS_Y 20
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
void check_open(FILE *fp, char *file);
void operate(char *file1, char *file2, SDL_Simplewin sw);
void shash_get_average_times(shash_t *ptr, char *file_name, int *c, int *total);
shash_t *shash_read_char(char *file_name);
void dhash_get_average_times(dhash_t *ptr, char *file_name, int *c, int *total);
dhash_t *dhash_read_char(char *file_name);
void exthash_get_average_times(exthash_t *ptr, char *file_name, int *c, int *total);
exthash_t *exthash_read_char(char *file_name);
void Print_Result(char *str, int c, int total);
void animation(SDL_Simplewin *sw, char *buffer, TTF_Font *font);
static char buffer[SIZE];

int main(int argc, char *argv[])
{
  SDL_Simplewin sw;
  char *file1 = NULL, *file2 = NULL;

  Custom_SDL_Init(&sw);

  /*Initialize SDL2_ttf*/
  TTF_Init();

  if (argc < NUM_ARGUMENT)
  {
    fprintf(stderr, "Too few arguments.\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    file1 = argv[1];
    file2 = argv[2];
  }

  operate(file1, file2, sw);

  /* Clear up graphics subsystems */
  atexit(TTF_Quit);
  atexit(SDL_Quit);

  /*Destroy window*/
  SDL_DestroyWindow(sw.win);

  return 0;
}

void operate(char *file1, char *file2, SDL_Simplewin sw)
{
  int c, total;
  shash_t *shash_ptr;
  dhash_t *dhash_ptr;
  exthash_t *exthash_ptr;
  TTF_Font *font = TTF_OpenFont(FONT_PATH, 20);

  shash_ptr = shash_read_char(file1);
  shash_get_average_times(shash_ptr, file2, &c, &total);
  Print_Result("shash", c, total);

  dhash_ptr = dhash_read_char(file1);
  dhash_get_average_times(dhash_ptr, file2, &c, &total);
  Print_Result("dhash", c, total);

  exthash_ptr = exthash_read_char(file1);
  exthash_get_average_times(exthash_ptr, file2, &c, &total);
  Print_Result("exthash", c, total);

  animation(&sw, buffer, font);
}

void SDL_printf(char *fmt, ...)
{
  char buf[SIZE];
  va_list argptr;
  va_start(argptr, fmt);
  vsprintf(buf, fmt, argptr);

  strcat(buffer, "\n");
  strcat(buffer, buf);
  strcat(buffer, "\n");

  va_end(argptr);
}

void Print_Result(char *str, int c, int total)
{
  SDL_printf(str);
  SDL_printf("There are %d words and it cost %d times", c, total);
  SDL_printf("The average checking time is %.2f", (double)total / c);
}

void animation(SDL_Simplewin *sw, char *buffer, TTF_Font *font)
{
  SDL_Rect textRect;
  SDL_Surface *textSurface;
  SDL_Texture *text;
  SDL_Color textColor = {0x00, 0x00, 0x00, 0xFF};
  /*SDL_Color textBackgroundColor = {0xFF, 0xFF, 0xFF, 0xFF};*/

  textSurface = TTF_RenderUTF8_Blended_Wrapped(font, buffer, textColor, SCREEN_WIDTH);

  textRect.w = textSurface->w;
  textRect.h = textSurface->h;
  textRect.x = TEXT_POS_X;
  textRect.y = TEXT_POS_Y;

  text = SDL_CreateTextureFromSurface(sw->renderer, textSurface);
  SDL_FreeSurface(textSurface);

  while (!sw->finished)
  {
    /* Sleep for a short time */
    SDL_Delay(MILLISECONDDELAY);

    Custom_SDL_Events(sw);

    /*Initialize renderer color white for the background*/
    SDL_SetRenderDrawColor(sw->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    /*Clear screen*/
    SDL_RenderClear(sw->renderer);

    /*Draw text*/
    SDL_RenderCopy(sw->renderer, text, NULL, &textRect);

    /*Update screen*/
    SDL_RenderPresent(sw->renderer);
  }
  printf("=======end=======\n");
}

void check_open(FILE *fp, char *file_name)
{
  if (!fp)
  {
    fprintf(stderr, "Cant't open file '%s'\n", file_name);
    exit(EXIT_FAILURE);
  }
}

/*read character from file that named by argv[1] and return pointer*/
dhash_t *dhash_read_char(char *file_name)
{
  FILE *fp = NULL;
  char buff[MAXSIZE] = "";
  dhash_t *ptr;

  fp = fopen(file_name, "r");
  check_open(fp, file_name);
  ptr = dhash_create(BEGIN_SIZE);
  while (fgets(buff, MAXSIZE, fp) != NULL)
  {
    buff[strlen(buff) - 1] = '\0';
    dhash_insert(ptr, buff);
  }
  fclose(fp);
  return ptr;
}

void dhash_get_average_times(dhash_t *ptr, char *file_name, int *out_c, int *out_total)
{
  FILE *fp = NULL;
  int total = 0, n = 0, c = 0;
  char buff[MAXSIZE] = "";
  fp = fopen(file_name, "r");
  check_open(fp, file_name);
  while ((fgets(buff, MAXSIZE, fp)) != NULL)
  {
    buff[strlen(buff) - 1] = '\0';
    if (dhash_search(ptr, buff, &n) != -1)
    {
      total += n;
      c++;
    }
    else
    {
      c++;
    }
  }
  fclose(fp);

  *out_c = c;
  *out_total = total;
}

/*read character from file that named by argv[1] and return pointer*/
shash_t *shash_read_char(char *file_name)
{
  FILE *fp = NULL;
  char buff[MAXSIZE] = "";
  shash_t *ptr;

  fp = fopen(file_name, "r");
  check_open(fp, file_name);
  ptr = shash_create(BEGIN_SIZE);
  while (fgets(buff, MAXSIZE, fp) != NULL)
  {
    buff[strlen(buff) - 1] = '\0';
    shash_insert(ptr, buff);
  }
  fclose(fp);
  return ptr;
}

void shash_get_average_times(shash_t *ptr, char *file_name, int *out_c, int *out_total)
{
  FILE *fp = NULL;
  int total = 0, n = 0, c = 0;
  char buff[MAXSIZE] = "";
  fp = fopen(file_name, "r");
  check_open(fp, file_name);
  while ((fgets(buff, MAXSIZE, fp)) != NULL)
  {
    buff[strlen(buff) - 1] = '\0';
    if (shash_search(ptr, buff, &n) != -1)
    {
      total += n;
      c++;
    }
    else
    {
      c++;
    }
  }
  fclose(fp);

  *out_c = c;
  *out_total = total;
}

/*read character from file that named by argv[1] and return pointer*/
exthash_t *exthash_read_char(char *file_name)
{
  FILE *fp = NULL;
  char buff[MAXSIZE] = "";
  exthash_t *ptr;

  fp = fopen(file_name, "r");
  check_open(fp, file_name);
  ptr = exthash_create(BEGIN_SIZE);
  while (fgets(buff, MAXSIZE, fp) != NULL)
  {
    buff[strlen(buff) - 1] = '\0';
    exthash_insert(ptr, buff);
  }
  fclose(fp);
  return ptr;
}

void exthash_get_average_times(exthash_t *ptr, char *file_name, int *out_c, int *out_total)
{
  FILE *fp = NULL;
  int total = 0, n = 0, c = 0;
  char buff[MAXSIZE] = "";
  fp = fopen(file_name, "r");
  check_open(fp, file_name);
  while ((fgets(buff, MAXSIZE, fp)) != NULL)
  {
    buff[strlen(buff) - 1] = '\0';
    if (exthash_search(ptr, buff, &n) != -1)
    {
      total += n;
      c++;
    }
    else
    {
      c++;
    }
  }
  fclose(fp);

  *out_c = c;
  *out_total = total;
}
