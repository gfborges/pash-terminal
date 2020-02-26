#define PROGRAM_NAME "pash"
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <string.h>
#define clear() printf("\033[H\033[J")

const char * getUserName(){
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if(pw){
        return pw->pw_name;
    }
    return "";
}

bool read_input(const char * usr, char * cmd){
    printf("%s~# ", usr);
    scanf("%s", cmd);
    if( strcmp("exit", cmd) == 0 )
        return false;
    return true;
}

void exec_input(const char * usr, char * cmd){
    printf("\t%s", cmd);
    putchar('\n');
}

int main(){
   const char * usr = getUserName();
   char  cmd[100];
   clear();
   while( read_input(usr, cmd)  ){
       exec_input(usr, cmd);
   }
   getchar();
   return 0;
}
