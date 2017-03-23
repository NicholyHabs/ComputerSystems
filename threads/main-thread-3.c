#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int max;
volatile int balance = 0;

// Try to create our own mutex
//   0 --> free
//   1 --> locked
volatile unsigned int mutex = 0;


void spinLock(volatile unsigned int *lock) {
    while (*lock == 1) // test if lock is held
	   ; // spin
    
    *lock = 1;         // acquire lock
}


void spinUnlock(volatile unsigned int *lock) {
    *lock = 0;  // release lock
}


void * mythread(void *arg) {       
    char *letter = arg;
    printf("%s: begin\n", letter);
    int i;
    for (i = 0; i < max; i++) {
	   spinLock(&mutex);
	   balance = balance + 1;
	   spinUnlock(&mutex);
    }
    printf("%s: done\n", letter);
    return NULL;
}
 
void * mythreadDecrease(void *arg) {       
    char *letter = arg;
    printf("%s: begin\n", letter);
    int i;
    for (i = 0; i < max; i++) {
	   spinLock(&mutex);
	   balance = balance - 1;
	   spinUnlock(&mutex);
    }
    printf("%s: done\n", letter);
    return NULL;
}
int main(int argc, char *argv[]) {                    
    if (argc != 2) {
	   fprintf(stderr, "usage: main-first <loopcount>\n");
	       exit(1);
    }
    max = atoi(argv[1]);

    // Create and run pthreads
    pthread_t p1, p2;
    printf("main: begin [balance = %d]\n", balance);
    pthread_create(&p1, NULL, mythread, "A"); 
    pthread_create(&p2, NULL, mythread, "B");

    // join waits for the threads to finish
    pthread_join(p1, NULL); 
    pthread_join(p2, NULL); 

    printf("main: done\n [balance: %d]\n [should: %d]\n", 
	   balance, max*2);
    return 0;
}

