#include <xv6/types.h>
#include <xv6/stat.h>
#include <xv6/user.h>

int
main(int argc, char **argv)
{
  int i;

  if(argc < 2){
    fprintf(2, "usage: kill pid...\n");
    _exit(1);
  }
  for(i=1; i<argc; i++)
    kill(atoi(argv[i]));
  _exit(0);
}
