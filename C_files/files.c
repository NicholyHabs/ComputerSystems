// demonstrate opening, reading, writing, and closing files in C
/// NGH 2016

// There are two basics wats to interact with ciles in C: the stream-based way 
// and the byte-based way

// The stream-based way is similar to Java file operations: open a file, get a
// reference to that file, and then read and write using the reference
// In C, the reference has type FILE *

// The byte-based way is similar, but uses a lower-level interface

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>


int main(){
    
    // To open a file, use open
    // Two arguments: string with the filename
    // Second arg: is a set of flags that specify how the file is going to be used
    // To open a file read-only, pass O_RDONLY flag
    
    // The return type is int and is called "the file deiscriptor"
    // By default, every new program gets three file descriptiors for free
    // 0 -> standard input
    // 1-> standard output
    // 2 -> standard error (usally the same as standard output)
    int fid = open("test.txt", O_RDONLY);
    if(fid < 0){
        perror("open");
        exit(1);
    }
    printf("%d\n", fid);
    
    // Read fromt the file and print its contents to the console
    // The basic command to read from an open file is read
    while(1){
        //read takes three arguments: the file descriptor of the open file, 
        // a pointer to a location that will receive the read data, the number, 
        // of bytes to read
        
        // read returns the number of bytes that it read OR
        // 0 if reached the end of the file OR
        // a number less than 0 if it encountered an error
        char c;
        int rc = read(fid, &c, sizeof(char));
        
        // add some cases to check for the end of the file and errors
        if(rc== 0){// end of the file
        break;
        }
        if(rc< 0){// Error
            perror("read"); // print a message indicating what type of error happened
            exit(1); // exit on 1 to indicate an error occured
        }
        printf("%c", c);
    }
    
    // Close the file associated with fid
    close(fid);
    
    return 0;
    
}