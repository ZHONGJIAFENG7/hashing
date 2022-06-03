#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dhash.h"
#define BEGIN_SIZE 31
#define MAXSIZE 10000
#define NUM_ARGUMENT 3
void get_average_times(dhash_t *ptr, char *file_name);
void check_open(FILE *fp, char *file);
dhash_t *read_char(char *file_name);
void remove_char(char *str, char ch);

int main(int argc, char *argv[])
{
  dhash_t *ptr;
  if (argc < NUM_ARGUMENT)
  {
    fprintf(stderr, "Too few arguments.\n");
    exit(EXIT_FAILURE);
  }
  ptr = read_char(argv[1]);
  get_average_times(ptr, argv[2]);
  return 0;
}

void check_open(FILE *fp, char *file_name)
{
  if (!fp)
  {
    fprintf(stderr, "Cant't open file '%s'\n", file_name);
    exit(EXIT_FAILURE);
  }
}

dhash_t *read_char(char *file_name)
{
  FILE *fp = NULL;
  char buff[MAXSIZE] = "";
  dhash_t *ptr;

  fp = fopen(file_name, "r");
  check_open(fp, file_name);
  ptr = hash_create(BEGIN_SIZE);
  while (fgets(buff, MAXSIZE, fp) != NULL)
  {
    buff[strlen(buff) - 1] = '\0';
    hash_insert(ptr, buff);
  }
  fclose(fp);
  return ptr;
}

void get_average_times(dhash_t *ptr, char *file_name)
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
    if (hash_search(ptr, buff, &n) != -1)
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

  printf("There are %d words and it cost %d times\n", c, total);
  printf("The average checking time is %f\n", (double)total / c);
}
