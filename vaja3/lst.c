#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>

typedef struct { char path[4096]; time_t mt; } Ent;


static int cmp(const void *a, const void *b) {
    time_t x = ((Ent*)a)->mt, y = ((Ent*)b)->mt;
    if (x > y) return -1;
    if (x < y) return  1;
    return 0;
}


int main(int argc, char* argv[]) {
    DIR *dir;
    bool flag_r = 0;
    const char *pot = ".";

    if(argc>=2){
        const char *pot = argv[1];
        if(!strcmp(pot,"-r")){
            flag_r = 1;
        }else{
            const char *pot = argv[1];
        }
        if(argc>=3){
            const char *pot = argv[2];
        }
    }
    dir = opendir(pot);

    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    struct dirent *entry;
    struct stat buf;
    char F_pot[4096];

    Ent arr[1024];
    size_t n = 0;


    while ((entry = readdir(dir)) != NULL) {

        snprintf(arr[n].path, sizeof arr[n].path, "%s/%s", pot, entry->d_name);
        lstat(arr[n].path, &buf);
        arr[n].mt = buf.st_mtime;
        n++;

        //printf("Ime: %s\n", entry->d_name);
    }
    qsort(arr, n, sizeof arr[0], cmp);
    for (size_t i = 0; i < n; i++)
        printf("%ld\t%s\n", (long)arr[i].mt, arr[i].path);

    closedir(dir);
    return 0;
}
