#include "headers.h"
char filename[max_size]=".";
int count=0;
int c=0;
char p[max_size]=".";

int seek(int flag1,int flag2,int flag3,char *path,char *name,char *pre){
    count=0;
    strcpy(filename,".");
    strcpy(p,".");
    if(flag1==1&&flag2==1){
        printf("Invalid flags!\n");
    }
    char rel[4036];
    getcwd(rel,4036);
    if(chdir(path)==-1){
        printf("target directory not found\n");
        return 0;
    }
    char temp[4036];
    getcwd(temp,4036);
    find(name,flag1,flag2,flag3,temp);
    // printf("%s\n",p);
    // printf("%s\n",filename);
    if(strcmp(p,".")!=0){
        if(strcmp(filename,".")!=0){
            chdir(p);
            FILE *fptr=fopen(filename,"r");
            if(fptr==NULL){
                printf("file not found\n");
                return 0;
            }
            char array[4039];
            while(fgets(array,sizeof(array),fptr)){
                printf("%s",array);
            }
            printf("\n");
        }
        else{
            strcpy(pre,rel);
            chdir(p);
            return 0;
        }
    }
    chdir(rel);
    if(c==0){
        printf("No match found!\n");
    }
    return 0;
}

int find(char* name,int flag1,int flag2,int flag3,char* rel){
    char temp[1000];
    int f=0;
    getcwd(temp,1000);
    DIR *dir;
    dir=opendir(".");
    if(dir==NULL){
        printf("path not found\n");
        return 0;
    }
    struct dirent *d;
    while(d=readdir(dir)){
        if(strcmp(d->d_name,".")==0||strcmp(d->d_name,"..")==0){
            continue;
        }
        struct stat file_req;
        stat(d->d_name,&file_req);
        if(strncmp(d->d_name,name,strlen(name))==0){
            if(S_ISDIR(file_req.st_mode)&&flag2==0){
                char temp[4096];
                getcwd(temp,sizeof(temp));
                printf(BLUE_COLOR"%s",temp+strlen(rel));
                printf("/%s\n"DEFAULT_COLOR,d->d_name);
                c++;
                chdir(d->d_name);
                if(flag3==1&&flag2!=1){
                    if(count==0) getcwd(p,sizeof(p));
                    else strcpy(p,".");
                    count++;
                }
                chdir(temp);
            }
            else if(!S_ISDIR(file_req.st_mode)&&flag1==0) {
                char temp1[4096];
                getcwd(temp1,sizeof(temp1));
                printf(SUCCESS_COLOR"%s",temp1+strlen(rel));
                printf("/%s\n"DEFAULT_COLOR,d->d_name);
                c++;
                if(flag3==1){
                    if(count==0) {
                        getcwd(p,sizeof(p));
                        // printf("%s,,\n",p);
                        strcpy(filename,d->d_name);
                    }
                    else {
                        strcpy(p,".");
                        strcpy(filename,".");
                    }
                    count++;
                }
            }
        }
        if(S_ISDIR(file_req.st_mode)){
            char temp[4039];
            getcwd(temp,sizeof(temp));
            chdir(d->d_name);
            find(name,flag1,flag2,flag3,rel);
            chdir(temp);
        }
    }
    return 0;
}
