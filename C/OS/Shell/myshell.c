#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include<sys/wait.h>


int register_SIGINT_IGN_handling() {
    struct sigaction new_action;
    memset(&new_action, 0, sizeof(new_action));

    new_action.sa_handler = SIG_IGN;
    new_action.sa_flags = SA_RESTART;
    sigemptyset(&new_action.sa_mask);

    // Overwrite default behavior for ctrl+c
    return sigaction(SIGINT, &new_action, NULL);
}

int register_SIGINT_DFL_handling() {
    struct sigaction new_action;
    memset(&new_action, 0, sizeof(new_action));

    new_action.sa_handler = SIG_DFL;
    new_action.sa_flags = SA_RESTART;
    sigemptyset(&new_action.sa_mask);

    return sigaction(SIGINT, &new_action, NULL);
}

int prepare(void){

    //make SIGCHLD signals possible to handle zombies
    struct sigaction SIGCHLD_action;
    memset(&SIGCHLD_action, 0, sizeof(SIGCHLD_action));
    //setting the handler of SIGCHILD to SIG_IGN will prevent zombies from existing
    SIGCHLD_action.sa_handler = SIG_IGN;
    SIGCHLD_action.sa_flags = SA_RESTART;
    sigemptyset(&SIGCHLD_action.sa_mask);

    //set for zombies quick disposal
    if(sigaction(SIGCHLD,&SIGCHLD_action,NULL)!=0){
        //Signal handle registration failed
        fprintf(stderr,"%s\n", strerror(errno));
        return 1;
    }

    if(register_SIGINT_IGN_handling()==-1){// set to ignore SIGINT handling
        fprintf(stderr,"%s\n", strerror(errno));
        return 0;
    }

    return 0;
}

int foregroundProcess(int index, int count, char** arglist){
    pid_t cpid;

    int child_pid=fork();
    if (child_pid < 0) {
        // fork() failed – handle error
        fprintf(stderr,"%s", strerror(errno));
        return 0;
    }
    if (child_pid ==0) {
        //child process
        if(register_SIGINT_DFL_handling()==-1){// set default SIGINT handling
            fprintf(stderr,"%s\n", strerror(errno));
            exit(1);
        }
        execvp(arglist[0], arglist);
        // if execvp() returned then it failed – handle error
        fprintf(stderr,"%s\n", strerror(errno));
        exit(1);
    }
    else {
        // parent process
        cpid= waitpid(child_pid,NULL,WUNTRACED);
        if(register_SIGINT_IGN_handling()==-1){// set to ignore SIGINT handling
            fprintf(stderr,"%s\n", strerror(errno));
            return 0;
        }
        if(cpid==-1 && errno!=ECHILD && errno!=EINTR){//An error in wait
            fprintf(stderr,"%s\n", strerror(errno));
            return 0;
        }
    }
    return 1;
}

int backgroundProcess(int index, int count, char** arglist){

    //remove "&" to get only the command
    arglist[index]=NULL;

    int child_pid=fork();
    if (child_pid < 0) {
        // fork() failed – handle error
        fprintf(stderr,"%s\n", strerror(errno));
        return 0;
    }
    if (child_pid ==0) {
        // child process
        execvp(arglist[0], arglist);
        // if execvp() returned then it failed – handle error
        fprintf(stderr,"%s\n", strerror(errno));
        exit(1);

    }
    else {
        // parent process
    }
    return 1;
}

int pipeProcess(int index, int count, char** arglist){
    int   pipefd[2];//pipefd[0] is the reader, pipefd[1] the writer
    pid_t cpid;
    //char  buf;


    arglist[index]=NULL;//remove "|" from arglist

    if(pipe(pipefd)==-1){//pipe pipefd
        //pipe failed
        fprintf(stderr,"%s\n", strerror(errno));
        return 0;
    }

    int child_pid1=fork();
    if (child_pid1 < 0) {
        // fork() failed – handle error
        fprintf(stderr,"%s\n", strerror(errno));
        return 0;
    }
    if(child_pid1==0){
        //child1 process
        //fixed to close stdout
        if(close(1)==-1){//close stdout
            fprintf(stderr,"%s\n", strerror(errno));
            exit(1);
        }
        if(close(pipefd[0])==-1){//close reader
            fprintf(stderr,"%s\n", strerror(errno));
            exit(1);
        }
        if(dup2(pipefd[1],1)==-1){//create copy
            fprintf(stderr,"%s\n", strerror(errno));
            exit(1);
        }
        if(close(pipefd[1])==-1){//close writer
            fprintf(stderr,"%s\n", strerror(errno));
            exit(1);
        }
        if(register_SIGINT_DFL_handling()==-1){// set default SIGINT handling
            fprintf(stderr,"%s\n", strerror(errno));
            exit(1);
        }
        execvp(arglist[0],arglist);
        // if execvp() returned then it failed – handle error
        fprintf(stderr,"%s\n", strerror(errno));
        exit(1);
    }
    else{
        int child_pid2=fork();
        if (child_pid2 < 0) {
            // fork() failed – handle error
            fprintf(stderr,"%s\n", strerror(errno));
            return 0;
        }
        if(child_pid2==0){
            //child2 process
            //fixed to close stdin
            if(close(0)==-1){//close stdin
                fprintf(stderr,"%s\n", strerror(errno));
                exit(1);
            }
            if(close(pipefd[1])==-1){//close writer
                fprintf(stderr,"%s\n", strerror(errno));
                exit(1);
            }
            if(dup2(pipefd[0],0)==-1){//create copy
                fprintf(stderr,"%s\n", strerror(errno));
                exit(1);
            }
            if(close(pipefd[0])==-1){//close reader
                fprintf(stderr,"%s\n", strerror(errno));
                exit(1);
            }
            if(register_SIGINT_DFL_handling()==-1){// set default SIGINT handling
                fprintf(stderr,"%s\n", strerror(errno));
                exit(1);
            }
            execvp(arglist[index+1],arglist+index+1);
            // if execvp() returned then it failed – handle error
            fprintf(stderr,"%s\n", strerror(errno));
            exit(1);
        }
        else {
            if(close(pipefd[1])==-1){//close writer
                fprintf(stderr,"%s\n", strerror(errno));
                return 0;
            }
            if(close(pipefd[0])==-1){//close reader
                fprintf(stderr,"%s\n", strerror(errno));
                return 0;
            }
            //fixed to wait for all children not in a specific order
            while ((cpid = wait(NULL)) > 0);
            ////////////////////////////////////////////////////////

            if(register_SIGINT_IGN_handling()==-1){// set to ignore SIGINT handling
                fprintf(stderr,"%s\n", strerror(errno));
                return 0;
            }
        }
    }
    return 1;
}

int process_arglist(int count, char** arglist){
    int index=0;


    //check what kind of command we have
    while(index<count && strcmp(arglist[index],"&")!=0 && strcmp(arglist[index],"|")!=0){index++;}

    //do regular command
    if(index==count) {
        return foregroundProcess(index, count, arglist);
    }

    //do background command
    if(strcmp(arglist[index],"&")==0){
       return backgroundProcess(index, count, arglist);
    }

    //do pipe commands
    if(strcmp(arglist[index],"|")==0) {
       return pipeProcess(index, count, arglist);
    }

    if(register_SIGINT_IGN_handling()==-1){// set to ignore SIGINT handling
        fprintf(stderr,"%s\n", strerror(errno));
        return 0;
    }
    return 1;
}

int finalize(void){
    return 0;
}