#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_CONNECTION_QUEUE_LEN 1024
#define PORT 10237
#define MAXBUFFER 1024
#define CMAX (10)
#define BUF 16

int *buffer;

int use  = 0;
int fill = 0;
// Declare three semaphores
sem_t empty; // consumers to signal something is taken out of array
sem_t full; // server to signal something is put in to array
sem_t mutex; // locking and unlocking
int pshared = 0;

void do_fill(int value) { // read into shared buffer
  buffer[fill] = value;
  fill++;
  if (fill == BUF)
	fill = 0;
}


int do_get() { // write into shared buffer
    int tmp = buffer[use];
    use++;
    if (use == BUF)
	use = 0;
    return tmp;
}

/* Hashtable structure definitions */
typedef struct _hashnode_t {
  char *key;
  char *value;
  struct _hashnode_t *next;
} hashnode_t;

typedef struct _hash_t {
  hashnode_t **buckets;
  int size;
  pthread_mutex_t *mutexes;
} hashtable_t;


/*
* Read a null-terminated string from a socket
*
* Inputs:
*     int fd: the socket representing the connection to the client
*     char *buffer: the buffer that will hold the string
*
*/
void readString(int fd, char *buffer) {
    int i = 0;
    while (1) {
      read(fd, &buffer[i], sizeof(char));

      if (buffer[i] == '\0')
        break;
      else
        i++;
    }
}

void writeString(char *s, int socket_fd) {
  int rc;
  if(s == NULL){
    char z = '\0';
    rc = write(socket_fd, &z, sizeof(char));
  } 
  else {
    rc = write(socket_fd, s, strlen(s) + 1); // +1 for the terminating NULL byte
  }
  if (rc < 0) {
    perror("writeString");
    exit(1);
  }
}

/*
* Hash a string to an integer
*
* Input
*    char *s: the string to hash
* Output
*    The integer hash code
*/
unsigned long int hash(char *s) {
  unsigned long int h = 0;

  int i;
  for (i = 0; i < strlen(s); i++) {
    h += (unsigned long int) s[i];
  }

  return h;
}


/*
* Create a new hashtable_t
*
* Input
*     tableSize: the number of buckets in the new table
* Output
*    A pointer to the new table
*/
hashtable_t *hashtableInit(int tableSize) {

//hashtable_t *table = NULL;
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
  /*
  table->mutexes = malloc(table->size * sizeof(pthread_mutex_t));

  for (i = 0; i < table->size; i++) {
    //table->mutexes[i] = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    int c = pthread_mutex_init(&table->mutexes[i], NULL);
    if(c != 0) {
      perror("mutex init\n");
      exit(1);
    }
  } */
  
  return table;
}


/*
* Insert a key-value pair into a table
*
* Inputs
*    hashtable *h: the hashtable performing the insertion
*    int connection_fd: the socket connection to the client
* Output
*    Nothing; a response indicating success (0) or failure (-1) is sent
*    back to the client
*/
void hashtableInsert(hashtable_t *h, int connection_fd) {
 
  
 
  char *key = malloc(sizeof(char) * MAXBUFFER);
  readString(connection_fd, key);
  char *value = malloc(sizeof(char) *MAXBUFFER);
  readString(connection_fd, value);

  int hashCode = hash(key);
  int succeed = -1; 
  int b = hashCode % h->size;
  hashnode_t *current;
  // Create a new hashnode_t
  hashnode_t *hnode = malloc(sizeof(hashnode_t));
  
 // pthread_mutex_lock(&h->mutexes[b]);
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
  
  succeed = 0; 
 // pthread_mutex_unlock(&h->mutexes[b]);
  if(succeed == 0){
    write(connection_fd, &succeed, sizeof(int));
  }
  else{
  write(connection_fd, &succeed, sizeof(int));
  }
  //close(connection_fd);
  
}


/*
* Lookup the value for a given key and send it back to the client
*
* Inputs
*    hashtable *h: the hashtable performing the insertion
*    int connection_fd: the socket representing the connection to the client
* Output
*    Nothing; the value string is sent to the client over the socket
     returns a NULL string if the key is not in the table
*/

void hashtableLookup(hashtable_t *h, int connection_fd) {
 
      hashnode_t *hLook;
      char *key = malloc(sizeof(char) * MAXBUFFER);
      readString(connection_fd, key);
       //char *k;
       int hashCode = hash(key);
       int b = hashCode % h->size;
       hLook = h->buckets[b]; 
       
       //pthread_mutex_lock(&h->mutexes[b]);
      
         if(hLook == NULL ){
            //printf("look up false started\n");
            writeString(NULL, connection_fd);
        
         }
         
         else {
        //pthread_mutex_unlock(&h->mutexes[b]); 
          //printf("look up started\n");
           writeString(hLook->value, connection_fd);
         ///return NULL;
         }
         
}


