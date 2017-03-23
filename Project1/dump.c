#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

void usage(char *prog) {
    fprintf(stderr, "usage: %s <-i file>\n", prog);
    exit(1);
}

int main(int argc, char *argv[]) {

	if (argc != 3) {
		usage(argv[0]);
	}
    
    // Default argument
    char *inFile = "/no/such/file";

    // Get input parameters
    // getopt is a useful function for parsing command line arguments
    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "i:")) != -1) {
		switch (c) {
	    	case 'i':
	        	inFile = strdup(optarg);
	           	break;
	       	default:
	           	usage(argv[0]);
	   }
    }

    // open input file and check for errors
    int fd = open(inFile, O_RDONLY);
    if (fd < 0) {
		perror("open");
	   	exit(1);
    }

    int r;
    while (1) {
        
    	// Read a single int from the file
        int rc;
	    rc = read(fd, &r, sizeof(int));
        
        // Check for end of file
        // 0 indicates EOF
	    if (rc == 0)
	       break;
        
        // Check error code
	    if (rc < 0) {
	       perror("read");
	       exit(1);
	    }
        
        // Print to console
	    printf("%u", r);
	    printf("\n");
    }
    
    // Close the file
    (void) close(fd);

    return 0;
}

