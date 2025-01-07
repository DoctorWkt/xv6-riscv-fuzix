#include <stdio.h>
#include <errno.h>
#include <string.h>

// perror test

int main() {
  errno= 0;
  printf("errno %d strerror() %s\n", errno, strerror(errno));
  perror("No error");

  errno= EINVAL;
  printf("errno %d strerror() %s\n", errno, strerror(errno));
  errno= EINVAL; perror("Invalid");

  errno= EPERM;
  printf("errno %d strerror() %s\n", errno, strerror(errno));
  errno= EPERM; perror("Permission");
  return(0);
}
