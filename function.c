#include "headers.h"

// int function(char *str, char *pre, char *his) {
//     char backup[max_size];
//     strcpy(backup, str);
//     char command[1029][max_size];
//     int c = 0;
//     char *comd = strtok(backup, "|");
//     while (comd != NULL) {
//         strcpy(command[c], comd);
//         c++;
//         comd = strtok(NULL, "|");
//     }
//     int fd[c - 1][2];
//     int fd_in = 0;
//     for (int i = 0; i < c - 1&&c>1; i++) {
//         if (pipe(fd[i]) == -1) {
//             perror("Pipe failed");
//             return 0;
//         }
//     }
//     int std_in = dup(STDIN_FILENO);
//     int std_out = dup(STDOUT_FILENO);
//     for (int i = 0; i < c; i++) {
//         char *temp1 = strstr(command[i], "<");
//         char *temp = strstr(command[i], ">");
//         if (i == 0&&temp1!=NULL) {
//             char dummy[max_size];
//             strcpy(dummy, temp1);
//             char *file = strtok(dummy + 1, " ");
//             int f = open(file, O_RDONLY);
//             if (f < 0) {
//                 perror("No such input file found!");
//                 return 0;
//             }
//             dup2(f, STDIN_FILENO);
//             close(f);
//             *temp1 = '\0';
//         }
//         if (i==c-1&&temp!=NULL) {
//             *temp = '\0';
//             temp++;
//             int f;
//             if (temp[0] == '>') {
//                 while(temp[0]==' '||temp[0]=='>'){
//                     temp++;
//                 }
//                 f = open(temp, O_WRONLY | O_CREAT | O_APPEND, 0644);
//             } else {
//                 while(temp[0]==' '){
//                     temp++;
//                 }
//                 f = open(temp, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//             }
//             if (f < 0) {
//                 perror("Failed to open output file");
//                 return 0;
//             }
//             dup2(f, STDOUT_FILENO);
//             close(f);
//             *temp = '\0';
//         }
//         if(c==1){
//             execute_command(command[i],pre,his);
//             dup2(std_in, STDIN_FILENO);
//             dup2(std_out, STDOUT_FILENO);
//             close(std_in);
//             close(std_out);
//             return 0;
            
//         }
//         if (i != 0) {
//             dup2(fd[i - 1][0], STDIN_FILENO);
//             close(fd[i - 1][0]);
//         }
//         if (i != c - 1) {
//             dup2(fd[i][1], STDOUT_FILENO);
//             close(fd[i][1]);
//         }
//         execute_command(command[i], pre, his);
//         dup2(std_in, STDIN_FILENO);
//         dup2(std_out, STDOUT_FILENO);
//         if (i != 0) {
//             close(fd[i - 1][1]);
//         }
//     }
//     dup2(std_in, STDIN_FILENO);
//     dup2(std_out, STDOUT_FILENO);
//     close(std_in);
//     close(std_out);
//     return 0;
// }

int function(char *str, char *pre, char *his) {
    int n=0;
    if(strcmp(str,"")==0||strcmp(str," ")==0||str==NULL||strcmp(str,"\n")==0){
        return 0;
    }
    char backup[max_size];
    strcpy(backup, str);
    char command[1029][max_size];
    if(strstr(backup,"|")==NULL&&strstr(backup,"<")==NULL&&strstr(backup,">")==NULL){
        return execute_command(backup,pre,his);
    }
    int c=0;
    char *comd=strtok(backup, "|");
    while(comd!=NULL){
        strcpy(command[c], comd);
        c++;
        comd=strtok(NULL,"|");
    }
    int fd[c-1][2];
    for(int i=0;i<c-1&&c>1;i++){
        if (pipe(fd[i]) == -1) {
            perror("Pipe failed");
            return 0;
        }
    }
    for(int i=0;i<c;i++){
        pid_t pid = fork();
        if(pid<0){
            perror("Fork failed");
            return 0;
        }
        else if(pid==0){
            char *temp1=strstr(command[i],"<");
            char *temp=strstr(command[i],">");
            if(i==0){
                if(temp1!=NULL){
                    char dummy[max_size];
                    strcpy(dummy, temp1);
                    char *file=strtok(dummy + 1, " ");
                    int f=open(file, O_RDONLY);
                    if(f<0){
                        perror("No such input file found!");
                        exit(EXIT_FAILURE);
                    }
                    dup2(f, STDIN_FILENO);
                    close(f);
                    *temp1 = '\0';
                }
            }
            if(i==c-1){
                if(temp!=NULL) {
                    *temp = '\0';
                    temp++;
                    int f;
                    if(temp[0]=='>'){
                        while(temp[0]==' '||temp[0]=='>'){
                            temp++;
                        }
                        f=open(temp,O_WRONLY|O_CREAT|O_APPEND, 0644);
                    } 
                    else{
                        while(temp[0]==' ') {
                            temp++;
                        }
                        f=open(temp,O_WRONLY|O_CREAT|O_TRUNC,0644);
                    }
                    if(f<0){
                        perror("Failed to open output file");
                        exit(EXIT_FAILURE);
                    }
                    dup2(f,STDOUT_FILENO);
                    close(f);
                    *temp = '\0';
                }
            }
            if(c>1){
                if(i!=0){ 
                    dup2(fd[i - 1][0], STDIN_FILENO);
                }
                if(i!=c-1){
                    dup2(fd[i][1], STDOUT_FILENO);
                }
            }
            for(int j=0;j<c-1;j++) {
                close(fd[j][0]);
                close(fd[j][1]);
            }
            execute_command(command[i], pre, his);
            exit(0); 
        }
        else{ 
            if(i!=0){
                close(fd[i - 1][0]);
                close(fd[i - 1][1]);
            }
        }
    }
    for(int i=0;i<c-1;i++){
        close(fd[i][0]);
        close(fd[i][1]);
    }
    for(int i=0;i<c;i++) {
        wait(NULL);
    }
    return 0;
}


