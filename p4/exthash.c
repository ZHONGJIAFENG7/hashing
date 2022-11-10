#include "exthash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PRIME 31
#define RANGE 0.6
static void check_size(exthash_t *ptr);
static unsigned int hash_openssl(char *str);
static int getPrime(int num);
exthash_t *exthash_create(int size);
int exthash_insert(exthash_t *ptr, char *str);
int exthash_search(exthash_t *ptr, char *str, int *count);
int exthash_free(exthash_t *ptr);

static unsigned int hash_openssl(char *str)
{
  int i, l;
  unsigned long ret = 0;
  unsigned short *s;
  if (str == NULL)
  {
    return 0;
  }
  l = (strlen(str) + 1) / 2;
  s = (unsigned short *)str;
  for (i = 0; i < l; i++)
  {
    ret ^= (s[i] << (i & 0x0f));
  }
  return ret;
}

static int getPrime(int num)
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

/* when the size reaches around 60% and expand the size similar to previous program    because I think if the size of array do not change and then plenty word      will be inserted into linked list and will influence the look-up times*/
static void check_size(exthash_t *ptr)
{

  node_t *tmp = NULL;

  if ((double)ptr->length / (double)ptr->size > RANGE)
  {
    int newsize = getPrime(ptr->size * 2);

    exthash_t *nptr = exthash_create(newsize);

    int i = 0;

    while (i < ptr->size)
    {
      node_t *head = &(ptr->array[i]);
      while (head)
      {
        if (head->value != NULL)
        {
          exthash_insert(nptr, head->value);
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

exthash_t *exthash_create(int size)
{
  int i = 0;
  exthash_t *ptr = (exthash_t *)malloc(sizeof(exthash_t) * 1);

  if (ptr == NULL)
  {
    fprintf(stderr, "Error! Malloc.\n");
    exit(EXIT_FAILURE);
  }

  memset(ptr, 0, sizeof(exthash_t));

  ptr->size = size;

  ptr->array = (node_t *)malloc(sizeof(node_t) * size);

  for (i = 0; i < size; i++)
  {
    ptr->array[i].next = NULL;
    ptr->array[i].value = NULL;
  }
  return ptr;
}

int exthash_insert(exthash_t *ptr, char *str)
{
  unsigned int hashkey1 = 0;
  node_t *nptr = NULL;
  char *newstr = NULL;

  check_size(ptr);
  hashkey1 = hash_openssl(str) % ptr->size;
  nptr = &(ptr->array[hashkey1]);
  /*when the linked list has some element,then go to the next area until
    find position and then the use next pointer point to it*/
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

int exthash_search(exthash_t *ptr, char *str, int *count)
{
  unsigned int hashkey1 = hash_openssl(str) % ptr->size;

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
int exthash_free(exthash_t *ptr)
{
  int i = 0;

  while (i < ptr->size)
  {
    node_t *head = &(ptr->array[i]);
    if (head->value != NULL)
    {
      free(head->value);
    }
    head = head->next;
    while (head)
    {
      if (head->value != NULL)
      {
        node_t *tmp = head->next;
        free(head->value);
        free(head);
        head = tmp;
      }
    }
    i++;
  }
  free(ptr->array);
  free(ptr);

  return 0;
}
