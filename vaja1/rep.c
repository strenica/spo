#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define VELBLK 5//256*16
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
    fileCheck(file); //če obstaja file in je berlijv
    
    int fd = open(file, O_RDONLY);
    if (fd < 0) { perror("open"); exit(1); }

    long long size = lseek(fd, 0, SEEK_END);
    char buf[VELBLK];
    long long pos = size;
    long long zacetekIzpisa = 0;
    int found = 0;

    
    while (pos > 0){
        long long start;
        if (pos > VELBLK)
            start = pos - VELBLK; 
        else
            start = 0;   

        printf("size: %lli, pos:%lli, start:%lli, found:%lli\n",size,pos,start, found);

        printf("i: %i\n",lseek(fd, start, SEEK_SET));
        //if (lseek(fd, start, SEEK_SET) < 0) {perror("lseek"); exit(1); }
        ssize_t r_bytes = read(fd, buf, pos - start); //koliko bajtov preberem
        
        printf("r_bytes: %zd\n",r_bytes);
        if (r_bytes <= 0) break;

        ///
        printf("Beseda: ");
        for(int i = 0; i!=r_bytes ;i++){
            printf("%c",buf[i]);
            }
        printf("\n\n");
        /////

        puts("loop");
        for (ssize_t i = r_bytes - 1; i >= 0; i--){
            printf("zacetekIzpisa: %lli, r_bytes:%zd, i:%zd, start:%lli, found:%i, buf[i]:%c, size:%lli \n",zacetekIzpisa,r_bytes,i,start,found, buf[i],size);
            if (buf[i] != '\n') continue; 
            if (start + i == size - 1) continue;
            if (++found == n){ zacetekIzpisa = start + i + 1; goto done; }
        }
        
        pos = start;
        puts("---------------new loop-----------------");
    }
    done:
        printf("done: \nzacetekIzpisa: %lli\n",zacetekIzpisa);
        lseek(fd, zacetekIzpisa, SEEK_SET);
        ssize_t r;
        while ((r = read(fd, buf, VELBLK)) > 0)
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
            case '?':
                printf("? napaka\n");
                return 0;
            default:
                printf("napaka\n");
                return 0;
        }
    }
    if (!file && optind < argc){
      file=argv[optind];
    }
    if (!file) {
        printf("Uporaba: [-n st] datoteka\n");
        return 0;
    } 
    
    endOfFile(n, file);
}
