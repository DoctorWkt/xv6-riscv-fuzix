#include <stdio.h>

// Test that NULL pointer dereferences cause an exception

volatile char *x= NULL;

int main() {
  char y;
  printf("About to deref a NULL pointer\n");
  fflush(stdout);
  y= *x;
  printf("Hmm, it seemed to work, y is %x :-S\n", y);
  *x = 23;
  y= *x;
  printf("Hmm, it seemed to work, y is %x :-S\n", y);
  return(0);
}
