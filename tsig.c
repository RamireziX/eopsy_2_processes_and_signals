#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>

#define WITH_SIGNALS

#ifdef WITH_SIGNALS
//global variable for my handler
bool mark = false;

//own SIGINT handlers
void sigIntHandler(int sig_num){
    // to podobno nie potrzebne signal(SIGINT, sigIntHandler); 
    printf("parent[%d]: received interrupt, shutting down child processes\n", getpid()); 
    mark = true;
}
//to powinien byc handler do sigterm
void sigIntHandlerChild(int sig_num){
    signal(SIGINT, sigIntHandler); 
    printf("child[%d]: interrupted, terminating execution\n", getpid()); 
}
#endif

//parent process
void childProcesses(int howMany){
    pid_t forkResult;
    //ignore all signals
    #ifdef WITH_SIGNALS
    signal(SIGINT, SIG_IGN);
    //except for SIGCHLD
    signal(SIGCHLD, SIG_DFL);
    //own SIGINT handler
    signal(SIGINT, sigIntHandler); 
    #endif
    // create NUM_CHILD children
    //tutaj sprawdzic flage, trzymac childy w tablicy i wyslac sigterm kazdemu
    for(int i=0;i<howMany;i++)
    {   
        forkResult = fork();
        // child process
        if(forkResult == 0) 
        { 
            #ifdef WITH_SIGNALS
            //if mark is set, kill all child processes to jest zle, musi byc w parencie
            if(mark == true){
                printf("parent[%d]: creation of child[%d] interrupted\n", getppid(), getpid()); 
                kill(getpid(), SIGTERM);
            }
            //ignore all signals
            signal(SIGINT, SIG_IGN);
            //own SIGINT handler
            //signal(SIGINT, sigIntHandlerChild); 
            //signal(SIGTERM, sigIntHandlerChild); //tutaj handler do sigterm trzeba zrobic
            //spytaj o pytanie na mailu 
            #endif
            printf("child[%d]: identifier of the parent process: %d\n", getpid(), getppid()); 
            //sleep 10 seconds (execution)
            sleep(10);
            printf("child[%d]: execution complete\n", getpid()); 
            exit(0); 
        }
        //error when creating a child
        else if(forkResult < 0){
            fprintf(stderr, "child[%d]: error\n", getpid()); 
        }
        //sleep 1 second between creating children
        sleep(1);
    }
    #ifdef WITH_SIGNALS
    if(mark != true)
    #endif
    printf("parent[%d]: child processes created successfully\n", getpid());
    for(int i=0;i<howMany;i++){
        // wait for child processes to end
        wait(NULL); 
    }
    printf("parent[%d]: no more child processes\n", getpid());
    //restore old handlers
    #ifdef WITH_SIGNALS
    signal(SIGINT, SIG_DFL);
    #endif
}

int main() {
   
    int NUM_CHILD = 5;
    childProcesses(NUM_CHILD);

    return 0;
}

