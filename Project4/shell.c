/// NGH CMS 330 
// Project 4 
// Make a shell for basic command line functions


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAX_LENGTH 64
int main(){
    
    char *cwd;
    int rc;
    char buffer[MAX_LENGTH];
    int finish = 1; 
    char args[MAX_LENGTH];
    char *argv[MAX_LENGTH];
    
    char *redirect[MAX_LENGTH];

    //add while loop until "exit"
    while (finish != 0){
    int length = 0;
     
    printf("shell$ ");
    fgets(args, MAX_LENGTH, stdin );
    char *token;
    char *space = " \n";
    int i = 0; 
  
    token = strtok(args, space);
    while( token != NULL ) {
    argv[i] = token;
    i++;
    length++;
    token = strtok(NULL, space);
    } 
    argv[i] = NULL;
    
    if (length == 0){
        finish = 1;
    }
    else if(strcmp(argv[0], "cd") == 0){
        char *path = argv[1]; 
        int y;
        y = chdir(path);
        if(y == 0){
        printf("%s\n", path);
        }
        else { printf("error");}
      
    }
    else if(strcmp(argv[0] , "pwd") ==0){
        cwd = getcwd(buffer, MAX_LENGTH);
        printf("%s\n", cwd);
        
    }
    else if(strcmp(argv[0], "exit" )== 0){
        finish = 0; 
    }
    

  else{
    rc = fork();  
    int exec = 0; 
    // child 
    if(rc == 0){
    int i; 
    int w;  
  
  
  //  execvp(x , argv);
    for(i = 0; i< length; i++){
        
    if(strcmp(argv[i], "&" )== 0){
        rc = 1; 
        return -1; 
    }
      for(w = 0; w< length; w++){
         if(strcmp(argv[w], ">") == 0){
            int y = w++;
            int z; 
            for(z = 0; z <= y; z++){
                redirect[z] = argv[z];
            }
            
            close(1);
            open(argv[w++], O_WRONLY | O_CREAT, S_IRWXU);
            execvp(redirect[0], redirect);
            
            return -1;          
        }
    
      
        else {
            exec = 1; 
        }
      }
    }  
        if(exec != 0){
            execvp(argv[0], argv); 
        }
    perror("execvp");
    return -1;
    }
    
    // parent 
    else if(rc> 0){
       if(rc == 1){
           return -1;  
       }
     else{   int rc = wait(NULL);
        if (rc < 0) {
            perror("wait");
            return -1;}
        }
    }
    // error 
    else {
        perror("fork");
        
    } 
  }
    }
    return 0; 
    
}