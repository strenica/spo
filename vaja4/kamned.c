#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096

static int read_line(char *buf, int size) {
    if (fgets(buf, size, stdin) == NULL) {
        return 0;
    }
    buf[strcspn(buf, "\n")] = '\0';
    return 1;
}

static int append(char *filename) {
    char line[MAXLINE];
    FILE *fptr = fopen(filename, "a");

    if (fptr == NULL) {
        perror("fopen");
        return 1;
    }
    while (read_line(line, MAXLINE)) {
        fputs(line, fptr);
        fputc('\n', fptr);
    }
    if (fclose(fptr) != 0) {
        perror("fclose");
        return 1;
    }
    return 0;
}

static int edit(char *filename, long n, int insert) {
    char tmpname[MAXLINE];
    char line[MAXLINE];
    char newline[MAXLINE];
    long number = 0;
    FILE *in, *tmp;

    if (insert && !read_line(newline, MAXLINE)) {
        fprintf(stderr, "Napaka: ni vrstice na stdin.\n");
        return 1;
    }

    in = fopen(filename, "r");
    if (in == NULL) {
        perror("fopen");
        return 1;
    }
    sprintf(tmpname, "%s.tmp", filename);
    tmp = fopen(tmpname, "w");
    if (tmp == NULL) {
        perror("fopen");
        fclose(in);
        return 1;
    }

    while (fgets(line, MAXLINE, in) != NULL) {
        number++;
        if (number == n) {
            if (insert) {
                fputs(newline, tmp);
                fputc('\n', tmp);
            } else {
                continue;
            }
        }
        fputs(line, tmp);
    }

    if (insert && number + 1 == n) {
        fputs(newline, tmp);
        fputc('\n', tmp);
    } else if (number + 1 < n) {
        fprintf(stderr, "Napaka: datoteka ima le %ld vrstic.\n", number);
        fclose(in);
        fclose(tmp);
        unlink(tmpname);
        return 1;
    }

    if (fclose(in) != 0 || fclose(tmp) != 0) {
        perror("fclose");
        unlink(tmpname);
        return 1;
    }
    if (unlink(filename) != 0) {
        perror("unlink");
        return 1;
    }
    if (rename(tmpname, filename) != 0) {
        perror("rename");
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    long n;

    if (argc == 3 && strcmp(argv[1], "-a") == 0) {
        return append(argv[2]);
    }
    if (argc == 4 && (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "-i") == 0)) {
        n = atol(argv[2]);
        if (n < 1) {
            fprintf(stderr, "Napaka: '%s' ni stevilka vrstice.\n", argv[2]);
            return 1;
        }
        return edit(argv[3], n, strcmp(argv[1], "-i") == 0);
    }

    fprintf(stderr, "Uporaba: %s -a <file> | -d <n> <file> | -i <n> <file>\n", argv[0]);
    return 1;
}
