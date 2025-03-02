#include "headers.h"

int printFilePermissions(struct stat fileStat) {
    printf((S_ISDIR(fileStat.st_mode))?"d":"");
    printf((S_ISCHR(fileStat.st_mode))?"c":"");
    printf((S_ISBLK(fileStat.st_mode))?"b":"");
    printf((S_ISLNK(fileStat.st_mode))?"l":"");
    printf((S_ISFIFO(fileStat.st_mode))?"p":"");
    printf((S_ISSOCK(fileStat.st_mode))?"s":"");
    printf((S_ISREG(fileStat.st_mode))?"-":"");

    printf((fileStat.st_mode & S_IRUSR)?"r":"-");
    printf((fileStat.st_mode & S_IWUSR)?"w":"-");
    printf((fileStat.st_mode & S_IXUSR)?"x":"-");
    printf((fileStat.st_mode & S_IRGRP)?"r":"-");
    printf((fileStat.st_mode & S_IWGRP)?"w":"-");
    printf((fileStat.st_mode & S_IXGRP)?"x":"-");
    printf((fileStat.st_mode & S_IROTH)?"r":"-");
    printf((fileStat.st_mode & S_IWOTH)?"w":"-");
    printf((fileStat.st_mode & S_IXOTH)?"x":"-");
    if((S_ISDIR(fileStat.st_mode))) return 0;
    if(fileStat.st_mode & S_IXUSR || fileStat.st_mode & S_IXGRP || fileStat.st_mode & S_IXOTH) return 1;
    else return 2;
    
}

int l(char *filename){
    struct stat s;
    if(stat(filename,&s)<0){
        printf("File not found\n");
        return 0;
    }
    int dum=printFilePermissions(s);
    char timeBuf[256];
    struct tm *tm_info;
    tm_info = localtime(&s.st_mtime);
    strftime(timeBuf, sizeof(timeBuf), "%b %d %H:%M", tm_info);
    printf(" %ld %s %s %ld %s ",s.st_nlink,getpwuid(s.st_uid)->pw_name,getgrgid(s.st_gid)->gr_name,s.st_size,timeBuf);
    if(dum==0) {
        printf(BLUE_COLOR"%s\n"DEFAULT_COLOR,filename);
    }
    else if(dum==1) {
        printf(SUCCESS_COLOR"%s\n"DEFAULT_COLOR,filename);
    }
    else {
        printf("%s\n",filename);
    }
}



int compare(const void *a,const void *b){
    int i=strcmp((char*)a,(char*)b);
    return i;
}

int reveal(char *flag,char *path){
    char array[1000][4036];
    int f1=0;
    int f2=0;
    for(int j=0;j<strlen(flag);j++){
        if(flag[j]=='a') f1=1;
        else if(flag[j]=='l') f2=1;
    }
    int i=0;
    char temp[1000];
    getcwd(temp,1000);
    if(chdir(path)!=-1){
        DIR *dir;
        struct dirent *d;
        dir=opendir(".");
        if(dir!=NULL){
            while((d=readdir(dir))!=NULL){
                if(d->d_name[0]=='.'&&f1==0) continue;
                strcpy(array[i],d->d_name);
                i++;
            }
        }
        closedir(dir);
        qsort(array,i,sizeof(array[0]),compare);
        if(f2==1){
            int t=0;
            for(int j=0;j<i;j++){
                struct stat s;
                if(stat(array[j],&s)==0) 
                    t+=s.st_blocks;
                else {
                    printf("File not found\n");
                    return 0;
                }
            }
            t/=2;
            printf("total %d\n",t);
        }
        chdir(temp);
    }
    else{
        strcpy(array[0],path);
        i++;
    }
    for(int j=0;j<i;j++){
        if(f2==1){
            l(array[j]);
            continue;
        }
        struct stat s;
        if(stat(array[j],&s)==0){
            if(S_ISDIR(s.st_mode)){
                printf(BLUE_COLOR"%s\n"DEFAULT_COLOR,array[j]);
            }
            else if(s.st_mode & S_IXUSR || s.st_mode & S_IXGRP || s.st_mode & S_IXOTH){
                printf(SUCCESS_COLOR"%s\n"DEFAULT_COLOR,array[j]);
            }
            else{
                printf("%s\n",array[j]);
            }
        }
        else{
            printf("%s\n",array[j]);
        }        
    }
}