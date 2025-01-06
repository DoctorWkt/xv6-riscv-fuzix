#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

void cprintf(char *fmt, ...);

int main() {
  int status, exitvalue;
  pid_t wpid;
  pid_t pid = fork();

  switch (pid) {
  case -1:
    cprintf("fork error\n");
    exit(1);

  case 0:
    sleep(3);
    cprintf("In child, slept 3 seconds, now exit(5)\n");
    exit(5);

  default:
    cprintf("In parent, waiting for %d\n", pid);
    wpid = wait(&status);
    cprintf("Got back pid %d status %d\n", wpid, status);
    if (WIFEXITED(status)) {
      exitvalue = WEXITSTATUS(status);
      cprintf("exit value was %d\n", exitvalue);
    }
  }
  exit(0);
}
