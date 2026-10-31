#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static void izpisBitov(unsigned m, int n)  
{
    for (int i = n - 1; i >= 0; i--) {
        putchar(((m >> i) & 1) ? '1' : '0');
        if (i % 3 == 0 && i != 0)
            putchar(' ');                  
    }
}

static void izpisDovoljenj(mode_t m){
    char s[10];

    s[0] = (m & S_IRUSR) ? 'r' : '-';
    s[1] = (m & S_IWUSR) ? 'w' : '-';
    s[2] = (m & S_IXUSR) ? 'x' : '-';
    s[3] = (m & S_IRGRP) ? 'r' : '-';
    s[4] = (m & S_IWGRP) ? 'w' : '-';
    s[5] = (m & S_IXGRP) ? 'x' : '-';
    s[6] = (m & S_IROTH) ? 'r' : '-';
    s[7] = (m & S_IWOTH) ? 'w' : '-';
    s[8] = (m & S_IXOTH) ? 'x' : '-';
    s[9] = '\0';

    if (m & S_ISUID) s[2] = (m & S_IXUSR) ? 's' : 'S';
    if (m & S_ISGID) s[5] = (m & S_IXGRP) ? 's' : 'S';
    if (m & S_ISVTX) s[8] = (m & S_IXOTH) ? 't' : 'T';

    printf("Nova dovoljenja: %s\n", s);
}

int main(int argc, char *argv[]) {
    
  if (argc != 3) {
    printf("uporaba: %s dovoljenja datoteke\n", argv[0]);
    return 1;
  }

  char *dovoljenja = argv[1];
  char *pot = argv[2];
  int nov;
  
  if (dovoljenja[0] >= '0' && dovoljenja[0] <= '9') {
      //////
      long v = strtol(dovoljenja, NULL, 8);
      nov = v & 07777;
      printf("12 bitov: ");
      izpisBitov((unsigned)nov, 12);
      printf(", 8 bitov: ");
      izpisBitov((unsigned)nov, 8);
      printf("\n");
      ////////////
      
      //nov = strtol(dovoljenja, NULL, 8) & 07777;
  } else {
    struct stat st;
    if (stat(pot, &st) != 0) {
      perror(pot);
      return 1;
    }
    
    printf("NOV: %i\n",nov);
    nov = st.st_mode & 07777;
    printf("NOV: %i\n",nov);


    
    if (strcmp(dovoljenja, "u+s") == 0)
      nov |= S_ISUID;
    else if (strcmp(dovoljenja, "u-s") == 0)
      nov &= ~S_ISUID;
    else if (strcmp(dovoljenja, "g+s") == 0)
      nov |= S_ISGID;
    else if (strcmp(dovoljenja, "a+rX") == 0) {
      nov |= S_IRUSR | S_IRGRP | S_IROTH;
    if (S_ISDIR(st.st_mode))
        nov |= 0111;
    } else {
      fprintf(stderr, "neznano: %s\n", dovoljenja);
      return 1;
    }
  }

  if (chmod(pot, nov) != 0) {
    perror("chmod");
    return 1;
  }
  izpisDovoljenj(nov);
  return 0;
}
