#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096

int read_line(char *line) {
  if (fgets(line, MAXLINE, stdin) == NULL) {
    return 0;
  }
  line[strcspn(line, "\n")] = '\0';
  return 1;
}

int append(char *fileName) {
  char line[MAXLINE];
  FILE *f;

  f = fopen(fileName, "a");
      if (f == NULL) {
        perror("fopen");
        return 1;
      }

  while (read_line(line)) {
    fputs(line, f);
    fputc('\n', f);
  }

  if (ferror(f)) {
    perror("fputs");
    fclose(f);
    return 1;
  }

  if (fclose(f) != 0) {
    perror("fclose");
    return 1;
  }
  return 0;
}

int rewrite(char *fileName, long n, char *newline) {
  char line[MAXLINE];
  char tmpname[MAXLINE];
  FILE *in, *tmp;
  long number = 0;
  long max;

  in = fopen(fileName, "r");
      if (in == NULL) {
        perror("fopen");
        return 1;
      }

  snprintf(tmpname, sizeof(tmpname), "%s.tmp", fileName);
  tmp = fopen(tmpname, "w");
      if (tmp == NULL) {
        perror("fopen");
        fclose(in);
        return 1;
      }

  while (fgets(line, MAXLINE, in) != NULL) {
    number++;
    if (number == n) {
      if (newline == NULL) {
        continue;
      }
      fputs(newline, tmp);
      fputc('\n', tmp);
    }
    fputs(line, tmp);
  }

  max = number;
  if (newline != NULL) {
    max = number + 1;
  }

  if (n > max) {
    printf("Napaka: datoteka ima %ld vrstic.\n", number);
    fclose(in);
    fclose(tmp);
    unlink(tmpname);
    return 1;
  }

  if (newline != NULL && n == number + 1) {
    fputs(newline, tmp);
    fputc('\n', tmp);
  }

  if (fclose(in) != 0 || fclose(tmp) != 0) {
    perror("fclose");
    unlink(tmpname);
    return 1;
  }
  if (unlink(fileName) != 0) {
    perror("unlink");
    unlink(tmpname);
    return 1;
  }
  if (rename(tmpname, fileName) != 0) {
    perror("rename");
    unlink(tmpname);
    return 1;
  }
  return 0;
}

int main(int argc, char *argv[]) {
  char newline[MAXLINE];
  long n;

  if (argc == 3 && strcmp(argv[1], "-a") == 0) {
    return append(argv[2]);
  }

  if (argc == 4 && strcmp(argv[1], "-d") == 0) {
    n = atol(argv[2]);
    if (n < 1) {
      printf("Napaka: '%s' ni stevilka vrstice.\n", argv[2]);
      return 1;
    }
    return rewrite(argv[3], n, NULL);
  }

  if (argc == 4 && strcmp(argv[1], "-i") == 0) {
    n = atol(argv[2]);
    if (n < 1) {
      printf("Napaka: '%s' ni stevilka vrstice.\n", argv[2]);
      return 1;
    }
    if (!read_line(newline)) {
      printf("Napaka: ni vrstice na stdin.\n");
      return 1;
    }
    return rewrite(argv[3], n, newline);
  }

  printf("Uporaba: %s -a <file> | -d <n> <file> | -i <n> <file>\n", argv[0]);
  return 1;
}
