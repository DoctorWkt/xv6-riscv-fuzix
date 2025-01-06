#include <unistd.h>
#include <fcntl.h>

void cprintf(char *fmt, ...);

int main() {
  int fd;
  int res;

  fd= open("/README", O_RDONLY);

  res= isatty(0);
  cprintf("fd %d isatty %d\n", 0, res);
  res= isatty(fd);
  cprintf("fd %d isatty %d\n", fd, res);
  return(0);
}
