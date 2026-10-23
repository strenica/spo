#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h> 
#include <stdlib.h>

void pregled(char *l){
    struct stat buf;

    if (stat(l, &buf) < 0){
        printf("%s, ne obstaja\n",l);
		exit(2);
    }
    char type;
	if ( S_ISREG(buf.st_mode) )
		type = '-';	
	else if ( S_ISDIR(buf.st_mode) )
		type = 'd';	
	else if ( S_ISLNK(buf.st_mode) )
		type = 'l';	
	else 
		type = '?';

}


int main(int argc, char *argv[]) {
  int option;
  char *l;
  while ((option = getopt(argc, argv, ":l:")) != -1) {

    switch (option) {
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
  pregled(l);
}
