#ifndef EXTHASH_H
#define EXTHASH_H

#include "shash.h"
typedef struct __exthash_handle_t
{
  int size;
  int length;
  node_t *array;
} exthash_t;

exthash_t *exthash_create(int size);
int exthash_insert(exthash_t *ptr, char *str);
int exthash_search(exthash_t *ptr, char *str, int *counts);
int exthash_free(exthash_t *ptr);

#endif
