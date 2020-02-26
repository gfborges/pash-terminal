#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

int main(int argc, char * argv[]){
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    printf("%s\n", cwd);
    return 0;
}
