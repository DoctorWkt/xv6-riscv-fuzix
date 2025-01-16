// Print the time in seconds

#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

void cprintf(char *, ...);

int main(int argc, char *argv[]) {
  time_t tim;
  cprintf("Current time in seconds: %ld\n", time(NULL));
  time(&tim);
  cprintf("Current time using pointer: %ld\n", tim);
  return(0);
}
