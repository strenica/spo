#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>


int main() {
    char wd[1024][256];
    ino_t wdIno[1024];
    DIR *dir = opendir(".");
    struct dirent *entry;
    int n = 0;

    loop:
        while ((entry = readdir(dir)) != NULL) {
            if(!strcmp(entry->d_name,".")){
                wdIno[n]=entry->d_ino;
            }
            struct stat st;
            if(n>0 && strcmp(entry->d_name,".") && strcmp(entry->d_name,"..") && stat(entry->d_name,&st)==0 && st.st_ino == wdIno[n-1]){
                snprintf(wd[n], sizeof wd[n], "%s", entry->d_name);
            }
        }

    chdir(".."); closedir(dir); dir = opendir(".");
    if(wdIno[n] == wdIno[n-1]){
        goto stop;
    }
    n++; goto loop;

    stop:
    for(;n!=0;n--){
        printf("%s/",wd[n]);
    }
    printf("\n");
}
