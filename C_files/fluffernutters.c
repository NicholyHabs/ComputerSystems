// Setup for the Fluffernutter problem
// DSM, 2016

// A fluffernutter is a "sandwich" made from three ingredients: white bread,
// marshmallow fluff, and peanut buffer.

// Here's the setup. There is a producer, called the "dealer" who has an
// infinite supply of the three items. There are three consumers, each of whom
// possesses an unlimited supply of two of the items required to construct
// a fluffernutter, but needs the third. For example, the first consumer has
// all the fluff and peanut butter he could want, but needs bread.

// The dealer generates a random item and places it on the shared table space.
// The consumer who needs the item takes it, constructs the fluffernutter, and
// eats it. The process repeats over and over again.

// This is a variation of Dijkstra's "cigarette-smokers' problem". The original
// three items were tobacco, rolling paper, and matches.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Shared item variable
int item;

// Number of dealer iterations to run
int loops;

// Add code here to declare a mutex and one or more condition variables
// Question: how many CVs do you need?

// Item value definitions
#define EMPTY 0
#define BREAD 1
#define FLUFF 2
#define PEANUTBUTTER 3

/*** Code for the dealer ***/
void dealer() {
  int i;
  for (i = 0; i < loops; i++) {

    // Check if the item value is 0; if it's not, wait on a condition variable
    // for a signal from a consumer

    // Once the item is EMPTY, generate a new random item value

    // Signal the appropriate condition variable for generated item

  }
}

/*** Code for the consumer ***/
void consumer(void *desiredItem) {

  while (1) {

    // Check if the desired item is on the table

    // If it's not, wait on a CV until it becomes available

    // Reset the item value to EMPTY, then signal the producer's CV
  }
}

/*** Main ***/
int  main(int argc, char *argv[]) {
  loops = atoi(argv[1]);

  // Create one producer and three consumer threads

  // Launch the threads to run their associated routines

  // Wait for the producer to finish, then exit
}
