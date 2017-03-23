// Setting up a server-side socket
// DSM, 2016

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char** argv) {

  // Create the socket
  // Returns a descriptor that refers to the new connection
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

  // Accept incoming requests
  printf("Server is running and will accept incoming requests.\n\n");
  
  // A buffer to receive messages
  char buf[140];

  // Loop, accepting and processing incoming messages
  while (1) {
    
    // accept blocks until a client connection arrives
    socklen_t sa_len = sizeof(sa);
    int connection_fd = accept(server_fd, (struct sockaddr*) &sa, &sa_len);
    if (connection_fd < 0) {
      perror("accept");
      return(-1);
    }

    printf("Accepted a request.  Descriptor = %d\n", connection_fd);

    rc = read(connection_fd, &buf, sizeof(buf));
    printf("Message: %s\n", buf);
    
    // Add some code to write a response back to the client...
    int wc = write(connection_fd, "Message received", 140);
    // Close the connection
    rc = close(connection_fd);
    if (rc < 0){
      perror("close");
      return(-1);
    }
  }
}
