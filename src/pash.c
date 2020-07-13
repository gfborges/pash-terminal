#define PROGRAM_NAME "pash"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define clear() printf("\033[H\033[J")

char * USR;
char cwd[PATH_MAX];
char _cwd[PATH_MAX] = "~";
char * PASH_PATH;
char * HOME;
char * NULL_ENVP[] = {(char*)  NULL};

char * get_envp(char * envp[], char * var){
    int var_len = strlen(var);
    for(int i = 0; envp[i] != NULL; i++) {
        if( strncmp(envp[i], var, var_len) == 0 ) {
            return envp[i]+ var_len+1;
        }
    }
    return "";
}

void _getcwd(){
    char home[PATH_MAX] = "/home/";
    strcat(home, USR);
    unsigned int home_len = strlen(home);
    getcwd(cwd, sizeof(cwd));
    strcat(cwd, "/");
    if( strncmp(cwd, home, home_len) == 0 ) {
        strcpy(_cwd, "~");
        char path[PATH_MAX];
        strcpy(path, &cwd[home_len]);
        strcat(_cwd, path);
    }
    else{
        strcpy(_cwd, cwd);
    }
    _cwd[ strlen(_cwd) - 1 ] = '\0';
}

char * getUserName(){
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if(pw){
        return pw->pw_name;
    }
    return "";
}

void print_header(){
    printf("\e[1m%s:", USR); //user name 
    _getcwd(); // prepare the current dir 
    printf("%s#\e[0m ", _cwd); // print current dir
}

void cd(char * cmd, char * par[]){
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    if(par[1] == NULL){
        par[1] = HOME;
    } else {
        strcat(cwd, "/");
        strcat(cwd, par[1]);
    }
    if( chdir(par[1]) < 0 ){
        if( chdir(cwd) < 0 ){
            perror("chdir");
        }
    }
}

void exec_input(char * cmd, char * par[], int n, char * envp[]){

    if( strcmp("exit", cmd) == 0 ) {
        exit(0);
    } else if( strcmp("cd", cmd) == 0 ) {
        cd(cmd, par);
        return;
    }

    pid_t pid = fork();
    if(pid < 0)  {
        // fork failed
        perror("fork");
        return;
    } 
    if( pid == 0 ){ // child process
        char _cmd[PATH_MAX + 100];
        char * pch = strtok(PASH_PATH, ":"); // search command at path
        // TODO try working dir first
        while(pch != NULL){
            strcpy(_cmd, pch);
            strcat(_cmd, "/");
            strcat(_cmd, cmd);
            execve(_cmd, par, envp); // executes input
            pch = strtok(NULL, ":");
        }
        perror("execve");
        exit(0); // make shure child is killed
    }
    if( pid > 0 ){ // parent process
        wait(0);
    }
}

int read_input(char * cmd, char * par[]){
    // TODO . for woking dir and ~ for $HOME
    char line[1000], *linetok[100], *pch;
    int parc = 0;
    print_header();
    fgets(line, 1000, stdin);
    pch = strtok(line, " \n");
    while(pch !=  NULL) {
        linetok[parc++] = strdup(pch);
        pch = strtok(NULL, " \n");
    }
    strcpy(cmd, linetok[0]);
    for(int i = 0; i < parc; i++) {
        par[i] = linetok[i];
    }
    par[parc] = (char*) NULL;
    free(pch);
    return parc;
}

int main(int argc, char * argv[], char * envp[]){
    USR = get_envp(envp, "USERNAME");
    PASH_PATH = get_envp(envp, "PASH_PATH");
    HOME = get_envp(envp, "HOME");
    exec_input("clear", argv, 1, envp);
    // infinite loop for input (exit inside read input)
    while(true){
        char cmd[100], *par[20];
        int n;
        n = read_input(cmd, par);
        exec_input(cmd, par, n, envp);
    }
    return 0;
}