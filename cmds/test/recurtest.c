#include <stdio.h>

// A test for infinite recursion

void recurse(int x) {
  printf("%d at 0x%p\n", x, &x);
  recurse(x+1);
}

int main() {
  recurse(1);
  return(0);
}
