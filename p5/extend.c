#include "custom_sdl2.h"
#include "dhash.h"
#include "exthash.h"
#include "font.h"
#include "shash.h"
#include <stdio.h>
#include <stdlib.h>

#define RECTSIZE 20
#define MILLISECONDDELAY 10
#define BEGIN_SIZE 31
#define MAXSIZE 100
#define NUM_ARGUMENT 3
#define PRINT_X 2
#define PRINT_Y_STEP 24
#define SIZE 1024
void check_open(FILE *fp, char *file);
void remove_char(char *str, char ch);
void operate(char *file1, char *file2, SDL_Simplewin sw);
void shash_get_average_times(shash_t *ptr, char *file_name, int *c, int *total);
shash_t *shash_read_char(char *file_name);
void dhash_get_average_times(dhash_t *ptr, char *file_name, int *c, int *total);
dhash_t *dhash_read_char(char *file_name);
void exthash_get_average_times(exthash_t *ptr, char *file_name, int *c, int *total);
exthash_t *exthash_read_char(char *file_name);
void Print_Result(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], char *str, int c, int total);

int main(int argc, char *argv[])
{
  SDL_Simplewin sw;
  char *file1 = NULL, *file2 = NULL;

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

  Custom_SDL_Init(&sw);
  SDL_RenderClear(sw.renderer);
  operate(file1, file2, sw);
  SDL_RenderPresent(sw.renderer);

  while (!sw.finished)
  {
    Custom_SDL_Events(&sw);
  }

  atexit(SDL_Quit);

  return 0;
}

void operate(char *file1, char *file2, SDL_Simplewin sw)
{
  int c, total;
  shash_t *shash_ptr;
  dhash_t *dhash_ptr;
  exthash_t *exthash_ptr;

  shash_ptr = shash_read_char(file1);
  shash_get_average_times(shash_ptr, file2, &c, &total);
  Print_Result(&sw, fontdata, "shash", c, total);

  dhash_ptr = dhash_read_char(file1);
  dhash_get_average_times(dhash_ptr, file2, &c, &total);
  Print_Result(&sw, fontdata, "dhash", c, total);

  exthash_ptr = exthash_read_char(file1);
  exthash_get_average_times(exthash_ptr, file2, &c, &total);
  Print_Result(&sw, fontdata, "exthash", c, total);
}

void SDL_printf(SDL_Simplewin *pWindow, fntrow fontdata[FNTCHARS][FNTHEIGHT], char *fmt, ...)
{
  static int y = 0;

  char buffer[SIZE];
  va_list argptr;
  va_start(argptr, fmt);
  vsprintf(buffer, fmt, argptr);
  va_end(argptr);

  Custom_SDL_DrawString(pWindow, fontdata, buffer, PRINT_X, y);
  y += PRINT_Y_STEP;

  printf("%s\n", buffer);
}

void Print_Result(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], char *str, int c, int total)
{
  SDL_printf(sw, fontdata, str);
  SDL_printf(sw, fontdata, "There are %d words and it cost %d times", c, total);
  SDL_printf(sw, fontdata, "The average checking time is %d", total / c);
}

void check_open(FILE *fp, char *file_name)
{
  if (!fp)
  {
    fprintf(stderr, "Cant't open file '%s'\n", file_name);
    exit(EXIT_FAILURE);
  }
}

void remove_char(char *str, char ch)
{
  unsigned int i, j;
  for (i = 0; i < strlen(str) && str[i] != '\0'; i++)
  {
    if (str[i] == ch)
    {
      for (j = i; j < strlen(str) - 1; j++)
      {
        str[j] = str[j + 1];
      }
    }
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
  /*total is the number of checking times, and c is the number of words,
    n is the step that I get from double hash*/
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
  /*total is the number of checking times, and c is the number of words,
    n is the step that I get from double hash*/
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
  /*total is the number of checking times, and c is the number of words,
    n is the step that I get from double hash*/
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
