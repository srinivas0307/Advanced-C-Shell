#include "headers.h"

int hop(char * str,char *pre,char *his){
    char dir[1000];
    getcwd(dir,1000);
    if(strcmp(str,".")==0){
        strcpy(pre,dir);
        printf("current working directory :%s\n",dir);
    }
    else if(strcmp(str,"..\n")==0){
        chdir(str);
        strcpy(pre,dir);
        getcwd(dir,1000);
        printf("current working directory :%s\n",dir);
    }
    else if(strcmp(str,"-")==0){
        if(strcmp(pre,"NULL")==0){
            printf("no previous directory exists\n");
            return -1;
        }
        char dummy[100];
        strcpy(dummy,dir);
        chdir(pre);
        strcpy(pre,dummy);
        getcwd(dir,1000);
        printf("current working directory : %s\n",dir); 
    }
    else if(strncmp(str,"~",1)==0){
        strcpy(pre,dir);
        chdir(his);
        chdir(str+2);
        getcwd(dir,1000);
        printf("current working directory :%s\n",dir);
    }
    else{
        int r=chdir(str);
        if(r==-1){
            printf("no such directory exists\n");
            return -1;
        }
        strcpy(pre,dir);
        getcwd(dir,1000);
        printf("current working directory :%s\n",dir);
    }
    
    return 0;
}