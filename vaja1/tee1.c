#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>



void textCapture(FILE *out){    
    char buff[BUFSIZ];
    while (fgets(buff, sizeof(buff), stdin)){
        fputs(buff, stdout);
        fputs(buff, out);
        fflush(out);

    }
}

void fileHandle(char *file, int argument){
    int write = (argument != -1);
    FILE *fptr = fopen(file, write ? "w" : "a");
    if (fptr == NULL) {
        perror(write ? "Napaka pri kreiranju datoteke":"Napaka pri odpiranju/kreiranju datoteke");
        return;
    }
    textCapture(fptr);
    fclose(fptr);
}

int main(int argc, char *argv[])
{
    int option;
    char *file = NULL;

    while ((option = getopt(argc, argv, ":a:")) != -1){
        switch (option){
            case 'a':
                file = optarg;
                break;
            case ':':
                puts("Manjka argument za -a");
                return 0;
            case '?': 
            default:
                puts("Neznana opcija");
                return 0;
        }
    }
    if (!file) {
        puts("Uporaba: [-a] datoteka");
        return 0;
    }

    if (!file && optind < argc){
      file=argv[argc-1];
      option=0;
    } 

    fileHandle(file, option);
    return 0;
}
