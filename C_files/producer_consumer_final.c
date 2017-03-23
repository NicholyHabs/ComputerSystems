// Solution to the Producer-Consumer problem
// DSM, 2016

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Shared global variables
int *buffer; // the buffer
int max; // size of buffer
int numfull = 0; // current number of items in the buffer
int fillptr = 0; // next position to fill
int useptr = 0; // next position to consume
int i;
int loops; // Number of production iterations
int consumers; // Number of consumer threads to create

// Lock variable
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

// TWO condition variables, one for the producer and one for the consumers
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;

/*** Routines that interact with the shared buffer ***/
void do_fill(int value) {
    buffer[fillptr] = value;
    fillptr = (fillptr + 1) % max; /// works like a circle - makes fillptr 0 at (max -1)
    numfull++;
}

int do_get() {
    int tmp = buffer[useptr];
    useptr = (useptr + 1) % max;  /// works like a circle - makes useptr 0 at (max -1)
    numfull--;
    return tmp;
}

/*** Code for the producer ***/
void * producer(void *arg) {
  for (i = 0; i < loops; i++) {
    mutex_lock(&m);
    while (numfull == max) {
      cond_wait(&empty, &m);
    }
    do_fill(i);
    cond_signal(&fill);
    mutex_unlock(&m);
  }
}

/*** Code for the consumer ***/
void * consumer(void *arg) {
  while (1) {
    mutex_lock(&m);
    while (numfull == 0) {
      cond_wait(&fill, &m);
    }
    int tmp = do_get();
    cond_signal(&empty);
    mutex_unlock(&m);
    printf("%d\n", tmp);
  }
}

/*** Main ***/
int main(int argc, char *argv[]) {

  // Command line input arguments
  max   = atoi(argv[1]); // size of buffer
  loops = atoi(argv[2]); // number of production cycles to run
  consumers = atoi(argv[3]); // number of consumers

  // Allocate the shared buffer
  buffer = (int *) malloc(max * sizeof(int));

  // Create one producer and all of the consumers
  pthread_t pid, cid[CMAX];
  pthread_create(&pid, NULL, producer, NULL);
  for (i = 0; i < consumers; i++) {
    pthread_create(&cid[i], NULL, consumer, NULL);
  }

  // Wait for the producer to finish, then end
  pthread_join(pid, NULL);
}
