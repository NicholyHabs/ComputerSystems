/// NGH CMS 330
/// Project 5 Multithreaded Hashtable  
// 4/11/2016


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define HASHSIZE 1024
typedef struct _hashnode_t {
  char *key;
  char *value;
  struct _hashnode_t *next;
} hashnode_t;

typedef struct _hash_t {
  hashnode_t **buckets;
  int size;
  
  // Add something here to create an array of locks
  // There will be one lock per bucket
  pthread_mutex_t *mutexes;
  
} hashtable_t;

/*** Hash a string to an integer ***/
// Input
//    char *s: the string to hash
// Output
//    The integer hash code as an unsigned long int type
unsigned long int hash(char *s) {
  // You may choose your own hash function implementation
  // Anything that is reasonable is accetpable

  // Note that you SHOULD NOT use a complex cryptographic hash
  // function. Something that adds up the char values of the key
  // string, possibly with a little multiplication mixed in would be
  // fine. A quick search will turn up several suggestions.

 
  unsigned long int hashvalue = 0;
  int i; 
  for(i=0; i< strlen(s); i++){
  hashvalue = hashvalue * 31 + s[i]; 
  }
  return hashvalue; 
}

/*** Create a new hashtable_t ***/
// Input
//    tableSize: the number of buckets in the new table
// Output
//    A pointer to the new table
hashtable_t * hashtableInit(int tableSize) {
  if(tableSize<1){
    return NULL;
  }
  hashtable_t *table = malloc(sizeof(hashtable_t));
  table->buckets = malloc(sizeof(hashnode_t *) * tableSize);
  
  int i;
  for(i=0; i<tableSize; i++){
    table->buckets[i] = NULL;
  }
  table->size = tableSize;
  
  table->mutexes = malloc(table->size * sizeof(pthread_mutex_t));

  for (i = 0; i < table->size; i++) {
    //table->mutexes[i] = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    int c = pthread_mutex_init(&table->mutexes[i], NULL);
    if(c != 0) {
      perror("mutex init\n");
      exit(1);
    }
  }
  
  return table;
}

/*** Insert a key-value pair into a table ***/
// Inputs
//    hashtable *h: the hashtable performing the insertion
//    char *key: the key string
//    char *value: the value string
// Output
//    Nothing
void hashtableInsert(hashtable_t *h, char *key, char *value) {

  // Hash the input and determine the bucket that will hold the new pair
  int hashCode = hash(key);
  int b = hashCode % h->size;
  hashnode_t *current;
  // Create a new hashnode_t
  hashnode_t *hnode = malloc(sizeof(hashnode_t));
  
  pthread_mutex_lock(&h->mutexes[b]);
  hnode->key = strdup(key);
  hnode->value = strdup(value);
  hnode->next = NULL;
  if(h->buckets[b] == NULL){
    h->buckets[b] = hnode;
  }
  
  else{
  current = h->buckets[b];
  
  while(current->next != NULL){
    current = current->next;
  }
    current->next = hnode;
  }
  pthread_mutex_unlock(&h->mutexes[b]);
 
  // Insert into the bucket's list
  // Acquire the bucket's lock before inserting
  // Release the bucket's lock after the insertion is done

}

/*** Lookup the value associated with a particular key ***/
// Inputs
//    hashtable_t *h: the table
//    char *key: the key to find
// Output
//    The char *value associated with the key or NULL if no match exists
char * hashtableLookup(hashtable_t *h, char *key) {
       hashnode_t *hLook;
       char *k;
       int hashCode = hash(key);
       int b = hashCode % h->size;
       hLook = h->buckets[b]; 
      
       pthread_mutex_lock(&h->mutexes[b]);
         if(strcmp(hLook->key, key) == 0){
           //printf("found key: %s value: %s \n", hLook->key, hLook->value);
           k = hLook->key; 
           pthread_mutex_unlock(&h->mutexes[b]);
           return k;
         }
         else {
        pthread_mutex_unlock(&h->mutexes[b]); 
         return NULL;
         }

}

/*** Remove a key-value pair from the table if it exists ***/
// Inputs
//    hashtable_t *h: the table
//    char *key: the key to find
// Output
//    The char *value associated with the key or NULL if no match exists
char * hashtableRemove(hashtable_t *h, char *key) {

    // As with insertion, use the bucket's lock to protect any changes
    // to the bucket's contents
    int hashCode = hash(key);
     int b = hashCode % h->size;
      char *v;
      hashnode_t *hRemove;
      hashnode_t *previous;
      pthread_mutex_lock(&h->mutexes[b]);
      hRemove = h->buckets[b];
      if(h->buckets[b] == NULL){
        pthread_mutex_unlock(&h->mutexes[b]);
        return NULL;
      }
    
          if(strcmp(hRemove->key, key) == 0){
          v = hRemove->value;
          h->buckets[b] = hRemove->next;
          pthread_mutex_unlock(&h->mutexes[b]);
          return v;
         }
       
         else {
         while ((hRemove != NULL) && (strcmp(hRemove->key, key) != 0)){
             
             previous = hRemove;
             hRemove = hRemove->next;
         }
             if(hRemove == NULL){
               pthread_mutex_unlock(&h->mutexes[b]);
               return NULL;
             }
             else {
               v = hRemove->value;
               previous->next = hRemove->next;
               free(hRemove);
              pthread_mutex_unlock(&h->mutexes[b]);
               return v;
             }
             
          }
        }
      
  

/*** Print a hashtable ***/
// Input
//    hashtable_t *h: the table
void hashtablePrint(hashtable_t *h) {
  int i;
  for (i = 0; i < h->size; i++) {
    hashnode_t *node = h->buckets[i];
    printf("Contents of bucket %d:\n", i);

    while (node != NULL) {
      printf("\t<%s, %s>\n", node->key, node->value);
      node = node->next;
    }
  }
}

void * test(void *arg){
  hashtable_t *table = (hashtable_t *) arg;
  
  
  hashtableInsert(table, "T1", "table");
  hashtableInsert(table, "T2", "table");
  hashtableInsert(table, "T3", "table");
  hashtableInsert(table, "T4", "table");
  hashtableInsert(table, "T5", "table");
 
  hashtableLookup(table, "T4");
  
  hashtableRemove(table, "T1");
  hashtableRemove(table, "T2");
  hashtableRemove(table, "T3");
  hashtableRemove(table, "T4");
  hashtableRemove(table, "T5");
  
  
  return NULL;
}

/*** Main ***/
// You must write your own tests for your hashtable implementation.
//
// Good tests will ensure that all of the basic operations work correctly
// for a range of key values when both a single thread and multiple
// threads are inserting, retrieving, and removing from the table
//
// Think hard about the results of your tests! How will you know that
// your code is really working?
int main() {
 
  hashtable_t *table = hashtableInit(10);
  
  /*
  //hashtablePrint(table);
  hashtableInsert(table, "the", "for");
  hashtableInsert(table, "done", "table");
  hashtablePrint(table);
  hashtableLookup(table, "the");
  hashtableRemove(table, "the");
  //hashtablePrint(table);
  hashtableRemove(table, "done");
  hashtablePrint(table);
  */
  
  /*** Put the rest of your tests here ***/
 
  int i;
  int threads = 25;
   pthread_t threadArray[threads];
  for(i =0; i<=threads; i++){
    pthread_create(&threadArray[i], NULL, test, table ); 
  }
  
  for(i =0; i<=threads; i++){
    pthread_join(threadArray[i], NULL );
   
  }
  
  hashtablePrint(table);
  
  return 0;
}
