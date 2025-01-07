#include <stdio.h>

int x= 3;
char ch= 'W';
long l=-23;
char *str= "Hello world";

int main() {
  printf("printf test\n");
  printf("int %d\n", x);
  printf("char %c\n", ch);
  printf("long %ld\n", l);
  printf("string %s\n", str);
  printf("pointer %p\n", str);
  printf("hex: %x %x %lx\n", x, ch, l);
  printf("oct: %o %o %lo\n", x, ch, l);
  return(0);
}
