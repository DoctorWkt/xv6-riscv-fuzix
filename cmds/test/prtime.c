// Print the time in seconds

#include <xv6/types.h>
#include <xv6/stat.h>
#include <xv6/fcntl.h>
#include <xv6/user.h>

void cprintf(char *, ...);

int main(int argc, char *argv[]) {
  cprintf("Current time in seconds: %ld\n", time());
  return(0);
}
