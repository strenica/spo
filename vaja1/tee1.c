#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

void textCapture(int fd){
    char buff[BUFSIZ];
    ssize_t n;
    while ((n = read(STDIN_FILENO, buff, sizeof buff)) > 0){
        write(STDOUT_FILENO, buff, n);
        if (fd >= 0)
            write(fd, buff, n);
    }
}

void fileHandle(char *file, int append){
    int flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);
    int fd = open(file, flags, 0644);
    if (fd < 0) {
        perror(append ? "Napaka pri odpiranju/kreiranju datoteke":"Napaka pri kreiranju datoteke");
        return;
    }
    textCapture(fd);
    close(fd);
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
