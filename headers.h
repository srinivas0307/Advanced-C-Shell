#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<pwd.h>
#include<time.h>
#include<grp.h>
#include<dirent.h>
#include<sys/stat.h>
#include<wait.h>
#include<fcntl.h>
#include<signal.h>
#include<termios.h>
#include<ctype.h>
#include <netdb.h>

#define max_size 4039
#define SUCCESS_COLOR "\x1b[32m"
#define ERROR_COLOR "\x1b[31m"
#define DEFAULT_COLOR "\x1b[0m"
#define BLUE_COLOR "\x1b[34m"

typedef struct bgnode{
    int pid;
    char command[max_size];
    char status[max_size];
}bgnode;

typedef struct aliasnd{
    char name[max_size];
    char command[max_size];
}aliasnd;

int function(char * str,char *pre,char* his);
int check(char *str);
int background(char *str);
int reveal(char *flag,char *path);
int add(char * input,char* p);
int execute(char * command);
void history();
void purge();
int hop(char* str,char* pre,char *his);
int display_info(char * his);
int proclore(int a);
int seek(int flag1,int flag2,int flag3,char *path,char *name,char *pre);
int find(char* name,int flag1,int flag2,int flag3,char* rel);
int execute_command(char * str,char *pre,char * his);
int foreground(char *arg[max_size]);
void activities();
int neonate(int n);
int ping(int pid,int sig);
void ctrlc(int signum);
int bg(int pid);
int iman(char * command);
void checkforalias(char *str);
int load();
int f_g(int pid);