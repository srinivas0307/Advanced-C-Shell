#include "headers.h"
char history_file[max_size];

int add(char* input,char * p){
    strcpy(history_file,p);
    char temp[1000];
    char* str;
    char* saveptr2;
    strcpy(temp, input);
    char output[1000]="";
    str = strtok_r(temp, ";", &saveptr2);
    while(str!=NULL){
        char * dum=strtok(str," ");
        if(strcmp(dum,"log")==0){
            str=strtok_r(NULL,";",&saveptr2);
            continue;
        }
        while(dum!=NULL){
            strcat(output,dum);
            strcat(output," ");
            dum=strtok(NULL," ");
        }
        str = strtok_r(NULL, ";", &saveptr2);
        if(str!=NULL){
            strcat(output,";");
        }
    }
    if(strcmp(output,"")==0){
        return 0;
    }
    char array[15][1000];
    FILE *fptr;
    fptr=fopen(history_file,"r");
    int i=0;
    if(fptr!=NULL){
        while(fgets(array[i], 1000, fptr) != NULL&&i<14){
            array[i][strcspn(array[i], "\n")] = '\0';
            if(strcmp(array[0],output)==0){
                fclose(fptr);
                return 0;
            }
            i++;
        }
        fclose(fptr);
    }
    fptr=fopen(history_file,"w");
    fprintf(fptr,"%s\n",output);
    for(int j=0;j<i;j++){
        fprintf(fptr,"%s\n",array[j]);
    }
    fclose(fptr);
    fflush(stdout);
}

int execute(char* command){
    FILE *fptr;
    int i = atoi(command);
    if(i<1||i>15){
        return 0;
    }
    fptr=fopen(history_file,"r");
    if(fptr!=NULL){
        while(i>0&&fgets(command, 1000, fptr) != NULL){
            i--;
        }
        fclose(fptr);
    }
    return 1;
}

void history(){
    FILE* fptr;
    fptr=fopen(history_file,"r");
    if(fptr==NULL){
        return;
    }   
    char array[1000];
    while(fgets(array, 1000, fptr) != NULL){
        printf("%s",array);
    }
    fclose(fptr);
    return;
}

void purge(){
    FILE* fptr;
    fptr=fopen(history_file,"w");
    if(fptr==NULL){
        return ;
    }
    fclose(fptr);
    fflush(stdout);
    return;
}
