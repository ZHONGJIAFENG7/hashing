#ifndef SHASH_H
#define SHASH_H
#define MAX_STRING_LEN 1000

typedef struct _node_t
{
  char *value;
  struct _node_t *next;
} node_t;
typedef struct __hash_handle_t
{
  int size;
  int length;
  node_t *array;
} dhash_t;

dhash_t *hash_create(int size);
int hash_insert(dhash_t *ptr, char *str);
int hash_search(dhash_t *ptr, char *str, int *counts);
int hash_free(dhash_t *ptr);

#endif
