#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/wait.h>
#define WITH_SIGNALS

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
        else if(forkResult < 0){
            fprintf(stderr, "child[%d]: error\n", getpid()); 
        }
        sleep(1);
    } 
    
    printf("parent[%d]: child processes created successfully\n", getpid());
    
    for(int i=0;i<howMany;i++){
        wait(NULL); 
    }
    printf("parent[%d]: no more child processes\n", getpid());
   
}

int main() {
   
    int NUM_CHILD = 3;
    
    #ifdef WITH_SIGNALS
        return 0;
    #endif
    #ifndef WITH_SIGNALS
        childProcess(NUM_CHILD);
        return 0;
    #endif
    
    return 0;
    
    
   
}

