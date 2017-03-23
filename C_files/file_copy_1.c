// Working with files in C
// DSM, 2015

// Copy the contents of one file to another

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_LEN 32

int main(int argc, char *argv[]) {
    
    // Check for the correct number of command-line arguments
    if (argc != 3) {
        printf("Usage: ./file_copy_1 inputfile outputfile\n");
        exit(0);
    }
    
    // Get the filenames from the command-line arguments
    // argv[0] is the name of the program
    char *inFileName = argv[1];
    char *outFileName = argv[2];

    // Open the files and check for errors
    FILE *in = fopen(inFileName, "r");
    if (in == NULL) {
        perror("input file open");
        exit(1);
    }

    FILE *out = fopen(outFileName, "w");
    if (out == NULL) {
        perror("output file open");
        exit(1);
    }
    
    // Allocate an input buffer
    char *s = malloc(MAX_LEN * sizeof(char));
    
    // Read all the bytes from the file and copy to the new file
    while (1) {
        
        // fgets reads the specified number of bytes - 1 to a char * location
        //
        // It automatically places a terminating null in the last byte
        //
        // fgets stops reading if it encounters the end-of-file or a newline
        char *rc = fgets(s, MAX_LEN, in);
        
        // fgets returns NULL if it reaches the end-of-file
        if (rc == NULL) {
            break;
        }
        
        // Write the characters
        int wc = fprintf(out, "%s", s);
        
        // fprintf returns the number of chars written or a negative value 
        // if an error occurred
        if (wc < 0) {
            perror("write");
            exit(1);
        }
    }
    
    // Close the files
    fclose(in);
    fclose(out);
    
    return 0;
}