#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/wait.h>

void childProcess(int howMany){
    pid_t forkResult;
    for(int i=0;i<howMany;i++)
    { 
        forkResult = fork();
        if(forkResult == 0) 
        { 
            printf("child[%d]: identifier of the parent process: %d\n", getpid(), getppid()); 
            sleep(1);
            printf("child[%d]: execution complete\n", getpid()); 
            exit(0); 
        }
        sleep(1);
    } 
    for(int i=0;i<howMany;i++){
    wait(NULL); 
    }
}
int main() {
   
    int NUM_CHILD = 2;
    
    childProcess(NUM_CHILD);
    
    printf("parent[%d]: child processes created successfully\n", getppid());
    
   return 0;
}

