#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Hash table key/value pair with linked list pointer.

  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the the 
  next `LinkedPair` in the list of `LinkedPair` nodes. 
 */
typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable {
  int capacity;
  LinkedPair **storage;
} HashTable;

/*
  Create a key/value linked pair to be stored in the hash table.
 */
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/*
  Use this function to safely destroy a hashtable pair.
 */
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/*
  djb2 hash function

  Do not modify this!
 */
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

/*
  Fill this in.

  All values in storage should be initialized to NULL
 */
HashTable *create_hash_table(int capacity)
{
  HashTable *ht =malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair *));

  return ht;
}

/*
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 */
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  // This is our hashed index
  unsigned int index = hash(key, ht->capacity);

  // Create a LinkedPair
  LinkedPair *new_pair = create_pair(key, value);

  // Check stored linked_pairs
  LinkedPair *stored_pair = ht->storage[index];

  if (stored_pair == NULL) {
    // There is nothing here, so store the new pair here
    ht->storage[index] = new_pair;

  } else {
    // if there is something saved at the index we hashed to...
    while (stored_pair) {
      // check if stored_pair key is the same as key we are inserting. Replace the value if it is
      if (strcmp(stored_pair->key, new_pair->key) == 0) {
        stored_pair->value = new_pair->value;

        // Update the linked_pair->next with the next value of the current linked_pair before we
        // overwite it and lose access to the information
        // new_pair->next = ht->storage[index]->next;

        // // Clean up the old pair we are replacing
        // destroy_pair(stored_pair);

        // // Replace the old linked_pair with the new linked_park
        // ht->storage[index] = new_pair;

        // New key/value linked pair has been inserted and we are done, so return
        return;
      } else if (stored_pair->next == NULL) {
        // The key we are inserting isn't the same as any in the linked list,
        // and we know we're at the end of the linked list if next == NULL,
        // so we will insert the new linked_pair at the end of the linked list here
        stored_pair->next = new_pair;

        // Do I need to malloc any memory here to keep the data in the heap??????

        // New key/value linked pair has been inserted and we are done, so return
        return;

      } else {
        // there is something stored here, but has a different key, and the next isn't null
        // so we'll go to the next pair in the linked list
        stored_pair = stored_pair->next;
      }
    }
  }
}


/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);

  // Check stored linked_pairs
  LinkedPair *stored_linked_pair = ht->storage[index];

  if (stored_linked_pair == NULL) {
    // There is nothing stored at this index, so the key we are searching for doesn't exist.
    printf("Error: Nothing at index: %d with key: %s\n", index, key);

  } else {
    LinkedPair *prev_pair = NULL;
    // We found something at the index we hashed to,
    // so now we search through the linked list for a key that matches what we're searching for
    while (stored_linked_pair != NULL) {
      if ((strcmp(key, stored_linked_pair->key) == 0) && strcmp(stored_linked_pair->key, ht->storage[index]->key) == 0) {
        // The key we are searching for is found and it's in the actual array (ie, first item in the linked list)
        // store the next pair in the linked list in a variable
        LinkedPair *new_head = stored_linked_pair->next;

        // destroy the pair we were searching for and found
        destroy_pair(stored_linked_pair);

        // save the next value as the new head of the linked list in the array
        ht->storage[index] = new_head;

        return;

      } else if (strcmp(key, stored_linked_pair->key) == 0) {
        // The key we are searching for is found, but it's burried in the linked list (ie not the head of the linked list)
        // Update the prev pair to point to what the pair we are destorying was pointing to
        prev_pair->next = stored_linked_pair->next;

        // Destroy this linked pair
        destroy_pair(stored_linked_pair);

        return;
      }

      // save the previous pair
      prev_pair = stored_linked_pair;

      // go to next pair
      stored_linked_pair = stored_linked_pair->next;
    }
  }

  // They key was not found in the array or linked list
  printf("Error: Nothing at index: %d with key: %s\n", index, key);
  return;
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);

  if (ht->storage[index] == NULL) {
    // Nothing was found at the index we hashed to
    printf("Unable to retrieve entry with key: %s\n", key);
    return NULL;
  }

  // Check stored linked_pairs, save in variable
  LinkedPair *stored_pair = ht->storage[index];

  while (stored_pair) {
    if (strcmp(key, stored_pair->key) == 0) {
      // the key matches what we're searching for
      return stored_pair->value;
    }
    // key not found on this iteration. move to next pair
    stored_pair = stored_pair->next;
  }

  // key not found in linked list
  printf("Unable to retrieve entry with key: %s\n", key);
  return NULL;
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{
  // loops through each spot on the array
  for (int i = 0; i < ht->capacity; i++) {
    LinkedPair *stored_linked_pair = ht->storage[i];

    // dives through linked lists at each index of the array
    while (stored_linked_pair != NULL) {
      // save a pointer to the next pair so we can delete it on the next loop
      LinkedPair *next_pair = stored_linked_pair->next;

      // destory the current pair
      destroy_pair(stored_linked_pair);

      // update stored_linked_pair to next so that we can delete that one
      stored_linked_pair = next_pair;
    }
  }
  free(ht->storage);
  free(ht);
}

