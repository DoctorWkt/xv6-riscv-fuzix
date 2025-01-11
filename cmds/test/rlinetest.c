#include <stdio.h>
#include <readline/readline.h>
// #include <readline/history.h>


char *
       readline (const char *prompt);

int main() {
  char *ch;
  while (1) {
    ch= readline("> ");
    if (ch==NULL || ch[0]==0) break;
    printf(">>>%s<<<\n", ch);
  }
  return(0);
}
