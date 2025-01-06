#include <xv6/types.h>
#include <xv6/stat.h>
#include <xv6/fcntl.h>
#include <xv6/user.h>

int errno;

// C startup code. This wraps main() so that
// it's OK if main() does not call _exit().

void start()
{
  extern int main();
  int result= main();
  _exit(result);
}
