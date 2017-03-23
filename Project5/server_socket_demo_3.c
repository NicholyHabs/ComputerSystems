// Reading strings from a server-side socket
// DSM, 2016

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_BUFFER_LEN 1024

/*** How does this routine work? ***/
void readString(int fd, char *buffer) {
  
    int i = 0;
    while (1) {
      read(fd, &buffer[i], sizeof(char));
      
      if (buffer[i] == '\0')
        break;
      else 
        i++;
    }
}

int main(int argc, char** argv) {

  // Create the socket
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    perror("socket");
    return(-1);
  }

  // Initialize the sockaddr structure
  int port = 10237;

  struct sockaddr_in sa;
  bzero((char *) &sa, sizeof(sa));
  sa.sin_family = AF_INET; 
  sa.sin_addr.s_addr = htonl(INADDR_ANY); 
  sa.sin_port = htons((unsigned short) port);

  // Bind the socket descriptor to the address
  int rc = bind(server_fd, (struct sockaddr*) &sa, sizeof(sa));
  if (rc < 0) {
    perror("bind");
    return(-1);
  }
 
  // Make the socket listen for incoming requests
  int max_queue_length = 1024;
  
  rc = listen(server_fd, max_queue_length);
  if (rc < 0) {
    perror("listen");
    return(-1);
  }
  
  // Loop, accepting and the processing incoming messages
  while (1) {
    
    // accept blocks until a client connection arrives
    socklen_t sa_len = sizeof(sa);
    int connection_fd = accept(server_fd, (struct sockaddr*) &sa, &sa_len);
    if (connection_fd < 0) {
      perror("accept");
      return(-1);
    }
    
    // Read a message that has three components
    char cmd;
    char key[128];
    char value[1024];

    rc = read(connection_fd, &cmd, sizeof(char));
    printf("Command: %c\n", cmd);
    
    readString(connection_fd, key);
    printf("Key: %s\n", key);

    readString(connection_fd, value);
    printf("Value: %s\n", value);
    
    // Send a response back to the client
    char * msg = "Done!";
    write(connection_fd, msg, strlen(msg) + 1);

    // Close
    rc = close(connection_fd);
    if (rc < 0){
      perror("close");
      return(-1);
    }
  }
}
