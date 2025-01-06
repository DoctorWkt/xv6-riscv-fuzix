#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

extern int errno;
void cprintf(char *fmt, ...);

int main() {
  int err= open("/foobar", O_RDONLY);
  if (err == -1)
    cprintf("open failed errno %d\n", errno);

  exit(0);
}