/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
// HashTable *hash_table_resize(HashTable *ht)
// {
//   HashTable *new_ht = malloc(sizeof(HashTable));
//   new_ht->capacity = ht->capacity * 2;
//   new_ht->storage = calloc(new_ht->capacity, sizeof(LinkedPair *));

//   for (int i = 0; i < ht->capacity; i++) {
    
//   }

//   return new_ht;
// }


#ifndef TESTING
int main(void)
{
  // struct HashTable *ht = create_hash_table(2);

  // hash_table_insert(ht, "line_1", "Tiny hash table\n");
  // hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  // hash_table_insert(ht, "line_3", "Linked list saves the day!\n");
  
  struct HashTable *ht = create_hash_table(8);

  hash_table_insert(ht, "key-0", "val-0");
  hash_table_insert(ht, "key-1", "val-1");
  hash_table_insert(ht, "key-2", "val-2");
  hash_table_insert(ht, "key-3", "val-3");
  hash_table_insert(ht, "key-4", "val-4");
  hash_table_insert(ht, "key-5", "val-5");
  hash_table_insert(ht, "key-6", "val-6");
  hash_table_insert(ht, "key-7", "val-7");
  hash_table_insert(ht, "key-8", "val-8");
  hash_table_insert(ht, "key-9", "val-9");

  hash_table_insert(ht, "key-0", "new-val-0");
  hash_table_insert(ht, "key-1", "new-val-1");
  hash_table_insert(ht, "key-2", "new-val-2");
  hash_table_insert(ht, "key-3", "new-val-3");
  hash_table_insert(ht, "key-4", "new-val-4");
  hash_table_insert(ht, "key-5", "new-val-5");
  hash_table_insert(ht, "key-6", "new-val-6");
  hash_table_insert(ht, "key-7", "new-val-7");
  hash_table_insert(ht, "key-8", "new-val-8");
  hash_table_insert(ht, "key-9", "new-val-9");
  
  printf("\n\n");

  printf("key: %s, vale: %s\n", ht->storage[0]->key, ht->storage[0]->value);
  printf("key: %s, vale: %s\n", ht->storage[1]->key, ht->storage[1]->value);
  printf("key: %s, vale: %s\n", ht->storage[2]->key, ht->storage[2]->value);
  printf("key: %s, vale: %s\n", ht->storage[3]->key, ht->storage[3]->value);
  printf("key: %s, vale: %s\n", ht->storage[4]->key, ht->storage[4]->value);
  printf("key: %s, vale: %s\n", ht->storage[5]->key, ht->storage[5]->value);
  printf("key: %s, vale: %s\n", ht->storage[6]->key, ht->storage[6]->value);
  printf("key: %s, vale: %s\n", ht->storage[7]->key, ht->storage[7]->value);
  // printf("key: %s, vale: %s\n", ht->storage[8]->key, ht->storage[8]->value);
  // printf("key: %s, vale: %s\n", ht->storage[9]->key, ht->storage[9]->value);
  // printf("key: %s, vale: %s\n", ht->storage[1]->next->key, ht->storage[1]->next->value);

  // printf("%s", hash_table_retrieve(ht, "line_1"));
  // printf("%s", hash_table_retrieve(ht, "line_2"));
  // printf("%s", hash_table_retrieve(ht, "line_3"));
  
  printf("\n\n");

  // int old_capacity = ht->capacity;
  // ht = hash_table_resize(ht);
  // int new_capacity = ht->capacity;

  // printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
