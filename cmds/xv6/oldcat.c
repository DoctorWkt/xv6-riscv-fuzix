#include <xv6/types.h>
#include <xv6/fcntl.h>
#include <xv6/user.h>

char buf[512];

void
cat(int fd)
{
  int n;

  while((n = read(fd, buf, sizeof(buf))) > 0) {
    if (write(1, buf, n) != n) {
      fprintf(2, "cat: write error\n");
      _exit(1);
    }
  }
  if(n < 0){
    fprintf(2, "cat: read error\n");
    _exit(1);
  }
}

int
main(int argc, char *argv[])
{
  int fd, i;

  if(argc <= 1){
    cat(0);
    _exit(0);
  }

  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], O_RDONLY)) < 0){
      fprintf(2, "cat: cannot open %s\n", argv[i]);
      _exit(1);
    }
    cat(fd);
    close(fd);
  }
  _exit(0);
}
