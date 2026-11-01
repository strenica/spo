#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

typedef struct { char path[4096]; time_t mt; } Ent;

bool flag_r = 0;
static int cmp(const void *a, const void *b) {
    time_t x = ((Ent*)a)->mt
    time_t y = ((Ent*)b)->mt;
    if(!flag_r){
    if (x > y) return -1;
    if (x < y) return  1;
    }else{
    if (x < y) return -1;
    if (x > y) return  1;
    }
    return 0;
}


int main(int argc, char* argv[]) {
    DIR *dir;
    char *pot = ".";

    if(argc>=2){
        if(!strcmp(argv[1],"-r")){
            flag_r = 1;
        }else{
            pot = argv[1];
        }
        if(argc>=3){
            pot = argv[2];
        }
    }

    dir = opendir(pot);
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    struct dirent *entry;
    struct stat buf;

    Ent arr[1024];
    size_t n = 0;


    while ((entry = readdir(dir)) != NULL) {
        snprintf(arr[n].path, sizeof arr[n].path,
                 (pot[strlen(pot)-1] == '/') ? "%s%s" : "%s/%s",
                 pot, entry->d_name);
        lstat(arr[n].path, &buf);
        arr[n].mt = buf.st_mtime;
        n++;
    }

    qsort(arr, n, sizeof arr[0], cmp);
    for (size_t i = 0; i < n; i++)
        printf("%ld\t%s\n", (long)arr[i].mt, arr[i].path);

    closedir(dir);
    return 0;
}
