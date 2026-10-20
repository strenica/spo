#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define VEL 256*16
extern int printf(const char *restrict fmt, ...);

void fileCheck(char *file){
	int fd = open(file, O_RDONLY);
	if ( fd < 0) {
		perror("Ne morem odpreti datoteke");
		exit(0);	
	}
	if ( close(fd) < 0)
		perror("Ne morem zapreti datoteke");
}

void endOfFile(int n, char *file){
    fileCheck(file);
    int fd = open(file, O_RDONLY);
    if (fd < 0) { perror("open"); exit(1); }

    long long size = lseek(fd, 0, SEEK_END);
    const int BLK = 4096;
    char buf[BLK];
    long long pos = size, off = 0;
    int found = 0;

    while (pos > 0){
        long long start = pos > BLK ? pos - BLK : 0;
        if (lseek(fd, start, SEEK_SET) < 0) { perror("lseek"); exit(1); }
        ssize_t got = read(fd, buf, pos - start);
        if (got <= 0) break;

        for (ssize_t i = got - 1; i >= 0; i--){
            if (buf[i] != '\n') continue;
            if (start + i == size - 1) continue;  
            if (++found == n){ off = start + i + 1; goto done; }
        }
        pos = start;
    }
done:
    lseek(fd, off, SEEK_SET);
    ssize_t r;
    while ((r = read(fd, buf, BLK)) > 0)
        if (write(STDOUT_FILENO, buf, r) != r) { perror("write"); exit(1); }
    close(fd);
}


int main(int argc, char *argv[])
{
    int n;
    n = 5;
    int option;
    char *file = NULL;

    while ((option = getopt(argc, argv, ":n:")) != -1){
        switch (option){
            case 'n':
                n = atoi(optarg);
                break;
            case ':':
                printf("Manjka argument za -n\n");
                return 0;
            default:
                printf("Neznana opcija\n");
                return 0;
        }
    }
    if (!file && optind < argc){
      file=argv[optind];
    }
    if (!file) {
        printf("Uporaba: [-n] datoteka\n");
        return 0;
    } 
    
    endOfFile(n, file);
}
