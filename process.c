#include "headers.h"

extern int fp;
extern int bgcount;
extern bgnode bgarray[max_size];

int check(char *str){
    char *temp=strtok(str," ");
    int count=0;
    while(temp!=NULL){
        if(temp[strlen(temp)-1]=='&'){
            count++;
        }
        temp=strtok(NULL," ");
    }
    return count;
}

void sigchld_handler(int signo) {
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            for(int i=0;i<bgcount;i++){
                if(bgarray[i].pid==pid){
                    printf("%s exited normally (%d)\n",bgarray[i].command,pid);
                    for(int j=i;j<bgcount-1;j++){
                        bgarray[j]=bgarray[j+1];
                    }
                    bgcount--;
                    break;
                }
            }
        } else if (WIFSIGNALED(status)) {
            int signal_number = WTERMSIG(status);
            for(int i=0;i<bgcount;i++){
                if(bgarray[i].pid==pid){
                    printf("%s terminated by signal %d (%d)\n",bgarray[i].command,signal_number,pid);
                    for(int j=i;j<bgcount-1;j++){
                        bgarray[j]=bgarray[j+1];
                    }
                    bgcount--;
                    break;
                }
            }
        }
    }
}

int background(char *str){
    char *first=strtok(str," ");
    char *arg[1028];
    arg[0]=first;
    int i=1;
    while(first!=NULL){
        first=strtok(NULL," ");
        arg[i]=first;
        i++;
    }
    arg[i]=NULL;
    struct sigaction sa;
    sa.sa_handler = &sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    int child=fork();
    if(child==0){
        execvp(arg[0],arg);
    }
    else if(child>0){
        printf("%d\n",child);
        bgarray[bgcount].pid=child;
        strcpy(bgarray[bgcount].command,arg[0]);
        strcpy(bgarray[bgcount].status,"Running");
        bgcount++;
        // printf("%d\n",bgcount);   
    }
    else {
        printf("fork failed\n");
    }
}

int foreground(char *arg[max_size]){
    int child=fork();
        if(child==0){
            fp=getpid();
            if(execvp(arg[0],arg)==-1){
                printf(ERROR_COLOR"Command not found\n"DEFAULT_COLOR);
            }
            return 0;
        }
        else if(child>0){
            time_t start_time, end_time;
            time(&start_time);
            fp=child;
            while(1){
                int status;
                int pid=waitpid(child,&status,WNOHANG|WUNTRACED);
                if(pid>0){
                    if(WIFEXITED(status)){
                        break;
                    }
                    else if(WIFSTOPPED(status)){
                        bgarray[bgcount].pid=child;
                        strcpy(bgarray[bgcount].command,arg[0]);
                        strcpy(bgarray[bgcount].status,"STOPPED");
                        bgcount++;
                        break;
                    }
                }
                else if(pid==0){
                    continue;
                }
                else {
                    perror("waitpid");
                    return 0;
                }
            }  
            time(&end_time);
            fp=0;
            int elapsed_time = (int)difftime(end_time, start_time);
            if (elapsed_time > 2) {
                return elapsed_time;
            }
        }
        else {
            printf("fork failed\n");
        }
}

void activities(){
    int n=bgcount;
    for(int i=0;i<n;i++){
        char file1[1000];
        snprintf(file1,sizeof(file1),"/proc/%d/stat",bgarray[i].pid);
        FILE *fptr=fopen(file1,"r");
        if(fptr==NULL){
            continue;
        }
        char temp[max_size];
        fgets(temp,max_size,fptr);
        char s;
        sscanf(temp, "%*d %*s %c", &s);
        fclose(fptr);
        if(s=='T'){
            strcpy(bgarray[i].status,"Stopped");
        }
        else {
            strcpy(bgarray[i].status,"Running");
        }
    }
    for(int i=0;i<n;i++){
        printf("[%d] %d %s %s\n",i+1,bgarray[i].pid,bgarray[i].status,bgarray[i].command);
    }
    return;
}




