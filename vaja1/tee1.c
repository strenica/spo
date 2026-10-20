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

void fileHandle(char *file, int a){
    FILE *fptr = fopen(file, a ? "a" : "w");
    if (fptr == NULL) {
        perror(a ? "Napaka pri odpiranju/kreiranju datoteke":"Napaka pri kreiranju datoteke");
        return;
    }
    textCapture(fptr);
    fclose(fptr);
}

int main(int argc, char *argv[])
{
    int a = 0;
    int option;
    char *file = NULL;

    while ((option = getopt(argc, argv, ":a:")) != -1){
        switch (option){
            case 'a':
                a=1;
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

    if (!file && optind < argc){
      file=argv[optind];
      a=0;
    }
    if (!file) {
        puts("Uporaba: [-a] datoteka");
        return 0;
    } 

    fileHandle(file, a);
    return 0;
}
