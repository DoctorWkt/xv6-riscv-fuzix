#include <unistd.h>
#include <fcntl.h>

void cprintf(char *fmt, ...);

char buf[16];

int main() {
  int fd,err;

  fd= open("/README", O_RDONLY);
  if (fd==-1) {
    cprintf("Cannot open /README\n"); return(1);
  }

  err= lseek(fd, 50, SEEK_SET);
  if (err==-1) {
    cprintf("Cannot lseek(fd, 50, SEEK_SET)\n"); return(1);
  }
  read(fd, buf, 16);
  write(1, buf, 16);

  err= (int)lseek(fd, 16, SEEK_CUR);
  if (err==-1) {
    cprintf("Cannot lseek(fd, 16, SEEK_CUR)\n"); return(1);
  }
  read(fd, buf, 16);
  write(1, buf, 16);

  err= (int)lseek(fd, -16, SEEK_END);
  if (err==-1) {
    cprintf("Cannot lseek(fd, -16, SEEK_END)\n"); return(1);
  }
  read(fd, buf, 16);
  write(1, buf, 16);

  close(fd);

  return(0);
}
