#include "headers.h"
aliasnd aliasarray[max_size];
int aliascount;

void checkforalias(char *str){
    // printf("%s--\n",str);
    char dummy[max_size];
    strcpy(dummy,str);
    char *temp=strtok(dummy," ");
    for(int i=0;i<aliascount;i++){
        if(strcmp(temp,aliasarray[i].name)==0){
            strcpy(str,aliasarray[i].command);
            str[strcspn(str, "\n")] = '\0';
            while(temp!=NULL){
                temp=strtok(NULL," ");
                if(temp!=NULL){
                    strcat(str," ");
                    strcat(str,temp);
                }
            }
            return;
        }
    }
}

int load(){
    FILE *ptr;
    ptr=fopen(".myshrc","r");
    if(ptr==NULL){
        return 0;
    }
    char str[max_size];
    char *p;
    while(fgets(str,max_size,ptr)!=NULL){
        char *temp=strtok_r(str,"=",&p);
        strtok(temp," ");
        strcpy(aliasarray[aliascount].name,temp);
        temp=strtok_r(NULL,"=",&p);
        while(temp[0]==' '){
            temp++;
        }
        strcpy(aliasarray[aliascount].command,temp);
        // printf("%s-%s\n",aliasarray[aliascount].name,aliasarray[aliascount].command);
        aliascount++;
    }
    return 0;
}