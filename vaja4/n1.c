#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define VELIKOST_BUF 8000

int main(int argc, char *argv[]) {
  long B = 4096;
  long N = 5'000'000;

  struct timespec ts1, ts2;
  double cas;
  long i;
  FILE *fptr;
  char buf[VELIKOST_BUF];

  if (argc > 1) {
    B = atol(argv[1]);
  }
  if (argc > 2) {
    N = atol(argv[2]);
  }

  if (B < 1 || B > VELIKOST_BUF) {
    printf("Napaka: B mora biti med 1 in %d.\n", VELIKOST_BUF);
    return 1;
  }

  fptr = fopen("izhod.txt", "w");
  if (fptr == NULL) {
    perror("fopen");
    return 1;
  }

  if (setvbuf(fptr, buf, _IOFBF, B) != 0) {
    perror("setvbuf");
    fclose(fptr);
    return 1;
  }

  if (clock_gettime(CLOCK_MONOTONIC, &ts1) != 0) {
    perror("clock_gettime");
    fclose(fptr);
    return 1;
  }

  for (i = 0; i < N; i++) {
    if (fputs("Moja najljubsa vrstica.\n", fptr) == EOF) {
      perror("fputs");
      fclose(fptr);
      return 1;
    }
  }

  if (fflush(fptr) != 0) {
    perror("fflush");
    fclose(fptr);
    return 1;
  }

  if (clock_gettime(CLOCK_MONOTONIC, &ts2) != 0) {
    perror("clock_gettime");
    fclose(fptr);
    return 1;
  }

  if (fclose(fptr) != 0) {
    perror("fclose");
    return 1;
  }

  cas = (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec) / 1e9;

  printf("B = %ld bajtov\n", B);
  printf("N = %ld\n", N);
  printf("Cas zapisovanja = %.6f s\n", cas);

  return 0;
}
