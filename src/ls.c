#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ls(char * dir_name, char * hidden){
    struct dirent ** dir_list;
    int n;
    n = scandir(dir_name, &dir_list, 0, alphasort);
    if( n < 0)
        ls(".",hidden);
    else{
        while(n-- > 2){
            if( strncmp(dir_list[n]->d_name,hidden,1) != 0 )
                printf("%s\n", dir_list[n]->d_name);
            free(dir_list[n]);
        }
        free(dir_list[1]);
        free(dir_list[0]);
        free(dir_list);
    }
}

int main(int argc, char * argv[]){
    char hidden[] = ".";
    int path = 1;
    for(int i =1; i < argc; i++){
        if( strcmp(argv[i], "-a") == 0 )
            hidden[0] = '\0';
        if( argv[i][0] != '-' )
            path = i;
    }
    ls(argv[path], hidden);
    return 0;
}