int execute_command(char * str,char *pre,char * his){
    printf("%s2\n",str);
    if(str==NULL){
        return 0;
    }
    char backup[max_size];
    strcpy(backup,str);
    int c=check(backup);
    char *ptr;
    char * test=strtok_r(str,"&",&ptr);
    for(int i=0;i<c;i++){
        char temp2[max_size];
        strcpy(temp2,test);
        background(temp2);
        test=strtok_r(NULL,"&",&ptr);
    }
    if(test==NULL||strcmp(test,"")==0||strcmp(test," ")==0){
        return 0;
    }
    // printf("%s\n",test);
    char *first=strtok(test," ");
    // printf("%s\n",first);
    if(strcmp(first,"hop")==0){
        first=strtok(NULL," ");
        while(first!=NULL){
            hop(first,pre,his);
            first=strtok(NULL," ");
        }
    }
    else if(strcmp(first,"reveal")==0){
        char* flag=strtok(NULL," ");
        if(flag==NULL){
            printf("%s1\n",flag);
            reveal("NULL",".");
        }
        else{
            if(strncmp(flag,"-a",2)==0||strncmp(flag,"-l",2)==0){
                char* dummy=strtok(NULL," ");
                if(dummy==NULL){
                    reveal(flag,".");
                }
                else{
                    if(strncmp(dummy,"-a",2)==0||strncmp(dummy,"-l",2)==0){
                        strcat(flag,dummy);
                        dummy=strtok(NULL," ");
                        // printf("dummy=%s\n",dummy);
                        if(dummy==NULL){
                            reveal(flag,".");
                        }
                        else{
                            reveal(flag,dummy);
                        }
                    }
                    else if(dummy==NULL){
                        reveal(flag,".");
                    }
                    else{
                        reveal(flag,dummy);
                    }
                }
            }
            else{
                reveal("NULL",flag);
            }
        }
    }
    else if(strcmp(first,"log")==0){
        char *dummy=strtok(NULL," ");
        if(dummy==NULL){
            history();
        }
        else{
            if(strcmp(dummy,"execute")==0){
                dummy=strtok(NULL," ");
                if(dummy==NULL){
                    printf(ERROR_COLOR"Incomplete command\n"DEFAULT_COLOR);
                }
                else{         
                    execute(dummy);
                    dummy[strcspn(dummy, "\n")] = '\0';
                    char *temp1=strtok(dummy,";");
                    while(temp1!=NULL){
                        char f[max_size];
                        strcpy(f,temp1);
                        execute_command(f,pre,his);
                        temp1=strtok(NULL,";");
                    }
                }
            }
            else if(strcmp(dummy,"purge")==0){
                purge();
            }
        }
    }
    else if(strcmp(first,"proclore")==0){
        char *dummy=strtok(NULL," ");
        if(dummy==NULL){
            proclore(getpid());
        }
        else{
            proclore(atoi(dummy));
        }
    }
    else if(strcmp(first,"seek")==0){
        int flag1=0;
        int flag2=0;
        int flag3=0;
        char *flag=strtok(NULL," ");
        while(flag!=NULL){
            if(strcmp(flag,"-d")==0){
                flag1=1;
            }
            else if(strcmp(flag,"-f")==0){
                flag2=1;
            }
            else if(strcmp(flag,"-e")==0){
                flag3=1;
            }
            else {
                break;
            }
            flag=strtok(NULL," ");
        }
        char *path=strtok(NULL," ");
        if(path==NULL){
            seek(flag1,flag2,flag3,".",flag,pre);
        }
        else{
            seek(flag1,flag2,flag3,path,flag,pre);
        }
    }
    else if(strcmp(first,"activities")==0){
        activities();
    }
    else if(strcmp(first,"neonate")==0){
        char *dummy=strtok(NULL," ");
        dummy=strtok(NULL," ");
        neonate(atoi(dummy));
    }
    else if(strcmp(first,"ping")==0){
        char *dummy=strtok(NULL," ");
        char *dummy1=strtok(NULL," ");
        ping(atoi(dummy),atoi(dummy1));
    }
    else if(strcmp(first,"bg")==0){
        char *dummy=strtok(NULL," ");
        bg(atoi(dummy));
    }
    else if(strcmp(first,"exit")==0){
        exit(0);
    }
    else if(strcmp(first,"iman")==0){
        char *dummy=strtok(NULL," ");
        iman(dummy);
    }
    else if(strcmp(first,"fg")==0){
        char *dummy=strtok(NULL," ");
        f_g(atoi(dummy));
    }
    else{
        char *arg[max_size];
        arg[0]=first;
        int e=0;
        int i=1;
        while(first!=NULL){
            first=strtok(NULL," ");
            char *t=first;
            arg[i]=first;
            i++;
        }
        
        arg[i]=NULL;
        return foreground(arg);
    }
    return 0;
}