#include <stdio.h>
#include <stdlib.h>

// malloc and free test

#define NUMPTRS 1024

void *ptr[NUMPTRS];

int main() {
  void *m;
  int idx, cnt;

  while (1) {
    // Try to allocate ten lumps of memory
    for (int i=0; i<10; i++) {
      // Find a NULL pointer in the array
      idx = rand() & (NUMPTRS-1);
      if (ptr[idx] != NULL) continue;

      cnt= rand() & (NUMPTRS-1);
      m= (void *)malloc(cnt);
      printf("malloc(%d) returned %p\n", cnt, m);
      if (m==NULL) continue;
      ptr[idx]= m;
    }

    // Now try to free an allocated lump
    idx = rand() & (NUMPTRS-1);
    if (ptr[idx] == NULL) continue;
    printf("freeing at address %p\n", ptr[idx]);
    free(ptr[idx]); ptr[idx]= NULL;
  }
  return(0);
}

       int rand(void);
