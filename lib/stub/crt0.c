#include <xv6/types.h>
#include <xv6/stat.h>
#include <xv6/fcntl.h>
#include <xv6/user.h>

extern void __stdio_init_vars();
extern int main(int argc, char *argv[]);

int errno;

// C startup code. This wraps main() so that
// it's OK if main() does not call _exit().

void start(int argc, char *argv[])
{
  __stdio_init_vars();		// Initialise stdio
  int result= main(argc, argv);
  _exit(result);
}
