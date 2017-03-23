#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int createClientSocket() {
  
  // Initialize the address structure
  struct addrinfo *servinfo;
  struct addrinfo hints;

  // hints sets up the parameters for the connection
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  int rc = getaddrinfo("localhost", "10237", &hints, &servinfo);
  if (rc != 0) {
    perror("getaddrinfo");
    exit(1);
  }
  
  // Create the socket
  int client_fd = socket(servinfo->ai_family, servinfo->ai_socktype, 0);
  if (client_fd < 0) {
    perror("socket");
    return(-1);
  }
  
  // Connect the socket
  rc = connect(client_fd, servinfo->ai_addr, servinfo->ai_addrlen);
  if (rc < 0) {
    perror("connect");
    exit(1);
  }

  return client_fd;
}


int main(int argc, char** argv) {

  char msg[128];

  while (1) {
    
    // Read from the console
    printf("$ ");
    fgets(msg, 128, stdin);

    // Open a new socket
    int clientSocket = createClientSocket();
    
    // Send the message
    int rc = write(clientSocket, msg, 128);
    if (rc < 0) {
      perror("write");
      exit(1);
    }

    // Close the socket
    close(clientSocket);
  }
}
