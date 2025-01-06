#include <stdio.h>

int main(void) {
  FILE *zin;
  int cnt;
  char buf[512];
  printf("Hello world\n");

  // Open README
  zin= fopen("/README", "r");
  if (zin==NULL) {
    fprintf(stderr, "fopen failed\n");
    return(1);
  }

  // Print the contents of README
  while (1) {
    cnt= fread(buf, 1, 512, zin);
    if (cnt < 1) break;
    fwrite(buf, 1, cnt, stdout);
  }
  fclose(zin);
  
  return(0);
}
