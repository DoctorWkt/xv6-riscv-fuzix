#include <stdio.h>
#include <unistd.h>

// brk and sbrk tests

//       int brk(void *addr);
//       void *sbrk(intptr_t increment);

int main() {
  void *cbrk;

  // Get the current brk
  cbrk= sbrk(0);
  printf("Initial brk is %p\n", cbrk);

  // Go up a few times
  for (int i=0; i < 30; i++) {
    cbrk= sbrk(0x1000);
    printf("New brk is %p\n", cbrk);
  }

  // Go down until it fails
  while (1) {
    cbrk= sbrk(-0x2000);
    printf("New brk is %p\n", cbrk);
    if (cbrk == (void *)-1) break;
  }

  // Get the current brk
  cbrk= sbrk(0);
  printf("Current brk is %p\n", cbrk);

  // Go up using brk()
  for (int i=0; i < 30; i++) {
    cbrk += 0x2000;
    if (brk(cbrk)==-1) break;
    printf("New brk is %p\n", cbrk);
  }

  // Go down using brk()
  while (1) {
    cbrk -= 0x723;
    if (brk(cbrk)==-1) break;
    printf("New brk is %p\n", cbrk);
  }
  return(0);
}
