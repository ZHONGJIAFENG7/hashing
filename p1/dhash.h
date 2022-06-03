#ifndef DHASH_H
#define DHASH_H
#define MAX_STRING_LEN 1000
typedef struct __hash_handle_t
{
  int size;
  int length;
  char **array;
} dhash_t;

dhash_t *hash_create(int size);
int hash_insert(dhash_t *ptr, char *str);
int hash_search(dhash_t *ptr, char *str, int *counts);
int hash_free(dhash_t *ptr);

#endif
