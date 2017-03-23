
/// in class use of exec 

#include <stdio.h>
#include <unistd.h>

int main(){
    char *args[4];
    args[0] = "ls";
    args[1] = "-l";
    args[2] = "-a";
    args[3] = NULL;
    
    
    execvp(args[0], args);
    
    perror("execvp");
    return -1; 
    
}