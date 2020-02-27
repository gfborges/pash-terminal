#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

char * get_envp(char * envp[], char * var){
    int var_len = strlen(var);
    for(int i = 0; envp[i] != NULL; i++){
        if( strncmp(envp[i], var, var_len) == 0 ){
            return envp[i] + var_len + 1;
        }
    }
    return "";
}

int main(int argc, char * argv[], char * envp[]){
    char cmd[100]; 
    char * PATH = get_envp(envp, "PATH"), * pch;
    // printf(">>> %s\n", PATH);
    pch = strtok(PATH, ":");
    while(pch != NULL){
        strcpy(cmd, pch);
        strcat(cmd, "/");
        strcat(cmd, argv[1]);
        pid_t pid = fork();
        if(pid == 0){
            // printf(">>> %s\n",cmd);
            execve(cmd, &argv[1], envp);
            exit(0);
        }
        else{
            wait(0);
            pch = strtok(NULL, ":");
        }
    }
    return 0;
}