#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

void usage(char *prog) {
    fprintf(stderr, "usage: %s <-s random seed> <-n number of records> <-o output file>\n", prog);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 7) {
        usage(argv[0]);
    }

    // Default arguments
    int randomSeed  = 0;
    int recordsLeft = 0;
    char *outFile   = "/no/such/file";

    // Process input parameters
    // getopt is a useful function for parsing command line arguments
    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "n:s:o:")) != -1) {
	
        switch (c) {
            case 'n':
                recordsLeft = atoi(optarg);
	    break;
	        case 's':
	            randomSeed  = atoi(optarg);
	            break;
	        case 'o':
	            outFile     = strdup(optarg);
	        break;
	        default:
	            usage(argv[0]);
	   }
    }

    // Seed random number generator
    srand(randomSeed);

    // Open and create output file
    // Always check for errors!
    int fd = open(outFile, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    if (fd < 0) {
        perror("open");
	    exit(1);
    }

    int r;
    int i;
    for (i = 0; i < recordsLeft; i++) {
        // Generate random int
        r = rand() % 0xFFFFFFFF;
        
        // Write to output file
        int rc = write(fd, &r, sizeof(int));
        
        // Verify that the correct number of bytes were written
	    if (rc != sizeof(int)) {
            perror("write");
	        exit(1);
	        // should probably remove file here but ...
	   }
    }

    // Ok to ignore error code here, because we're done anyhow...
    close(fd);

    return 0;
}

