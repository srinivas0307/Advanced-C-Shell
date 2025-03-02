#include "headers.h"
#include<stdlib.h>
#include<string.h>

int display_info(char * his){
    // printf("%s\n",currdir);
    char buffer[max_size];
    int t=getlogin_r(buffer,sizeof(buffer));
    if(t==0){
        printf(DEFAULT_COLOR"<"SUCCESS_COLOR"%s@",buffer);
    }
    else {
        printf(ERROR_COLOR "Error\n" DEFAULT_COLOR);
        return 0;
    }
    char hostname[max_size];
    int test= gethostname(hostname,sizeof(hostname));
    if(test==0){
        printf("%s"DEFAULT_COLOR":", hostname);
    }
    else{
        printf("Error\n");
    }

    char cwd[max_size];
    getcwd(cwd,sizeof(cwd));
    // printf(BLUE_COLOR"%s\n"DEFAULT_COLOR, cwd);
    
    if(getcwd(cwd,sizeof(cwd))!=NULL){
        if(strncmp(cwd,his,strlen(his))==0){
            printf(BLUE_COLOR"~%s"DEFAULT_COLOR">",cwd+strlen(his));
        }
        else{
            printf(BLUE_COLOR"~%s"DEFAULT_COLOR">",cwd);
        }
    }
    else{
        perror("Error\n");
    }

    return 0;
}