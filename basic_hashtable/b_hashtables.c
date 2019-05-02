#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/****
  Basic hash table key/value pair
 ****/
typedef struct Pair {
  char *key;
  char *value;
} Pair;

/****
  Basic hash table
 ****/
typedef struct BasicHashTable {
  int capacity;
  Pair **storage;
} BasicHashTable;

/****
  Create a key/value pair to be stored in the hash table.
 ****/
Pair *create_pair(char *key, char *value)
{
  Pair *pair = malloc(sizeof(Pair));
  pair->key = strdup(key);
  pair->value = strdup(value);

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(Pair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/****
  djb2 hash function

  Do not modify this!
 ****/
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char * u_str = (unsigned char *)str;

  while ((c = *u_str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}


/****
  Fill this in.

  All values in storage should be initialized to NULL
  (hint: look up `calloc`)
 ****/
BasicHashTable *create_hash_table(int capacity)
{
  BasicHashTable *ht = malloc(sizeof(BasicHashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(Pair *)); // calloc is the same as malloc, except it initializes all values to NULL. With malloc, some garbage values could remain in there right after you initialize it.

  return ht;
}

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  // This is our hashed index
  unsigned int index = hash(key, ht->capacity);

  // Create a Pair
  Pair *pair = create_pair(key, value);

  // Check stored pairs
  Pair *stored_pair = ht->storage[index];

  if (stored_pair != NULL) {
    // If stored_pair is not NULL, we know our bucket is not empty
    // Check to see if our new key is the same as the currently stored key.
    // If it is, we'll replace the value.
    // If it isn't, we'll throw a warning first, then replace the key and value.
    if (strcmp(key, stored_pair->key) != 0) { // strcomp() compares 2 strings to see if they are equiv
      printf("WARNING: Overwriting value: %s, %s with %s, %s\n", stored_pair->key, stored_pair->value, key, value);
    }
    destroy_pair(stored_pair);
  }
  ht->storage[index] = pair;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);

  if(ht->storage[index] == NULL) {
    printf("Error: Nothing at index: %d with key: %s\n", index, key);
  } else {
    destroy_pair(ht->storage[index]);
    ht->storage[index] = NULL;
  }
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);

  if (ht->storage[index] == NULL) {
    printf("Unable to retrieve entry with key: %s\n", key);
    return NULL;
  }

  return ht->storage[index]->value;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  for (int i = 0; i <ht->capacity; i++) {
    if (ht->storage[i] != NULL) {
      destroy_pair(ht->storage[i]);
    }
  }
  free(ht->storage);
  free(ht);
}


#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);

  hash_table_insert(ht, "Daren", "Larson\n");
  hash_table_insert(ht, "Joanne", "Larson\n");

  printf("%s", hash_table_retrieve(ht, "Daren"));
  printf("%s", hash_table_retrieve(ht, "Joanne"));
  // printf("%s", hash_table_retrieve(ht, "Greg"));

  hash_table_remove(ht, "Joanne");

  if (hash_table_retrieve(ht, "Joanne") == NULL) {
    printf("...gone tomorrow. (success)\n");
  } else {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }

  destroy_hash_table(ht);

  return 0;
}
#endif
