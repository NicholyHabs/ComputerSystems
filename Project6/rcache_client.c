// Client-side test program for the rCache system
#include <stdio.h>
#include "rcache.h"
#define MAXBUFFER 1024

// LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
// export LD_LIBRARY_PATH

int main(int argc, char** argv) {
  char response[MAXBUFFER];
  rInsert("a", "one");
  rInsert("b", "two");
  rInsert("c", "three");
  rPrint();

  // Add more tests here...
 //Add more tests here...
  
  //Testing all Lookups
  rLookup("a", response);
  printf("Found: %s\n", response);
  rLookup("b", response);
  printf("Found: %s\n", response);
  rLookup("c", response);
  printf("Found: %s\n", response);
  
  //Testing all Removes
  rRemove("a", response);
  printf("Removed: %s\n", response);
  rRemove("b", response);
  printf("Removed: %s\n", response);
  rRemove("c", response);
  printf("Removed: %s\n", response);
  
  printf("\n");
  printf("Values removed.\n");
  
  rPrint();
  
  //Testing NULL checks...
  
  rLookup("a", response);
  printf("Found: %s\n", response);
  
  rLookup("b", response);
  printf("Found: %s\n", response);
  
  rRemove("a", response);
  printf("Removed: %s\n", response);
  
  printf("\n");
  rPrint(); 
  return 0;
}
