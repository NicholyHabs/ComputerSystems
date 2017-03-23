#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>

int max;
int loops;
int *buffer;

int use  = 0;
int fill = 0;

// Declare three semaphores
sem_t empty; // consumers to signal something is taken out of array
sem_t full; // server to signal something is put in to array
sem_t mutex; // locking and unlocking

#define CMAX (10)
int consumers = 1;

void do_fill(int value) { // read into shared buffer
    buffer[fill] = value;
    fill++;
    if (fill == max)
	fill = 0;
}


int do_get() { // write into shared buffer
    int tmp = buffer[use];
    use++;
    if (use == max)
	use = 0;
    return tmp;
}


void * producer(void *arg) { // servers main loop -- server is the producer

    int i;
    for (i = 0; i < loops; i++) {

       // Wait for a buffer spot to open
	   sem_wait(&empty);

       // Lock the buffer
	   sem_wait(&mutex);

       // Insert a new item
	   do_fill(i);

       // Release the buffer lock
	   sem_post(&mutex);

       // Signal that a new item is present
	   sem_post(&full);
    }

    // Write -1s to the buffer to signal the
    // consumers to end
    for (i = 0; i < consumers; i++) {
	   sem_wait(&empty);
	   sem_wait(&mutex);
	   do_fill(-1);
	   sem_post(&mutex);
	   sem_post(&full);
    }

    return NULL;
}
                                                                               
void * consumer(void *arg) { // function that the workers run 
    int tmp = 0;
    while (tmp != -1) {

       // Wait for an item to appear in the buffer
	   sem_wait(&full);

       // Lock the buffer
	   sem_wait(&mutex);

       // Get the next item
	   tmp = do_get();
    // add code to read from it and then call the right function
       // Release the buffer lock
	   sem_post(&mutex);

       // Signal that a free spot has opened
	   sem_post(&empty);
	   printf("%d %d\n", (int)arg, tmp);
    }
    return NULL;
}


int main(int argc, char *argv[]) {
    if (argc != 4) {
	   fprintf(stderr, "usage: %s <buffersize> <loops> <consumers>\n", argv[0]);
	   exit(1);
    }
    max   = atoi(argv[1]);
    loops = atoi(argv[2]);
    consumers = atoi(argv[3]);
    assert(consumers <= CMAX);

    buffer = (int *) Malloc(max * sizeof(int));
    int i;
    for (i = 0; i < max; i++) {
	   buffer[i] = 0;
    }

    // Initialize semaphores
    sem_init(&empty, max); // max are empty -- max is the number of spots in buffer
    sem_init(&full, 0);    // 0 are full
    sem_init(&mutex, 1);   // mutex -- buffer is initially unlocked

    // Create threads
    pthread_t pid, cid[CMAX];
    pthread_create(&pid, NULL, producer, NULL); 
    for (i = 0; i < consumers; i++) {
	   pthread_create(&cid[i], NULL, consumer, (void *)i); 
    }

    // Wait for all threads to complete
    pthread_join(pid, NULL); 
    for (i = 0; i < consumers; i++) {
	   pthread_join(cid[i], NULL); 
    }
    return 0;
}

