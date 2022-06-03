#include "shash.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define PRIME 31
#define RANGE 0.6
void check_size(dhash_t *ptr);
static unsigned int hash_ends(char *str);
int getPrime(int num);
dhash_t *hash_create(int size);
int hash_insert(dhash_t *ptr, char *str);
int hash_search(dhash_t *ptr, char *str, int *count);
int hash_free(dhash_t *ptr);

static unsigned int hash_ends(char *str)
{
  int i;
  int n = strlen(str);
  unsigned int hash = 0;

  for (i = 0; i < n; i++)
  {
    hash = str[i] + PRIME * hash;
  }

  return hash;
}

int getPrime(int num)
{
  int i = num;
  int flag = 1;
  while (flag != 0)
  {
    int j = 0;
    flag = 0;
    i++;
    for (j = 2; j <= i / 2; j++)
    {
      if (i % j == 0)
      {
        flag = 1;
        j = i;
      }
    }
  }
  return i;
}

void check_size(dhash_t *ptr)
{
  node_t *tmp = NULL;

  if ((double)ptr->length / (double)ptr->size > RANGE)
  {
    int newsize = getPrime(ptr->size * 2);

    dhash_t *nptr = hash_create(newsize);

    int i = 0;

    while (i < ptr->size)
    {
      node_t *head = &(ptr->array[i]);
      while (head)
      {
        if (head->value != NULL)
        {
          hash_insert(nptr, head->value);
        }
        head = head->next;
      }
      i++;
    }

    tmp = ptr->array;
    ptr->size = newsize;
    ptr->array = nptr->array;
    nptr->array = tmp;
    free(nptr);
  }
}

dhash_t *hash_create(int size)
{
  int i = 0;
  dhash_t *ptr = (dhash_t *)malloc(sizeof(dhash_t) * 1);
  if (ptr == NULL)
  {
    fprintf(stderr, "Error! Malloc.\n");
    exit(EXIT_FAILURE);
  }
  memset(ptr, 0, sizeof(dhash_t));

  ptr->size = size;

  ptr->array = (node_t *)malloc(sizeof(node_t) * size);

  for (i = 0; i < size; i++)
  {
    ptr->array[i].next = NULL;
    ptr->array[i].value = NULL;
  }

  return ptr;
}

int hash_insert(dhash_t *ptr, char *str)
{
  unsigned int hashkey1;
  node_t *nptr = NULL;
  char *newstr = NULL;

  check_size(ptr);

  hashkey1 = hash_ends(str) % ptr->size;
  nptr = &(ptr->array[hashkey1]);

  while (nptr->next != NULL)
  {
    nptr = nptr->next;
  }

  newstr = malloc(strlen(str) + 1);
  strcpy(newstr, str);

  if (nptr->value == NULL)
  {
    nptr->value = newstr;
  }
  else
  {
    node_t *newnode = malloc(1 * sizeof(node_t));
    newnode->value = newstr;
    newnode->next = nptr->next;
    nptr->next = newnode;
  }

  ptr->length++;

  return 0;
}

int hash_search(dhash_t *ptr, char *str, int *count)
{
  unsigned int hashkey1 = hash_ends(str) % ptr->size;

  node_t *nptr = &(ptr->array[hashkey1]);

  *count = 0;

  while (nptr)
  {
    if (nptr->value == NULL)
    {
      return -1;
    }
    else
    {
      *count += 1;
      if (strcmp(nptr->value, str) == 0)
      {
        return hashkey1;
      }
      nptr = nptr->next;
    }
  }

  return -1;
}
