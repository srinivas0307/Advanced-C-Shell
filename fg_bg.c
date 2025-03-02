#include "headers.h"

extern bgnode bgarray[max_size];
extern int bgcount;
extern int fp;

int bg(int pid){
    if(kill(pid,0)!=0){
        printf(ERROR_COLOR"Process not found\n"DEFAULT_COLOR);
        return 0;
    }
    if(kill(pid,SIGCONT)==-1){
        perror("Error");
        return 0;
    }
    return 1;
}

int f_g(int pid) {
    if (kill(pid, 0) != 0) {
        perror(ERROR_COLOR "Process not found" DEFAULT_COLOR);
        return 0;
    }

    char cmd[max_size] = "unknown system background process";
    for (int i = 0; i < bgcount; i++) {
        if (bgarray[i].pid == pid) {
            strncpy(cmd, bgarray[i].command, max_size - 1);
            cmd[max_size - 1] = '\0'; 

            for (int j = i; j < bgcount - 1; j++) {
                bgarray[j] = bgarray[j + 1];
            }
            bgcount--; 
            break;
        }
    }
    int pgrp = getpgid(pid);

    if (kill(pid, SIGCONT) == -1) {
        perror("Error");
        return 0;
    }

    int status;
    // if(tcsetpgrp(STDIN_FILENO, pid) == -1) {
    //     perror("tcsetpgrp");
    //     return 0;
    // }
    // // tcsetpgrp(STDIN_FILENO, pid);
    // signal(SIGTTIN, SIG_IGN);
    // signal(SIGTTOU,SIG_IGN);

    while (1) {
        int ret_pid = waitpid(pid, &status, WNOHANG|WUNTRACED);
        if (ret_pid > 0) {
            if (WIFEXITED(status)) {
                printf(ERROR_COLOR"\n\t%d\texited normally\n"DEFAULT_COLOR, pid);
                break;
            } else if (WIFSTOPPED(status)) {
                printf(ERROR_COLOR"\n\t%d\tstopped\n"DEFAULT_COLOR, pid);
                bgarray[bgcount].pid = pid;
                strncpy(bgarray[bgcount].command, cmd, max_size - 1);
                strncpy(bgarray[bgcount].status, "STOPPED", max_size - 1);
                bgcount++;
                break;
            }
        }
        else if (ret_pid == 0) {
            continue;
        }
        else {
            // perror("waitpid");
            return 0;
        }
    }

    fp = 0;
    // if(tcsetpgrp(STDIN_FILENO, pgrp) == -1) {
    //     perror("tcsetpgrp");
    //     return 0;
    // }
    // // tcsetpgrp(STDIN_FILENO, pgrp);
    // signal(SIGTTIN, SIG_IGN);
    // signal(SIGTTOU,SIG_IGN);
    return 0;
}

