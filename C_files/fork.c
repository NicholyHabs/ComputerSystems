#include <stdio.h>
#include <unistd.h>

int main(){
    
    
    int rc = fork();
    
    // child 
    if(rc == 0){
        char *args[4];
    args[0] = "ls";
    args[1] = "-l";
    args[2] = "-a";
    args[3] = NULL;
    
    
    execvp(args[0], args);
    
    perror("execvp");
    return -1; 
    }
    // parent 
    else if(rc> 0){
        
    }
    // error
    else {
        perror("fork");
        
    }
    return 0; 
}