/*
* Remove the key-value pair with the given key from the table. The value string
* is returned to the client, or a NULL string is returned if the key is not
* in the table.
*
* Inputs
*    hashtable *h: the hashtable performing the insertion
*    int connection_fd: the socket representing the connection to the client
* Output
*    Nothing; the value string is sent to the client over the socket
*/
void hashtableRemove(hashtable_t *h, int connection_fd) {
  
  char *key = malloc(sizeof(char) * MAXBUFFER);
  readString(connection_fd, key);
  int hashCode = hash(key);
 
     int b = hashCode % h->size;
      char *v;
      hashnode_t *hRemove;
      hashnode_t *previous;
      //pthread_mutex_lock(&h->mutexes[b]);
      hRemove = h->buckets[b];
      if(h->buckets[b] == NULL){
        //pthread_mutex_unlock(&h->mutexes[b]);
        writeString(NULL, connection_fd);
        return;
      }
    
          if(strcmp(hRemove->key, key) == 0){
          v = hRemove->value;
          h->buckets[b] = hRemove->next;
         // pthread_mutex_unlock(&h->mutexes[b]);
          writeString(v, connection_fd);
          return;
         }
       
         else {
         while ((hRemove != NULL) && (strcmp(hRemove->key, key) != 0)){
             
             previous = hRemove;
             hRemove = hRemove->next;
         }
             if(hRemove == NULL){
               //pthread_mutex_unlock(&h->mutexes[b]);
              // return NULL;
              writeString(NULL, connection_fd);
             }
             else {
               v = hRemove->value;
               previous->next = hRemove->next;
               free(hRemove);
             // pthread_mutex_unlock(&h->mutexes[b]);
               //return v;
               writeString(v, connection_fd);
             }
         }
         
}


/*
* Print the hastable to the server's console
*
* Input:
*     hastable_t *h: pointer to the table
*/
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

void * consumer(void *arg) { // function that the workers run 
    int tmp = 0;
    hashtable_t *table = (hashtable_t *) arg; 
    while (tmp != -1) {

       // Wait for an item to appear in the buffer
	   sem_wait(&full);

       // Lock the buffer
	   sem_wait(&mutex);

       // Get the next item
	   tmp = do_get();
    // add code to read from it and then call the right function
   // char buf[MAX_CONNECTION_QUEUE_LEN];
    int connection_fd = tmp;
    if (connection_fd < 0) {
      perror("accept");
      exit(1);
    }
    char c;
    // Read the first char from connection_fd to identify the command
    int rc = read(connection_fd, &c, sizeof(char));
    //printf("Message: %s\n", buf);
    // Decode the command and call the appropriate hash table function
    if(c == 'I'){
      hashtableInsert(table, connection_fd);
      //hashtablePrint(table);
     
    }
    else if(c == 'L'){
      hashtableLookup(table, connection_fd);
    }
    else if(c == 'R' ){
      hashtableRemove(table, connection_fd);
    }
    else{
      hashtablePrint(table);
    }
    // Close the connection
    rc = close(connection_fd);
    if(rc < 0 ){
      perror("close");
      exit(1);
    }
    
       // Release the buffer lock
	   sem_post(&mutex);

       // Signal that a free spot has opened
	   sem_post(&empty);
	   //printf("%d %d\n", (int)arg, tmp);
    }
    return NULL;
}

/*
* Main: create the server connection and make it listen for client messages
*/
int main(int argc, char** argv) {

  // Create the socket
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    perror("socket");
    return(-1);
  }

  // Initialize the sockaddr structure
  int port = PORT;

  struct sockaddr_in sa;
  bzero((char *) &sa, sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = htonl(INADDR_ANY);
  sa.sin_port = htons((unsigned short) port);

  // Bind the socket descriptor to the address
  int rc = bind(server_fd, (struct sockaddr*) &sa, sizeof(sa));
  if (rc < 0) {
    perror("bind");
    return(-1);
  }

  // Make the socket listen for incoming requests
  int max_queue_length = MAX_CONNECTION_QUEUE_LEN;
  
  rc = listen(server_fd, max_queue_length);
  if (rc < 0) {
    perror("listen");
    return(-1);
  }

  // Initialize a new hash table
  buffer = malloc(BUF * sizeof(int));
  hashtable_t * table = hashtableInit(25);
    sem_init(&empty, pshared, BUF); // max are empty -- max is the number of spots in buffer
    sem_init(&full, pshared, 0);    // 0 are full
    sem_init(&mutex, pshared, 1);   // mutex -- buffer is initially unlocked

    // Create threads
    pthread_t cid[CMAX];
   int i;
    for (i = 0; i < CMAX; i++) {
	   pthread_create(&cid[i], NULL, consumer, table); 
    }
  // Loop, accepting and processing incoming messages
  while (1) {

    // accept blocks until a client connection arrives
    socklen_t sa_len = sizeof(sa);
    int connection_fd = accept(server_fd, (struct sockaddr*) &sa, &sa_len);
    if (connection_fd < 0) {
      perror("accept");
      return(-1);
    }
    
    //printf("Server received connection.\n");

       // Wait for a buffer spot to open
	   sem_wait(&empty);

       // Lock the buffer
	   sem_wait(&mutex);

       // Insert a new item
	   do_fill(connection_fd);

       // Release the buffer lock
	   sem_post(&mutex);

       // Signal that a new item is present
	   sem_post(&full);
    
    
  }

  return 0;
}
