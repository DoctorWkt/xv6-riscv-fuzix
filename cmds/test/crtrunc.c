#include <unistd.h>
#include <fcntl.h>
void cprintf(char *fmt, ...);

int main() {
  int fd;

  fd= open("/fred", O_CREAT | O_APPEND);
  cprintf("fd is %d\n", fd);
  write(fd, "Hello\n", 6);
  close(fd);
  
  return(0);
}
