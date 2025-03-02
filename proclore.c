#include "headers.h"

int proclore(int a){
    char file1[1000];
    snprintf(file1,sizeof(file1),"/proc/%d/stat",a);
    FILE *fptr=fopen(file1,"r");
    if(fptr==NULL){
        printf(ERROR_COLOR"Process not found\n"ERROR_COLOR);
        return 0;
    }
    printf("pid : %d\n",a);
    char temp[10000];
    fgets(temp,10000,fptr);
    char status;
    int pgid;
    int tty;
    int tpgid = tcgetpgrp(a);
    sscanf(temp, "%*d %*s %c %*d %d %*d %d", &status, &pgid, &tty);
    fclose(fptr);
    snprintf(file1,sizeof(file1),"/proc/%d/statm",a);
    fptr=fopen(file1,"r");
    int vm;
    fscanf(fptr,"%d",&vm);
    fclose(fptr);
    snprintf(file1,sizeof(file1),"/proc/%d/exe",a);
    fptr=fopen(file1,"r");
    char path[1000];
    int n=readlink(file1,path,sizeof(path));
    printf("%d\n",n);
    path[n]='\0';
    fclose(fptr);
    long page_size = sysconf(_SC_PAGESIZE);
    if (page_size == -1) {
        perror("Error getting page size");
        return 1;
    }
    vm=vm*page_size;
    if(pgid!=tpgid){
        printf("Process id :%d\nStatus : %c\nProcess group id: %d\nVirtual memory : %d\nPath of the executable : %s\n",a,status,pgid,vm,path);
    }
    else {
        if(tty==0) printf("Process id :%d\nStatus : %c\nProcess group id: %d\nVirtual memory : %d\nPath of the executable : %s\n",a,status,pgid,vm,path);
        else printf("Process id :%d\nStatus : %c+\nProcess group id: %d\nVirtual memory : %d\nPath of the executable : %s\n",a,status,pgid,vm,path);
    }
    strcpy(path," ");
}



