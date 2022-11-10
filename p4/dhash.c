#include "dhash.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define PRIME 31
#define RANGE 0.6
static void check_size(dhash_t *ptr);
static unsigned int hash_ends(char *str);
static unsigned int hash_all(char *str);
void check_size(dhash_t *ptr);
static int getPrime(int num);

/*Every character in the world is used*/
static unsigned int hash_ends(char *str) {
   int i;
   int n = strlen(str);
   unsigned int hash = 0;
   
   for(i=0;i<n;i++){
      hash = str[i] + PRIME*hash;
   }
   return hash;
}

/*The 1st2 and last2 character*/
static unsigned int hash_all(char *str) {
   int c1, c2, cn1, cn2;
   int hash;
   int n;
   n = strlen(str);
   c1 = str[0];
   c2 = str[1];
   cn1 = str[n-1];
   cn2 = str[n-2];
   hash = (c1*c2 + cn1*cn2)*n;

   return hash;
}

static int getPrime(int num) {
   int i = num;
   int flag = 1;
   while(flag !=0){
      int j = 0;
      flag = 0;
      i++;
      for(j = 2; j <= i/2 ; j++) {
         if(i % j == 0) {
            flag = 1;
            j = i;
         }
      }
   }
   return i;
}

/*when the size reaches around 60% and then put old array in a big new array*/
int dhash_insert_t(dhash_t * ptr, char *str) {
   unsigned int hashkey1 = hash_ends(str) % ptr->size;
   unsigned int hashkey2 = hash_all(str);


   while(ptr->array[hashkey1] != NULL) {
      hashkey1 = (hashkey1 + hashkey2) % ptr->size;
   }

   ptr->array[hashkey1] = str;

   ptr->length++;

   return 0;
}

static void check_size(dhash_t *ptr) {
   if((double)ptr->length/(double)ptr->size> RANGE) {
      int newsize = getPrime(ptr->size * 2);

      dhash_t *nptr = dhash_create(newsize);

      int i = 0;

      while(i < ptr->size) {
         if(ptr->array[i] != NULL) {
            dhash_insert_t(nptr, ptr->array[i]);
         }
         i++;
      }

      free(ptr->array);
      ptr->size = newsize;
      ptr->array = nptr->array;
      free(nptr);
   }

}

dhash_t *dhash_create(int size) {
   int i = 0;
   dhash_t * ptr = (dhash_t *) malloc(sizeof(dhash_t) * 1) ;

   if(ptr == NULL) {
      fprintf(stderr,"Error! Malloc.\n");
      exit(EXIT_FAILURE);
   }
   memset(ptr, 0, sizeof(dhash_t));

   ptr->size = size;
   /*the size of array is prime*/
   ptr->array = (char ** )malloc(sizeof(char *) * size);

   for(i = 0; i < size ; i++ ) {
      ptr->array[i] = NULL;
   }
   return ptr;
}

/*insert words if there is empty in the position,and first position 
  decided by hashkey1,if there is not empty and then use hashkey2, 
  and loop them until the size get around 60%*/
int dhash_insert(dhash_t * ptr, char *str) {

   unsigned int hashkey1 ;
   unsigned int hashkey2 ;

   hashkey1 = hash_ends(str) % ptr->size;
   hashkey2 = hash_all(str);

   check_size(ptr);

   while(ptr->array[hashkey1] != NULL) {
      hashkey1 = (hashkey1 + hashkey2) % ptr->size;
   }

   ptr->array[hashkey1] = (char *)malloc(sizeof(char) * MAX_STRING_LEN );
   strcpy(ptr->array[hashkey1], str);

   ptr->length++;

   return 0;
}

/*if we can get the num firstly by using one hash,and the count then plus 1
  else we use hashkey2 to array and compare again and also count++,and get 
  the step about checking words*/
int dhash_search(dhash_t * ptr, char *str, int *count) {
   unsigned int hashkey1 = hash_ends(str) % ptr->size;
   unsigned int hashkey2 = hash_all(str);
   *count = 0;

   while(ptr->array[hashkey1 ] != NULL) {
      *count += 1;
      if(strcmp(ptr->array[hashkey1], str) == 0){
         return hashkey1;
      }
      hashkey1 = (hashkey1 + hashkey2) % ptr->size;
   }
   return -1;
}


int dhash_free(dhash_t *ptr) {
   int i = 0;

   for(i = 0; i < ptr->size; i++) {
      if(ptr->array[i] != NULL)
         free(ptr->array[i]);
   }
   free(ptr->array);
   free(ptr);

   return 0;
}
