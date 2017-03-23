#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int max;
int balance = 0; // shared global variable

void * mythread(void *arg) {
	// Local variables on thread-specific stack
    char *letter = arg;
    int i;

    printf("%s: begin [addr of i: %p]\n", letter, &i);
    
    // Access a shared global variable
    for (i = 0; i < max; i++) {
		balance = balance + 1; // shared: only one
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

    printf("main: begin [balance = %d] [%p]\n", balance, &balance);

    // Use pthread_t to declare a new thread
   	pthread_t p1, p2;

    // pthread_create starts each thread
    //    -- includes the function to run
    //       and any arguments for the function
    pthread_create(&p1, NULL, mythread, "A"); 
    pthread_create(&p2, NULL, mythread, "B");
    
    // join waits for the threads to finish
    pthread_join(p1, NULL); 
    pthread_join(p2, NULL); 
    printf("main: done\n [balance: %d]\n [should: %d]\n", 
	   balance, max*2);
    return 0;
}

