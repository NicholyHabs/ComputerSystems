#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_LENGTH 64



int cmp( const void *a, const void *b) 
{ 
    
    return strcmp(*(char* const*) a, *(char* const*) b);
}

int int_cmp(const void *a, const void *b) 
{ 
    const int *ia = (const int *)a; 
    const int *ib = (const int *)b;
    return *ia  - *ib; 

} 

void print_cstring_array(char **array, size_t len) 
{ 
    size_t i;
 
    for(i=0; i<len; i++) 
        printf("%s | ", array[i]);
 
    putchar('\n');
} 
void print_int_array(const int *array, size_t len) 
{ 
    size_t i;
 
    for(i=0; i<len; i++) 
        printf("%d | ", array[i]);
 
    putchar('\n');
} 
 

int main(int argc, char *argv[]) {
    char *c[]= { "b", "d", "x", "c", "a"};
    int num[] = {1, 52, 15512, 12514123, 12124};
    
     qsort(c, 5, sizeof(char *), cmp );
     qsort(num, 5, sizeof(int), int_cmp);
     
    print_cstring_array(c, 5);
    print_int_array(num,5);
   return 1; 
}