// Working with files in C
// DSM, 2015

// Copy the contents of one file to another

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    
    // Check for the correct number of command-line arguments
    if (argc != 3) {
        printf("Usage: ./file_copy_2 inputfile outputfile\n");
        exit(0);
    }
    
    // Get the filenames from the command-line arguments
    // argv[0] is the name of the program
    char *inFileName = argv[1];
    char *outFileName = argv[2];

    // Open the files and check for errors
    int in = open(inFileName, O_RDONLY); // int is file descriptor 
    if (in < 0) {
        perror("input file open");
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
    char c;
    while (1) {
        int rc = read(in, &c, sizeof(char));
        
        // Check for EOF
        if (rc == 0) {
            break;
        }
        
        // Check for error
        if (rc < 0) {
            perror("read");
            exit(1);
        }
        
        // Write the character
        int wc = write(out, &c, sizeof(char));
        
        // Write returns the number of bytes written
        // Make sure this number is correct
        if (wc != sizeof(char)) {
            perror("write");
            exit(1);
        }
    }
    
    // Close the files
    close(in);
    close(out);
    
    return 0;
}