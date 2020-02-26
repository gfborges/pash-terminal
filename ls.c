#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ls(char * dir_name){
    struct dirent ** dir_list;
    int n;
    n = scandir(dir_name, &dir_list, 0, alphasort);
    if( n < 0)
        perror("scandir");
    else{
        while(n-- > 2){
            printf("%s\n", dir_list[n]->d_name);
            free(dir_list[n]);
        }
        free(dir_list[1]);
        free(dir_list[0]);
        free(dir_list);
    }
}

int main(int argc, char * argv[]){
    if(argc == 1){
        ls(".");
        return 0;
    }
    ls(argv[1]);
    return 0;
}
