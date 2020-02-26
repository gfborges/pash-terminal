#define PROGRAM_NAME "pash"
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#define clear() printf("\033[H\033[J")

const char* getUserName(){
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if(pw){
        return pw->pw_name;
    }
    return "";
}

bool read_input(const char * usr){
    printf("%s~# ", usr);
    scanf()
}

int main(){
   clear();
   const char  * usr = getUserName();
   while( read_input(usr *)  ){
       
   }
   getchar();
   return 0;
}
