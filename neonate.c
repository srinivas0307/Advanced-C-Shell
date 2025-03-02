#include "headers.h"

int max_pid(){
    int max_pid=-1;
    FILE *fptr=fopen("/proc/loadavg","r");
    if(fptr==NULL){
        printf(ERROR_COLOR"Process not found\n"DEFAULT_COLOR);
        return 0;
    }
    char temp[10000];
    fgets(temp,10000,fptr);
    sscanf(temp, "%*f %*f %*f %*d%*c%*d %u", &max_pid);
    return max_pid;
}

int neonate(int n){
    char ch;
    struct termios newt,oldt;
    tcgetattr(STDIN_FILENO,&oldt);
    newt=oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    tcsetattr(STDIN_FILENO,TCSANOW,&newt);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    while(1){
        int pid=max_pid();
        // if(pid>0){
            printf("--%d--\n",pid);
        // }
        sleep(n);
        ch=getchar();
        if(ch=='x'){
            break;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    return 0;
    
}

int ping(int pid,int sig){
    pid=pid%32;
    printf("%d\n",pid);
    if(kill(pid,0)!=0){
        printf(ERROR_COLOR"Process not found\n"DEFAULT_COLOR);
        return 0;
    }
    if(kill(pid,sig)==-1){
        perror("Error");
        return 0;
    }
    return 1;
}