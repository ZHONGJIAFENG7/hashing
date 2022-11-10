#ifndef SHASH_H
#define SHASH_H

#define MAX_STRING_LEN 1000

typedef struct _node_t {
	char *value;
	struct _node_t * next;
} node_t;

typedef struct __shash_handle_t {
	int size;
	int length;
	node_t *array;
} shash_t;

shash_t *shash_create(int size);
int shash_insert(shash_t * ptr, char *str);
int shash_search(shash_t * ptr, char *str, int *counts);
int shash_free(shash_t *ptr);

#endif
