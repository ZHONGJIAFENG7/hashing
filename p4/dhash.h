#ifndef DHASH_H
#define DHASH_H
#define MAX_STRING_LEN 1000
typedef struct __dhash_handle_t {
	int size;
	int length;
	char **array;
} dhash_t;

dhash_t *dhash_create(int size);
int dhash_insert(dhash_t * ptr, char *str);
int dhash_search(dhash_t * ptr, char *str, int *counts);
int dhash_free(dhash_t *ptr);

#endif
