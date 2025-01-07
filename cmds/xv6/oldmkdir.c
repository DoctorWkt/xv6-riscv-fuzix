#include <xv6/types.h>
#include <xv6/stat.h>
#include <xv6/user.h>

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    fprintf(2, "Usage: mkdir files...\n");
    _exit(1);
  }

  for(i = 1; i < argc; i++){
    if(mkdir(argv[i]) < 0){
      fprintf(2, "mkdir: %s failed to create\n", argv[i]);
      break;
    }
  }

  _exit(0);
}
