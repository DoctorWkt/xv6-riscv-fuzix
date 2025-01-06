#include <unistd.h>

int brk(void *addr) {
  // Get the current brk value
  void *curbrk= sbrk(0);

  // Determine the difference
  // between addr and curbrk
  int diff= addr - curbrk;

  // Try to change brk by this amount
  if (sbrk(diff) == (void *)-1) {
    return(-1);
  }
  return(0);
}
