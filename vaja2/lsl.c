#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

static const char *to_str(struct stat *buf) {
  static char s[11];
  mode_t m = buf->st_mode;

  s[0] = '-';
  if (S_ISDIR(m))
    s[0] = 'd';
  else if (S_ISLNK(m))
    s[0] = 'l';
  else if (S_ISCHR(m))
    s[0] = 'c';
  else if (S_ISBLK(m))
    s[0] = 'b';
  else if (S_ISFIFO(m))
    s[0] = 'p';
  else if (S_ISSOCK(m))
    s[0] = 's';

  s[1] = (m & S_IRUSR) ? 'r' : '-';
  s[2] = (m & S_IWUSR) ? 'w' : '-';
  s[3] = (m & S_IXUSR) ? 'x' : '-';
  s[4] = (m & S_IRGRP) ? 'r' : '-';
  s[5] = (m & S_IWGRP) ? 'w' : '-';
  s[6] = (m & S_IXGRP) ? 'x' : '-';
  s[7] = (m & S_IROTH) ? 'r' : '-';
  s[8] = (m & S_IWOTH) ? 'w' : '-';
  s[9] = (m & S_IXOTH) ? 'x' : '-';

  if (m & S_ISUID)
    s[3] = (m & S_IXUSR) ? 's' : 'S';
  if (m & S_ISGID)
    s[6] = (m & S_IXGRP) ? 's' : 'S';
  if (m & S_ISVTX)
    s[9] = (m & S_IXOTH) ? 't' : 'T';

  s[10] = '\0';
  return s;
}

void pregled(const char *l) {
  struct stat buf;

  if (lstat(l, &buf) < 0) {
    fprintf(stderr, "%s, ne obstaja\n", l);
    return;
  }

  struct passwd *pw = getpwuid(buf.st_uid);
  char uid[32];
  const char *lastnik;

  if (pw != NULL) {
    lastnik = pw->pw_name;
  } else {
    snprintf(uid, sizeof uid, "%lu", (unsigned long)buf.st_uid);
    lastnik = uid;
  }

  printf("%s %lu %s %lld %s\n", to_str(&buf), (unsigned long)buf.st_nlink, lastnik, (long long)buf.st_size, l);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "uporaba: %s datoteka...\n", argv[0]);
    return 1;
  }

  for (int i = 1; i < argc; i++)
    pregled(argv[i]);

  return 0;
}
