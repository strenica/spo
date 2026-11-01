#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc, char* argv[]) {
    if(argc>=2)
        printf("%i\n",argc);
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return EXIT_FAILURE;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("Ime: %s\n", entry->d_name);
    }

    closedir(dir);
    return EXIT_SUCCESS;
}
