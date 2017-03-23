/// Fastsort program to sort binary input and output to file
// NGH 1/28/2016
// CMS 330 Project 1 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_LENGTH 64
#define MAX_LEN 32
int cmp( const void *a, const void *b) 
{ 
    
    return strcmp(*(char* const*) a, *(char* const*) b);
}
int int_cmp(const void *a, const void *b) 
{ 
    const int *ia = (const int *)a; 
    const int *ib = (const int *)b;
    //return (ia > ib) - (ia < ib); 
    return *ia  - *ib; 
} 

int main(int argc, char *argv[]) {
    
    // Check for the correct number of command-line arguments
    if (argc != 5) {
       fprintf(stderr, "Usage: ./fastsort -i inputfile -o outputfile\n");
        exit(1);
    }
    
    // Get the filenames from the command-line arguments
    // argv[0] is the name of the program
    char *inFileName = argv[2];
    char *outFileName = argv[4];
  

    // Open the files and check for errors
    int in = open(inFileName, O_RDONLY); // int is file descriptor 
    if (in < 0) {
        fprintf(stderr, "Error: could not open file %s \n", inFileName);
        exit(1);
    }

    // O_WRONLY | O_CREAT | O_TRUNC opens the file for writing, creates it
    // if it doesn't exist, and truncates it to 0 bytes if it does exist
    //
    // S_IRWXU sets the new file to have read, write, and execute permissions
    int out = open(outFileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU); // bitwise OR statement
    if (out < 0) {
       perror("output file open");
        exit(1);
    }
    
    // Read all the bytes from the file and copy to the new file

    
    struct stat x; 
    fstat(in, &x);
    int size = x.st_size;
         
    int numberOfInts = (size/ sizeof(int));
    int r[numberOfInts]; 
    
            int rc = read(in, &r, numberOfInts * sizeof(int));
// check rc for error
            if (rc < 0) {
            perror("read");
            exit(1);
        }

  
            qsort(r, numberOfInts, sizeof(int), int_cmp );

        // sort the binary numbers 
        
       
        
        // Write the character
        int wc = write(out, &r, numberOfInts * sizeof(int));
        
        // Write returns the number of bytes written
        // Make sure this number is correct
        if (wc != numberOfInts * sizeof(int)) {
            perror("write");
            exit(1);
        }
    
    
    // Close the files
    close(in);
    close(out);
    
    return 0;
}