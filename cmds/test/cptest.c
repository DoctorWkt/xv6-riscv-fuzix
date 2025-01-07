// Some stdio tests

extern void printchar(int);
extern void printint(int);
void cprintf(char *fmt, ...);

#include <stdio.h>
#include <unistd.h>

char buf[512];

int main(int argc, char *argv[]) {
  FILE *zin, *zout;
  size_t cnt;
  int err;

  if (argc != 3) {
    cprintf("Usage: cptest infile outfile\n"); exit(1);
  }

  zin= fopen(argv[1], "r");
  if (zin == NULL) { cprintf("fopen() on %s failed\n", argv[1]); exit(1); }
  zout= fopen(argv[2], "w");
  if (zout == NULL) { cprintf("fopen() on %s failed\n", argv[2]); exit(1); }

  while (1) {
    cnt= fread(buf, 1, 512, zin);
    if (cnt == -1 ) {
      cprintf("fread() failed\n"); exit(1);
    }
    if (cnt==0) break;
    fwrite(buf, 1, cnt, zout);
  }

  err= fclose(zin);
  if (err == -1) { cprintf("fclose() failed\n"); exit(1); }
  err= fclose(zout);
  if (err == -1) { cprintf("fclose() failed\n"); exit(1); }
  exit(0);
}
