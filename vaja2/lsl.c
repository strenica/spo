#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
    int option;
    char *l;
    while ((option = getopt(argc, argv, ":l:")) != -1){
        
        switch (option){
            case 'l':
                l = optarg;
                break;
            case ':':
                printf("Manjka argument za -l\n");
                return 0;
            case '?':
                printf("? napaka\n");
                return 0;
            default:
                printf("napaka\n");
                return 0;
        }

    }
    printf("\n%s\n",l);
}
