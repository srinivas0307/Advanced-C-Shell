#include "headers.h"

bgnode bgarray[max_size];
int bgcount;
int fp;
extern aliasnd aliasarray[max_size];
extern int aliascount;

char home_dir[max_size];
char command[max_size];
char pre[max_size]="NULL";
char his[max_size];

void ctrlc(int signum){
    if(fp==0){
        return;
    }
    if(kill(fp,SIGINT)==-1){
        perror("Error");
        return;
    }
}

void ctrlz(int signum){
    if(fp==0){
        return;
    }
    printf(ERROR_COLOR"\n\t%d\tstopped\n"DEFAULT_COLOR,fp);
    if(kill(fp,SIGSTOP)==-1){
        perror("Error");
        return;
    }
}

void ctrld(int signaum){
    for(int i=0;i<bgcount;i++){
        if(kill(bgarray[i].pid,0)==0){
            kill(bgarray[i].pid,SIGKILL);
        }
    }
    exit(0);
}

int main(){
    int aliascount=0;
    load();
    bgcount=0;
    fp=0;
    getcwd(his,sizeof(his));
    strcpy(home_dir,his);
    strcat(his,"/history_file.txt");
    display_info(home_dir);
    signal(SIGINT,ctrlc);
    signal(SIGTSTP,ctrlz);
    while(1){
        if(fgets(command, sizeof(command), stdin)==NULL){
            ctrld(getpid());
        }
        command[strcspn(command, "\n")] = '\0';
        if(command==NULL||strcmp(command,"")==0||strcmp(command," ")==0){
            display_info(home_dir);
            continue;
        }
        checkforalias(command);
        if(strcmp(command,"\0") == 0){
            continue;
        }
        if(strcmp(command,";")==0){
            printf("syntax error near unexpected token `;'\n");
            continue;
        }
        add(command,his);
        char* ptr;
        char * str=strtok_r(command,";",&ptr);
        char dummy[4039];
        strcpy(dummy,str);
        int flag;
        int time=0;
        char c[max_size];
        while(str!=NULL){
            char temp[max_size];
            strcpy(temp,str);
            flag=function(temp,pre,home_dir);
            if(flag>0){
                strcpy(c,str);
                time=flag;
            }
            str=strtok_r(NULL,";",&ptr);
        }
        display_info(home_dir);
        if(time!=0){
            char *temp1=strtok(c," ");
            printf("Time taken by '%s': %d seconds\n", temp1, time);
        }
    };
    return 0;
}